#pragma once

#ifdef FLAG_WINDOWS
	#define DLL_EXPORT __declspec( dllexport )
	#define DLL_IMPORT __declspec( dllimport )
#endif

#define ENTRYPOINT_PREFIX extern "C" {\
								                   DLL_EXPORT u32 NvOptimusEnablement = 0x00000001;\
								                   DLL_EXPORT i32 AmdPowerXpressRequestHighPerformance = 0x00000001;\
								                  }

#if defined( FLAG_WINDOWS )
	#define INLINE     __forceinline
	#define ENTRYPOINT ENTRYPOINT_PREFIX i32 CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,\
								                                                    LPSTR lpCmdLine, i32 nCmdShow ) 
#endif

#define EXIT( x )                        exitCode = x; goto exit##x;
#define SAFE_DELETE( x )                 if( x ) { delete( x ); x = nullptr; }
#define SAFE_ARRAY_DELETE( x )           if( x ) { delete[] x; x = nullptr; }
#define SAFE_STATIC_ARRAY_DELETE( x, y ) for( y* &z : x ) { SAFE_DELETE( z ); }
#define SAFE_RELEASE( x )                if( x ) { x->Release(); x = nullptr; }
#define SAFE_SHUTDOWN( x )               if( x ) { x->Shutdown(); x = nullptr; }
#define SAFE_INITIALIZE( x )             if( !x->Initialize() ) { return false; }
#define SAFE_INITIALIZE_EX( x, y )       if( !x->Initialize( y ) ) { return false; }

#define CORE_ALLOCATE( x )               new ( Instance.MemoryManager->GetCorePage()->Allocate( sizeof( x ) ) ) x()
#define RENDER_ALLOCATE( x )             new ( Instance.MemoryManager->GetRenderPage()->Allocate( sizeof( x ) ) ) x()
#define DEBUG_ALLOCATE( x )              new ( Instance.MemoryManager->GetDebugPage()->Allocate( sizeof( x ) ) ) x()

#define CORE_ALLOCATE_EX( x, y )         new ( Instance.MemoryManager->GetCorePage()->Allocate( sizeof( x ) ) ) x y
#define RENDER_ALLOCATE_EX( x, y )       new ( Instance.MemoryManager->GetRenderPage()->Allocate( sizeof( x ) ) ) x y
#define DEBUG_ALLOCATE_EX( x, y )        new ( Instance.MemoryManager->GetDebugPage()->Allocate( sizeof( x ) ) ) x y

#define CORE_ALLOCATED_FREE( x )         Instance.MemoryManager->GetCorePage()->Free( x )
#define RENDER_ALLOCATED_FREE( x )       Instance.MemoryManager->GetRenderPage()->Free( x )
#define RENDER_API_ALLOCATED_FREE( x )   Instance.MemoryManager->GetRenderApiPage()->Free( x )
#define DEBUG_ALLOCATED_FREE( x )        Instance.MemoryManager->GetDebugPage()->Free( x )

#ifdef FLAG_DEBUG
	#ifdef FLAG_WINDOWS
		#define CONSOLE_PRINT_ALL( x, ... )     Instance.Console->Printf( MESSAGE_CHANNEL_ALL, x, ##__VA_ARGS__ )
		#define CONSOLE_PRINT_INFO( x, ... )    Instance.Console->Printf( MESSAGE_CHANNEL_INFO, x, ##__VA_ARGS__ )
		#define CONSOLE_PRINT_WARNING( x, ... ) Instance.Console->Printf( MESSAGE_CHANNEL_WARNING, x, ##__VA_ARGS__ )
		#define CONSOLE_PRINT_ERROR( x, ... )   Instance.Console->Printf( MESSAGE_CHANNEL_ERROR, x, ##__VA_ARGS__ )
		#define CONSOLE_PUMP()                  Instance.Console->EventPump()
		#define FILE_WATCHDOG()                 if( Instance.FileWatchdog ) Instance.FileWatchdog->Pump()
	#endif
		#define ON_FATAL_ERROR( x, y )          Instance.Console->OnFatalError( x, y )
#else
	#define CONSOLE_PRINT_ALL( x, ... )
	#define CONSOLE_PRINT_INFO( x, ... )
	#define CONSOLE_PRINT_WARNING( x, ... )
	#define CONSOLE_PRINT_ERROR( x, ... )
	#define CONSOLE_PUMP()
	#define FILE_WATCHDOG()
	#define ON_FATAL_ERROR( x, y ) DisplayErrorDialog( L"i0rTech - FATAL ERROR", _bstr_t( x ), L"A fatal error has occured!", _bstr_t( y ) )
#endif
