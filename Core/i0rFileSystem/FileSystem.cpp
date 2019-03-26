#include "../Common.hpp"

#include <i0rCore/Helpers/File.hpp>
#include <i0rCore/Helpers/Class.hpp>

#include "FBinary.hpp"
#include "FTexture.hpp"
#include "FStaticScene.hpp"

#include <i0rCore/FSmallGeometry.hpp>
#include <i0rRender/Font.hpp>
#include <i0rCore/Dialogs.hpp>

#define ZLIB_WINAPI
#include "zlib/zlib.h"

#ifdef FLAG_32
	#pragma comment( lib, "../Core/i0rFileSystem/zlib/zdll.lib" )
#elif defined FLAG_64
	#pragma comment( lib, "../Core/i0rFileSystem/zlib/zdll64.lib" )
#endif

#ifdef FLAG_DEBUG
	#include "FileWatchdog.hpp"
#elif defined( FLAG_RELEASE )
	#define FLAG_VIRTUAL_FILE_SYSTEM
#endif

#include "FileSystem.hpp"

FileSystem::FileSystem() {
	m_Size      = 0;
	m_Name      = "";
	m_Pointer   = nullptr;

	#ifdef FLAG_DEBUG
		m_Watchdog = nullptr;
	#endif

	#ifdef FLAG_WINDOWS
		m_Handle   = nullptr;
	#endif
}

FileSystem::~FileSystem() {
	m_Size = 0;
	m_Name.clear();

	Unmount();

	for( std::pair<u32, file_t*> file : m_Files ) {
		file.first = 0;
		SAFE_DELETE( file.second )
	}

	m_Files.clear();

	#if defined( FLAG_DEBUG )
		m_Watchdog->~IFileWatchdog();
		DEBUG_ALLOCATED_FREE( m_Watchdog );
		m_Watchdog = nullptr;
	#endif
}

bool FileSystem::Mount( const char* fileSystemName ) {
	#ifdef FLAG_VIRTUAL_FILE_SYSTEM
		if( !::DoesFileExist( fileSystemName ) ) {
			ON_FATAL_ERROR( __FUNCTION__, "FileSystem does not exist!" );
			return false;
		}

		m_Name = fileSystemName;

		if( !PrepareMemory() ) {
			return false;
		}

		if( !PrepareFileSystem() ) {
			return false;
		}

		return true;
	#endif

	#ifdef FLAG_DEBUG
		m_Watchdog = DEBUG_ALLOCATE( FileWatchdog );
	
		const bool watchdogIni = m_Watchdog->Initialize();
		if( watchdogIni ) Instance.FileWatchdog = m_Watchdog;
		return watchdogIni;
	#endif
}

void FileSystem::Unmount() {
	#if defined( FLAG_WINDOWS ) && defined( FLAG_VIRTUAL_FILE_SYSTEM )
		memset( (void*)m_Pointer, 0, m_Size );
		UnmapViewOfFile( m_Pointer );
		CloseHandle( m_Handle );
		m_Handle  = nullptr;
		m_Pointer = nullptr;
	#endif
}

bool FileSystem::IsPathValid( const char* path ) {
	ASSERT( false, "FUNCTION IS NOT IMPLEMENTED" )
	return true;
}

file_t* FileSystem::GetFile( const char* file ) {
	return m_Files[HashStringCRC32( file )];
}

void* FileSystem::GetFileUncompressed( const char* file ) {
#ifdef FLAG_VIRTUAL_FILE_SYSTEM
	file_t* f = GetFile( file );

	if( !f ) {
		CONSOLE_PRINT_ERROR("FileSystem::GetFileUncompressed => File %s not found\n", file );
		return nullptr;
	}

	Bytef* fUncompress = ( Bytef* )malloc( f->UncompressedDataSize + 1 ); // + 1 for null terminator
	memset( fUncompress, 0, f->UncompressedDataSize );

	uLongf uncompressedSize[1] = { f->UncompressedDataSize };
	Bytef* srcPtr = ( Bytef* )( m_Pointer + f->CompressedData + 30 + strlen( f->Name ) );

	z_stream strm  = {};

	strm.next_in   = srcPtr;
	strm.avail_in  = f->CompressedDataSize;

	strm.next_out  = fUncompress;
	strm.avail_out = f->UncompressedDataSize;

	strm.zalloc = Z_NULL;
	strm.zfree  = Z_NULL;
	strm.opaque = Z_NULL;

	i32 uResultIni = inflateInit2( &strm, -MAX_WBITS ); // avoids zip header check

	i32 uResult = inflate( &strm, Z_FINISH );
	
	inflateEnd( &strm );

	if( uResult < 0 ) {
		CONSOLE_PRINT_WARNING( "FileSystem::GetFileUncompressed => File %s : inflate returned %i...\n", file, uResult );
	}

	fUncompress[f->UncompressedDataSize] = '\0'; // avoid non null terminated char array

	return fUncompress;
#elif defined( FLAG_DEBUG )
	void* dataPtr = nullptr;
	const std::string debugPath = "Base/" + std::string( file );
	FBinary::ReadFast( debugPath.c_str(), dataPtr );
	return dataPtr;
#endif
}

bool FileSystem::PrepareMemory() {
	void* fileSystemPointer = nullptr;
	m_Size = FBinary::ReadFast( m_Name.c_str(), fileSystemPointer );

	if( !fileSystemPointer || !m_Size ) {
		return false;
	}

	#if defined FLAG_WINDOWS && defined FLAG_VIRTUAL_FILE_SYSTEM
		m_Handle = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, (DWORD)m_Size,
								                        "i0rTechFileSystem" );

		if( !m_Handle ) {
			return false;
		}

		m_Pointer = (LPTSTR)MapViewOfFile( m_Handle, FILE_MAP_ALL_ACCESS, 0, 0, m_Size );

		if( !m_Size ) {
			return false;
		}

		memcpy( (void*)m_Pointer, fileSystemPointer, m_Size );
	#endif
	
	FBinary::Free( fileSystemPointer );
	return true;
}

bool FileSystem::PrepareFileSystem() {
	const file_system_pointer_t ptrEnd = m_Pointer + m_Size - 6;
	file_system_pointer_t ptr = ( *(u32*)ptrEnd + m_Pointer );

	if( !ptr ) {
		CONSOLE_PRINT_ERROR( "FileSystem::PrepareFileSystem => Central directory offset is NULL!\n" );
		return false;
	}

	file_t* file               = nullptr;
	zip_local_header_t zHeader = {};
	FILE_READ_SIZE( char, 20 ); // skip unused infos (should be removed later)

	while( ptr < ptrEnd ) {
		zHeader = FILE_READ_SIZE( zip_local_header_t, ZIP_LOCAL_HEADER_SIZE );

		if( !zHeader.FileNameLength ) {
			CONSOLE_PRINT_WARNING( "FileSystem::PrepareFileSystem => 0x%X : nameless file found.\n", ptr );
			continue;
		}

		if( zHeader.CompressedSize ) {
			file = new file_t();

			file->CompressedData = *(u32*)( ptr - sizeof( u32 ) ); // has to be done after the struct (since the memory alignement jumps several bytes)

			file->Name = new char[zHeader.FileNameLength + 1];
			memcpy( file->Name, ptr, ( zHeader.FileNameLength ) * sizeof( char ) ); // ignore the carriage return
			file->Name[zHeader.FileNameLength] = '\0';

			ptr += ( zHeader.FileNameLength + 1 ) * sizeof( char );
				
			file->CompressedDataSize   = zHeader.CompressedSize;
			file->UncompressedDataSize = zHeader.UncompressedSize;
			ASSERT( ( file->CompressedDataSize > 64 ), "File is too small for VFS!" )
			m_Files.insert( std::make_pair( HashStringCRC32( file->Name ), file ) );
		} else { // ignore folder (not useful for now)
			ptr += (zHeader.FileNameLength + 1) * sizeof(char);
		}

		FILE_READ_SIZE( char, 55 );
	}

	return true;
}

file_texture_t* FileSystem::GetFileTexture( const char* file ) {
	char* ptr = (char*)GetFileUncompressed( file );
	
	if( !ptr ) {
		return nullptr;
	}

	char* ptrCpy        = ptr;
	u32 texMagic        = FILE_READ( u32 );
	file_texture_t* tex = nullptr;

	switch( texMagic ) {
		case 0x20534444:
			tex = FTextureDDS::Load( ptr );
			break;

		default:
			CONSOLE_PRINT_INFO( "FileManager => Unknown texture magic 0x%x\n", texMagic );
			break;
	}

	FreeUncompressedFile( ptrCpy );
	return tex;
}

char* FileSystem::GetFileText( const char* file ) {
	char* str = (char*)GetFileUncompressed( file );
	return str;
}

file_small_geometry_t* FileSystem::GetFileSmallGeometry( const char* file ) {
	char* ptr = (char*)GetFileUncompressed( file );
	
	if( !ptr ) {
		return nullptr;
	}

	file_small_geometry_t* smallGeo = new file_small_geometry_t();
	smallGeo->Infos                 = FILE_READ( file_header_sge_t );
	smallGeo->BaseAddress           = ptr;

	return smallGeo;
}

void FileSystem::FreeUncompressedFile( void* memPtr ) {
	free( memPtr );
}

void* FileSystem::GetFileFont( const char* file, font_informations_t &fontInfos ) {
	u8* ptr = ( u8* )GetFileUncompressed( file ); // since font file are stored as unsigned...

	if( !ptr ) {
		return nullptr;
	}
	
	u8* ptrCpy = ptr; // keep track of the base address
	fontInfos  = FILE_READ( font_informations_t );

	u8* texels = (u8*)Instance.MemoryManager->GetCorePage()->Allocate( fontInfos.height * fontInfos.width * sizeof( u8 ) );
	i32 x = 0, y = 0, pos = 0, posY = 0, posX = 0;

	for( i32 i = 0; i < fontInfos.height; ++i ) {
		for( i32 j = 0; j < fontInfos.width; ++j ) {
			x    = j >> 3;
			y    = i >> 4;

			pos  = y * fontInfos.width / fontInfos.glyphWidth + x;
			posY = i - ( y << 4 );
			posX = j - ( x << 3 );

			if ( *( u8* )( ptr + ( pos * fontInfos.glyphHeight + posY ) ) & ( 1 << posX ) ) {
				texels[i * fontInfos.width + j] = 0xFF;
			}
			else {
				texels[i * fontInfos.width + j] = 0x0;
			}
		}
	}

	FreeUncompressedFile( ptrCpy );
	return texels;
}

bool FileSystem::LoadStaticScene( const char* file ) {
	char* ptr = (char*)GetFileUncompressed( file );

	if( !ptr ) {
		return nullptr;
	}

	char* ptrCpy                 = ptr;
	file_header_its_t fileHeader = FILE_READ( file_header_its_t );

	if( fileHeader.Magic != SCENE_MAGIC ) {
		CONSOLE_PRINT_ERROR( "FileSystem::LoadStaticScene => Scene %s : unexpected magic 0x%X\n", file, fileHeader.Magic );
		return false;
	}

	if( fileHeader.VersionMajor != SCENE_VERSION_MAJOR || fileHeader.VersionMinor != SCENE_VERSION_MINOR 
		|| fileHeader.VersionPatch != SCENE_VERSION_PATCH ) {
		CONSOLE_PRINT_ERROR( "FileSystem::LoadStaticScene => Scene %s : invalid static scene version\n", file );
		return false;
	}

	texture_dictionary_t textureDictionary = {};
	file_bloc_generic_t currentBloc        = {};
	u32 i = sizeof( file_header_its_t ) + sizeof( ISceneManager::scene_informations_t );

	while( *( char* )ptr != 0x0 ) {
		currentBloc = FILE_READ_RECURSIVE( file_bloc_generic_t );

		switch( currentBloc.Magic ) {
			case SCENE_SPAWN_MAGIC:
				CONSOLE_PRINT_INFO( "FileManager::LoadScene => %i spawns\n", currentBloc.Count );
				FSceneStatic::LoadSceneSpawnPoints( &currentBloc, ptr, Instance.SceneManager );
				break;

			case SCENE_DIR_LIGHT_MAGIC:
				CONSOLE_PRINT_INFO( "FileManager::LoadScene => %i dir lights\n", currentBloc.Count );
				break;

			case SCENE_POINT_LIGHT_MAGIC:
				CONSOLE_PRINT_INFO( "FileManager::LoadScene => %i point lights\n", currentBloc.Count );
				FSceneStatic::LoadScenePointLights( &currentBloc, ptr, Instance.SceneManager );
				break;

			case SCENE_MATERIAL_MAGIC:
				CONSOLE_PRINT_INFO( "FileManager::LoadScene => %i materials\n", currentBloc.Count );
				FSceneStatic::LoadSceneMaterials( &currentBloc, ptr, Instance.SceneManager, textureDictionary );
				break;

			case SCENE_TEXTURE_DICO:
				CONSOLE_PRINT_INFO( "FileManager::LoadScene => %i textures\n", currentBloc.Count );
				FSceneStatic::LoadSceneTextures( &currentBloc, ptr, textureDictionary );
				break;

			case SCENE_GEOM_MAGIC:
				CONSOLE_PRINT_INFO( "FileManager::LoadScene => %i meshes\n", currentBloc.Count );
				FSceneStatic::LoadSceneMeshes( &currentBloc, ptr, Instance.SceneManager );
				break;

			default:
				CONSOLE_PRINT_WARNING( "FileManager::LoadScene => Unknown bloc 0x%X\n", currentBloc.Magic );
				break;
		}

		i   += currentBloc.Size;
		ptr += currentBloc.Size;
		ReadPadding( ptr, i, 16 );
	}

	FreeUncompressedFile( ptrCpy );
	return true;
}
