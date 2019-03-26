#pragma once

/*====================================
	Build Flags
	====================================*/
#if defined( _DEBUG ) || defined( DEBUG ) || !defined( NDEBUG )
	#define FLAG_DEBUG
#elif !defined( _DEBUG ) && !defined( DEBUG )
	#define FLAG_RELEASE
#endif

/*====================================
	System Flags
	====================================*/
#if defined( _WIN32 ) || defined( _WIN64 )
	#define FLAG_WINDOWS
#elif defined( __linux__ )
	#define FLAG_LINUX
#elif defined( __APPLE__ ) || defined( __MACH__ )
	#define FLAG_OSX
#elif defined( __FreeBSD__ )
	#define FLAG_BSD
#elif defined( unix ) || defined( __unix ) || defined( __unix__ )
	#define FLAG_UNIX
#endif

/*====================================
	CPU Architecture Flags
	====================================*/
#if defined( _M_X64 ) || defined( __amd64 ) || defined( __amd64__ )
	#define FLAG_64
#elif defined( _M_IX86 )
	#define FLAG_32
#elif defined( __arm__ )
	#define FLAG_ARM_32
#elif defined( __aarch64__ )
	#define FLAG_ARM_64
#endif

	/*====================================
	Available GRAPHICS API
		Should be switchable somewhere in the options
	====================================*/
#define FLAG_OPENGL
