#include "../Common.hpp"

#if defined( FLAG_DEBUG )
	#include <i0rCore/ITaskManager.hpp>
	#include <i0rCore/IEngine.hpp>
	#include <i0rRender/ITextureManager.hpp>

	#include "FileWatchdog.hpp"

FileWatchdog::FileWatchdog() {
	memset( m_WorkingDirectory, 0, FILENAME_MAX * sizeof( char ) );
}

FileWatchdog::~FileWatchdog() {
	memset( m_WorkingDirectory, 0, FILENAME_MAX * sizeof( char ) );

	CancelIoEx( m_Watchdog, nullptr );
	CloseHandle( m_Watchdog );
	m_Watchdog = nullptr;

	if( m_ToReload.size() ) {
		file_watched_t* file = m_ToReload.front();

		while( file ) {
			m_ToReload.pop();
			SAFE_DELETE( file )
			if( !m_ToReload.size() ) break;
			file = m_ToReload.front();
		}

		file = nullptr;
	}
}
	
bool FileWatchdog::Initialize() {
	_getcwd( m_WorkingDirectory, FILENAME_MAX );
	CONSOLE_PRINT_INFO( "FileWatchdog::Initialize() => Working directory : %s\n", m_WorkingDirectory );
	m_Watchdog = ::CreateFile( m_WorkingDirectory, FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL );
	Instance.TaskManager->AddAsyncTask( std::bind( &FileWatchdog::Execute, this ) );
	return true;
}

void FileWatchdog::Pump() {
	if( m_ToReload.size() ) {
		file_watched_t* file = m_ToReload.front();
		std::string fsName = file->Name;
		RemoveWord( fsName, "Base/" );

		while( file ) {
			switch( file->Type ) {
				case FILE_WATCHED_TYPE_TEXTURE:
					Instance.TextureManager->Reload( fsName );
					break;

				case FILE_WATCHED_TYPE_SHADER:
					Instance.ShaderManager->Reload( fsName );
					break;

				case FILE_WATCHED_TYPE_MESH:
					break;

				case FILE_WATCHED_TYPE_UNKNOWN:
				default:
					break;
			};

			m_ToReload.pop();
			SAFE_DELETE( file )
			if( !m_ToReload.size() ) break;
			file = m_ToReload.front();
		}
	}
}

void FileWatchdog::Execute() {
	const DWORD MAX_BUFFER = 1024;

	BYTE Buffer[MAX_BUFFER] = {};
	WCHAR fileNameModified[FILENAME_MAX] = {};

	DWORD dwBytesReturned = 0;
	BOOL changesResult = FALSE;

	while( 1 ) {
		if( !Instance.Engine->IsRunning() ) {
			return;
		}

		if( changesResult && dwBytesReturned ) {
			const FILE_NOTIFY_INFORMATION* pNotifyInfo = (FILE_NOTIFY_INFORMATION*)Buffer;
			memcpy( fileNameModified, pNotifyInfo->FileName, pNotifyInfo->FileNameLength );

			if( pNotifyInfo->Action == FILE_ACTION_MODIFIED || pNotifyInfo->Action == FILE_ACTION_REMOVED ) {
				std::string relativeFileName = WideToMultibyte( fileNameModified );
				WindowsPathToEnginePath( relativeFileName );

				std::string extension = GetFileExtensionFromPath( relativeFileName );
				ToLower( extension );

				file_watched_t* file = new file_watched_t();
				file->Name = relativeFileName;

				if( extension == "dds" ) {
					file->Type = FILE_WATCHED_TYPE_TEXTURE;
				} else if( extension == "gl" ) {
					file->Type = FILE_WATCHED_TYPE_SHADER;
				} else {
					file->Type = FILE_WATCHED_TYPE_UNKNOWN;
				}
					
				m_ToReload.push( file );
				CONSOLE_PRINT_INFO( "FileWatchdog::Execute => File/folder %s has been edited.\n", relativeFileName.c_str() );
			}
		}

		memset( Buffer, 0, MAX_BUFFER * sizeof( BYTE ) );
		memset( fileNameModified, 0, FILENAME_MAX * sizeof( WCHAR ) );

		changesResult = ReadDirectoryChangesW( m_Watchdog, Buffer, MAX_BUFFER, TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE, 
								                                 &dwBytesReturned, 0, 0 );
	}
}
#endif
