#include "Common.hpp"

#include <i0rCore/Module.hpp>
#include <i0rGame/IGameLogic.hpp>
#include <i0rCore/Library.hpp>
#include <i0rRender/IRenderer.hpp>

#include "i0rTaskManager/TaskManager.hpp"
#include "i0rFileSystem/FileSystem.hpp"
#include "i0rInputHandler/InputHandler.hpp"
#include "i0rConsole/Console.hpp"
#include "i0rMemoryManager/MemoryManager.hpp"
#include "i0rSceneManager/SceneManager.hpp"

#include "i0rSystem/Configuration.hpp"

#include "Engine.hpp"

MODULE( Engine )

Engine::Engine() {
	m_IsRunning        = true;
	m_DeltaFrameTime   = 0.0;
	m_RenderLibrary    = nullptr;
}

void Engine::Shutdown() {
	m_DeltaFrameTime   = 0.0;
	Instance.GameLogic = nullptr;

	Instance.FileSystem->~IFileSystem();
	CORE_ALLOCATED_FREE( Instance.FileSystem );
	Instance.FileSystem = nullptr;

	Instance.TaskManager->~ITaskManager();
	CORE_ALLOCATED_FREE( Instance.TaskManager );
	Instance.TaskManager = nullptr;

	Instance.InputHandler->~IInputHandler();
	CORE_ALLOCATED_FREE( Instance.InputHandler );
	Instance.InputHandler = nullptr;

	SAFE_SHUTDOWN( Instance.Renderer )

	Instance.SceneManager->~ISceneManager();
	CORE_ALLOCATED_FREE( Instance.SceneManager );
	Instance.SceneManager = nullptr;

	Instance.Console->~IConsole();
	DEBUG_ALLOCATED_FREE( Instance.Console );
	Instance.Console = nullptr;

	LIBRARY_FREE_LIBRARY( m_RenderLibrary );
	m_RenderLibrary = nullptr;

	SAFE_DELETE( Instance.GameLogic )

	SAFE_DELETE( Instance.MemoryManager )
}

bool Engine::Initialize( IGameLogic* gameLogic ) {
	Instance.MemoryManager = new MemoryManager;
	SAFE_INITIALIZE( Instance.MemoryManager )

	#if defined( FLAG_DEBUG ) && defined( FLAG_WINDOWS )
		Instance.Console       = DEBUG_ALLOCATE( Console );

		SAFE_INITIALIZE( Instance.Console )
		CONSOLE_PRINT_INFO( "i0rTech : %s - %s %s\n", gameLogic->GetProjectName(), __DATE__, __TIME__ );
	#endif

	Configuration::LoadInput();
	Configuration::LoadVideo();

	// prepare the instances table
	Instance.Engine         = this;
	Instance.Configuration  = &_Configuration;
	Instance.GameLogic      = gameLogic;

	
	Instance.TaskManager   = CORE_ALLOCATE( TaskManager );
	Instance.FileSystem    = CORE_ALLOCATE( FileSystem );
	Instance.InputHandler  = CORE_ALLOCATE( InputHandler );
	Instance.SceneManager  = CORE_ALLOCATE( SceneManager );

	m_RenderLibrary        = LIBRARY_GET_LIBRARY_HANDLE( BuildLibraryName( "i0rTechRender" ) );

	if( !m_RenderLibrary ) return false;

	Instance.Renderer = GetModuleInstance<IRenderer>( m_RenderLibrary, "GetRendererInstance" );

	if( !Instance.Renderer ) return false;

	SAFE_INITIALIZE( Instance.TaskManager )

	if( !Instance.FileSystem->Mount() ) return false;

	SAFE_INITIALIZE( Instance.InputHandler )

	instances_t* instancesPtr = &Instance;
	SAFE_INITIALIZE_EX( Instance.Renderer,  instancesPtr )
	SAFE_INITIALIZE_EX( Instance.GameLogic, instancesPtr )

	return true;
}

void Engine::Run() {
	u64 previousFrameTime     = Instance.MainRenderWindow->GetElapsedTime(),
					frameTime             = 0,
					interpolatedFrameTime = 0,
					timedActionsTimer     = GetClientTimeAsMs();

	f64 frameTimeSum          = 0.0;

	while( 1 ) {
		frameTime        = Instance.MainRenderWindow->GetElapsedTime();
		m_DeltaFrameTime = ( f64 )( frameTime - previousFrameTime );

		if( m_DeltaFrameTime > ENGINE_SOD ) m_DeltaFrameTime = ENGINE_SOD;

		previousFrameTime  = frameTime;
		frameTimeSum      += m_DeltaFrameTime;

		// render effects, input keys, ...
		if( GetClientTimeAsMs() - timedActionsTimer >= 500 ) {
				Instance.InputHandler->ResetOncePerSecKeys();

				timedActionsTimer = GetClientTimeAsMs();
		}

		Instance.MainRenderWindow->EventPump();

		// debug specific calls
		CONSOLE_PUMP();
		FILE_WATCHDOG();

		while( frameTimeSum >= ENGINE_UPDATE_DELTA ) {
			Instance.InputHandler->Poll();
			Instance.GameLogic->OnTick();

			frameTimeSum -= ENGINE_UPDATE_DELTA;
		}

		Instance.GameLogic->OncePerTick();
		Instance.SceneManager->ComputeVisibility();

		if( !m_IsRunning ) break;

		interpolatedFrameTime = ( u64 )( frameTimeSum / ENGINE_UPDATE_DELTA );

		Instance.Renderer->OnTick();
		Instance.Renderer->Draw();
		Instance.Renderer->Swap();
	}
}
