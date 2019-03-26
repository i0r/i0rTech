#pragma once

#ifdef FLAG_64
	#define LIBRARY_ARCHITECTURE "64"
#elif defined( FLAG_32 )
	#define LIBRARY_ARCHITECTURE "32"
#else
	#define LIBRARY_ARCHITECTURE "64"
#endif

#ifdef FLAG_DEBUG
	#define LIBRARY_BUILD "-debug"
#else
	#define LIBRARY_BUILD
#endif

#ifdef FLAG_WINDOWS
	#define LIBRARY_PREFIX                              ""
	#define LIBRARY_EXTENSION                           ".dll"
	#define LIBRARY_GET_LIBRARY_HANDLE( x )             LoadLibrary( x )
	#define LIBRARY_GET_LIBRARY_FUNCTION_HANDLE( x, y ) GetProcAddress( x, y )
	#define LIBRARY_FREE_LIBRARY( x )                   if( x ) { FreeLibrary( x );  }
	#define LIBRARY_SAFE_FREE_LIBRARY( x )              if( x ) { FreeLibrary( x ); x = nullptr; }
#endif

#define LIBRARY_SUFFIX ( LIBRARY_ARCHITECTURE LIBRARY_BUILD LIBRARY_EXTENSION )

/*====================================
	BuildLibraryName
		Returns a library name according to the engine environment (e.g. CPU, BUILD, ...)
			const char* libraryFileName: library name ONLY (no extension or suffixes/prefixes)
====================================*/
static INLINE char* BuildLibraryName( const char* libraryFileName ) {
	char* libraryName = new char[MAX_PATH];

	strcpy_s( libraryName, 6, LIBRARY_PREFIX );
	strcat_s( libraryName, MAX_PATH - 30, libraryFileName );
	strcat_s( libraryName, 26, LIBRARY_SUFFIX );

	return libraryName;
}

/*====================================
	GetInstance
		Returns an instance located in a specific library
		NOTE: the library handle must be closed manually after the function call
			const library_handle_t library: library handle
			const char* functionName: getter name
====================================*/
template<typename T>
static T* GetModuleInstance( const library_handle_t library, const char* functionName ) {
	T* instance = nullptr;

	typedef T* ( *U )();

	const U functionInstance = ( U )LIBRARY_GET_LIBRARY_FUNCTION_HANDLE( library, functionName );

	if( !functionInstance ) {
		return instance;
	}

	instance = ( functionInstance )();

	return instance;
}
