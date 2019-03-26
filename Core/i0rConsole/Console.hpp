#pragma once

#include <i0rCore/IConsole.hpp>

#define CONSOLE_NAME               "i0rTechConsole"
#define CONSOLE_WIDTH              640
#define CONSOLE_HEIGHT             480
#define CONSOLE_MARGIN             5
#define CONSOLE_HISTORY_SIZE       64
#define CONSOLE_MAX_BUFFER_SIZE    4096

#if defined( FLAG_DEBUG ) && defined( FLAG_WINDOWS )
	static const LPCSTR lpszClassName = "i0rTechConsoleFrame";

	// WINGUI BUTTONS
	#define CONSOLE_COMMAND_CLIPBOARD  1
	#define CONSOLE_COMMAND_FILE       2
	#define CONSOLE_COMMAND_CLEAR      3
	#define CONSOLE_COMMAND_EXIT       4

#define CONSOLE_UPDATE_FILTER( x ) const bool newState = !SendMessage( (HWND)lParam, BM_GETCHECK, 0, 0 );\
								                           SendMessage( (HWND)lParam, BM_SETCHECK, (WPARAM)newState, 0 );\
								                           Instance.Console->OnFilterUpdate( x, newState )


	class Console : public IConsole {
		public:
			explicit Console() ;
			~Console();
					
			bool Initialize() ;
			void Printf( const console_message_channel_t channel, const char* text, ... ) ;

			void EventPump() ;
			void OnFatalError( const char* function, const char* errorText ) ;
			void ClearConsole() ;
			void CopyToClipboard() ;
			void CopyToFile() ;

			void OnFilterUpdate( const console_message_channel_t channel, const bool state ) ;

		private:
			void RefreshConsole( i32 &historyPointer, const i32 historyLimit ) ;
				
			static LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) ;

			char                         m_ConsoleFinalText[CONSOLE_MAX_BUFFER_SIZE];
			std::string                  m_History[CONSOLE_HISTORY_SIZE];
			console_message_channel_t    m_HistoryType[CONSOLE_HISTORY_SIZE];
	};
#endif
