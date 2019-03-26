#pragma once

/*====================================
	IsAlreadyRunning
		Checks whether or not the engine is already running (based on its appname)
			application_handle_t &handle: reference to the app handle
			const char* appName: appname
====================================*/
INLINE static bool IsAlreadyRunning( application_handle_t &handle, const char* appName ) {
	#ifdef FLAG_WINDOWS
		handle = CreateMutex( 0, 1, appName );
		return ( GetLastError() == ERROR_ALREADY_EXISTS );
	#endif
}

#ifdef FLAG_WINDOWS
	#define FREE_APPLICATION_HANDLE( x ) CloseHandle( x ); x = NULL;
#endif
