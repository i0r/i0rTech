#include <i0rCore/Common.hpp>
#include <i0rCore/Library.hpp>
#include <i0rCore/Dialogs.hpp>
#include <i0rCore/Module.hpp>
#include <i0rCore/Application.hpp>
#include <i0rCore/IEngine.hpp>
#include "GameLogic.hpp"

ENTRYPOINT {
	application_handle_t clientHandle = NULL;
	i32 exitCode                      = 0;
	GameLogic* gameLogic              = nullptr;

	if( IsAlreadyRunning( clientHandle, "i0rTechClient" ) ) {
		DisplayErrorDialog( L"i0rTech::Fatal Error - Application Already Running",
								              L"The application is already running on your computer.",
								              L"One or several application using i0rTech is already running on your computer.",
								              L"Close the current instance to continue." );
		EXIT( 1 )
	}

	const char* coreLibraryName = BuildLibraryName( "i0rTechCore" );

	while( !DoesFileExist( coreLibraryName ) ) {
		if( !DisplayErrorDialog( L"i0rTech::Fatal Error - Missing Library",
								                   _bstr_t( coreLibraryName ),
								                   L"File does not exist and/or is empty.",
								                   L"Please re-install the game or replace the missing file to continue",
								                   DIALOG_FLAG_BUTTON_CANCEL | DIALOG_FLAG_BUTTON_RETRY ) ) {
			EXIT( 2 )
		}
	}

	const library_handle_t coreLibraryHandle = LIBRARY_GET_LIBRARY_HANDLE( coreLibraryName );
	IEngine* engine = GetModuleInstance<IEngine>( coreLibraryHandle, "GetEngineInstance" );

	if( !engine ) {
		DisplayErrorDialog( L"i0rTech::Fatal Error - Corrupted Library",
								              _bstr_t( coreLibraryName ),
								              L"Library might be corrupted; unsuitable for this operating system or empty.",
								              L"Please re-install the game or replace the missing file to continue" );
		EXIT( 3 )
	}

	gameLogic = new GameLogic();
	if( !engine->Initialize( gameLogic ) ) {
		DisplayErrorDialog( L"i0rTech::Fatal Error - Engine Initialization",
								              L"Module : Engine",
								              L"Failed to initialize the engine (or one of its module)...",
								              nullptr );
		EXIT( 4 )
	}

	gameLogic->SetActiveGameMode( GAME_LOGIC_MODE_TEST );
	engine->Run();

	exit4:
		SAFE_SHUTDOWN( engine )
		gameLogic = nullptr;
	exit3 :
		LIBRARY_FREE_LIBRARY( coreLibraryHandle )
	exit2 :
		delete[] coreLibraryName;
		FREE_APPLICATION_HANDLE( clientHandle );
	exit1:
		return exitCode;
}
