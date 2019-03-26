#pragma once

#include <i0rCore/IFileSystem.hpp>

struct texture_t;
struct file_small_geometry_t;

struct file_t {
	~file_t() {
		CompressedData       = 0;
		CompressedDataSize   = 0;
		UncompressedDataSize = 0;
		delete Name;
	}
	
	u32              CompressedData;
	u32              CompressedDataSize;
	u32              UncompressedDataSize;
	char*            Name;
};

#define ZIP_LOCAL_HEADER_SIZE 26

struct zip_local_header_t {
	u32  CompressedSize;
	u32  UncompressedSize;

	u16  FileNameLength;

	u16  DontCare;
	u8   __PADDING__[10];
	u32  Offset;
};

class FileSystem : public IFileSystem {
	public:
		explicit FileSystem() ;
		~FileSystem() ;

		bool                    Mount( const char* fileSystemName = "Base.gfs" ) ;
		void                    Unmount() ;
		void                    FreeUncompressedFile( void* memPtr ) ;
		bool                    IsPathValid( const char* path ) ;
		file_t*                 GetFile( const char* file ) ;
		void*                   GetFileUncompressed( const char* file ) ;
		file_texture_t*         GetFileTexture( const char* file ) ;
		char*                   GetFileText( const char* file ) ;
		file_small_geometry_t*  GetFileSmallGeometry( const char* file ) ;
		void*                   GetFileFont( const char* file, font_informations_t &infos ) ;
		bool                    LoadStaticScene( const char* file ) ;

	private:
		bool                    PrepareMemory() ;
		bool                    PrepareFileSystem() ;

		std::map<u32, file_t*>  m_Files;
};
