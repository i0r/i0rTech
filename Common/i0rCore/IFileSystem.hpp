#pragma once

struct file_t;
struct file_texture_t;
struct file_small_geometry_t;
struct font_informations_t;

class IFileSystem {
	DECLARE_MEMBER( protected,  size_t,                 Size )
	DECLARE_MEMBER( protected,  std::string,            Name )
	DECLARE_MEMBER( protected,  file_system_pointer_t,  Pointer )

	#ifdef FLAG_DEBUG
		DECLARE_MEMBER( protected, IFileWatchdog*,         Watchdog )
	#endif

	#ifdef FLAG_WINDOWS
		DECLARE_MEMBER( protected, file_system_handle_t,   Handle )
	#endif

	public:
		virtual ~IFileSystem() {}

		virtual bool                   Mount( const char* fileSystemName = "Base.gfs" )             = 0;
		virtual void                   Unmount()                                                    = 0;

		virtual void                   FreeUncompressedFile( void* memPtr )                         = 0;
		virtual bool                   IsPathValid( const char* path )                              = 0;

		virtual file_t*                GetFile( const char* file )                                  = 0;
		virtual void*                  GetFileUncompressed( const char* file )                      = 0;
		virtual file_texture_t*        GetFileTexture( const char* file )                           = 0;
		virtual char*                  GetFileText( const char* file )                              = 0;
		virtual file_small_geometry_t* GetFileSmallGeometry( const char* file )                     = 0;
		virtual void*                  GetFileFont( const char* file, font_informations_t &infos )  = 0;
		
		virtual bool                   LoadStaticScene( const char* file )                          = 0;
};
