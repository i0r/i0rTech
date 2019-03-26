#include "../Common.hpp"
#include <i0rCore/IEngine.hpp>
#include <i0rCore/Module.hpp>
#include "Console.hpp"

#if defined( FLAG_DEBUG ) && defined( FLAG_WINDOWS )
	Console::Console() {
		m_IsDestroyed            = false;
		m_BlinkingTime           = false;
		m_HistoryPointer         = 0x0;
		m_Filter                 = 0x7;
		m_Window                 = nullptr;
		m_ConsoleTextArea        = nullptr;
		m_FatalErrorTextArea     = nullptr;
		m_InfoChannelCheckbox    = nullptr;
		m_WarningChannelCheckbox = nullptr;
		m_ErrorChannelCheckbox   = nullptr;
		m_ClipboardCopyButton    = nullptr;
		m_FileCopyButton         = nullptr;
		m_ClearButton            = nullptr;
		m_ExitButton             = nullptr;
		m_ConsoleBrush           = nullptr;
		m_FatalErrorBrush        = nullptr;
		m_Instance               = nullptr;

		memset( &m_History,          0, sizeof( m_History ) );
		memset( &m_HistoryType,      0, sizeof( m_HistoryType ) );
		memset( &m_Message,          0, sizeof( MSG ) );
		memset( &m_ConsoleFinalText, 0, 4096 * sizeof( char ) );
	}
		
	Console::~Console() {
		CONSOLE_PRINT_INFO( "Console::~Console\n" );

		ShowWindow( m_Window, SW_HIDE );
		CloseWindow( m_Window );

		UnregisterClass( lpszClassName, m_Instance );

		DestroyWindow( m_Window );
		DestroyWindow( m_ConsoleTextArea );
		DestroyWindow( m_FatalErrorTextArea );

		DestroyWindow( m_InfoChannelCheckbox );
		DestroyWindow( m_WarningChannelCheckbox );
		DestroyWindow( m_ErrorChannelCheckbox );

		DestroyWindow( m_ClipboardCopyButton );
		DestroyWindow( m_FileCopyButton );
		DestroyWindow( m_ClearButton );
		DestroyWindow( m_ExitButton );

		DeleteObject( m_ConsoleBrush );
		DeleteObject( m_FatalErrorBrush );

		DeleteObject( m_Instance );

		m_Window                 = nullptr;
		m_ConsoleTextArea        = nullptr;
		m_FatalErrorTextArea     = nullptr;

		m_InfoChannelCheckbox    = nullptr;
		m_WarningChannelCheckbox = nullptr;
		m_ErrorChannelCheckbox   = nullptr;

		m_ClipboardCopyButton    = nullptr;
		m_FileCopyButton         = nullptr;
		m_ClearButton            = nullptr;
		m_ExitButton             = nullptr;

		m_ConsoleBrush           = nullptr;
		m_FatalErrorBrush        = nullptr;
		m_Instance               = nullptr;

		m_Filter                 = 0;
		m_HistoryPointer         = 0;
			
		m_IsDestroyed            = false;
		m_BlinkingTime           = false;
		
		memset( &m_History,          0, sizeof( m_History ) );
		memset( &m_HistoryType,      0, sizeof( m_HistoryType ) );
		memset( &m_Message,          0, sizeof( MSG ) );
		memset( &m_ConsoleFinalText, 0, 4096 * sizeof( char ) );
	}

	/*====================================
	Console::Initialize
		Initialize the console
	====================================*/
	bool Console::Initialize() {
		m_Instance = GetBaseAddress();

		WNDCLASSEX windowClass    = {};
		windowClass.cbSize        = sizeof( WNDCLASSEX );
		windowClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		windowClass.lpfnWndProc   = Console::WndProc;
		windowClass.cbClsExtra    = NULL;
		windowClass.cbWndExtra    = NULL;
		windowClass.hInstance     = m_Instance;
		windowClass.hIcon         = LoadIcon( 0, IDI_APPLICATION );
		windowClass.hCursor       = LoadCursor( NULL, IDC_ARROW );
		windowClass.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
		windowClass.lpszMenuName  = NULL;
		windowClass.lpszClassName = lpszClassName;
		windowClass.hIconSm       = LoadIcon( 0, IDI_APPLICATION );

		if( !RegisterClassEx( &windowClass ) ) {
			return false;
		}

		m_Window = CreateWindowEx( WS_EX_CLIENTEDGE, lpszClassName, CONSOLE_NAME, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, 
								                     CW_USEDEFAULT, CW_USEDEFAULT, CONSOLE_WIDTH, CONSOLE_HEIGHT, NULL, NULL, m_Instance, NULL );

		if( !m_Window ) {
			return false;
		}

		m_ConsoleTextArea = CreateWindow( "edit", NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | ES_WANTRETURN 
								                            | ES_READONLY | ES_AUTOVSCROLL | WS_VSCROLL, 5, 5, CONSOLE_WIDTH - CONSOLE_MARGIN * 6,  CONSOLE_HEIGHT 
								                            - CONSOLE_HEIGHT / 4, m_Window,  NULL, (HINSTANCE)GetWindowLongPtr( m_Window, GWLP_HINSTANCE ), NULL );

		m_FatalErrorTextArea = CreateWindow( "static", NULL, WS_CHILD | WS_VISIBLE | SS_SUNKEN, 5, CONSOLE_HEIGHT - CONSOLE_HEIGHT / 4 + 5, 
								                               CONSOLE_WIDTH - CONSOLE_MARGIN * 6, 30, m_Window, NULL, (HINSTANCE)GetWindowLongPtr( m_Window, GWLP_HINSTANCE ), 
								                               NULL );

		m_InfoChannelCheckbox = CreateWindow( "button", "Info", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 5, CONSOLE_HEIGHT - 85, 75, 30, m_Window, NULL, 
								                                (HINSTANCE)GetWindowLongPtr( m_Window, GWLP_HINSTANCE ), NULL );

		m_WarningChannelCheckbox = CreateWindow( "button", "Warning", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 80,  CONSOLE_HEIGHT - 85, 100, 30, m_Window, 
								                                   NULL, (HINSTANCE)GetWindowLongPtr( m_Window, GWLP_HINSTANCE ), NULL );

		m_ErrorChannelCheckbox = CreateWindow( "button", "Error", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 180, CONSOLE_HEIGHT - 85, 75, 30, m_Window, NULL, 
								                                 (HINSTANCE)GetWindowLongPtr( m_Window, GWLP_HINSTANCE ), NULL );

		m_ClipboardCopyButton = CreateWindow( "button", NULL, BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 260, CONSOLE_HEIGHT - 85, 85, 30, m_Window, 
								                                (HMENU)CONSOLE_COMMAND_CLIPBOARD, (HINSTANCE)GetWindowLongPtr( m_Window, GWLP_HINSTANCE ), NULL );

		SendMessage( m_ClipboardCopyButton, WM_SETTEXT, 0, (LPARAM)"Copy" );

		m_FileCopyButton = CreateWindow( "button", NULL, BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 350, CONSOLE_HEIGHT - 85, 85, 30, m_Window, 
								                           (HMENU)CONSOLE_COMMAND_FILE, (HINSTANCE)GetWindowLongPtr( m_Window, GWLP_HINSTANCE ), NULL );

		SendMessage( m_FileCopyButton, WM_SETTEXT, 0, (LPARAM)"File" );

		m_ClearButton = CreateWindow( "button", NULL, BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 440, CONSOLE_HEIGHT - 85, 85, 30, m_Window, 
								                        (HMENU)CONSOLE_COMMAND_CLEAR, (HINSTANCE)GetWindowLongPtr( m_Window, GWLP_HINSTANCE ), NULL );

		SendMessage( m_ClearButton, WM_SETTEXT, 0, (LPARAM)"Clear" );

		m_ExitButton = CreateWindow( "button", NULL, BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 530, CONSOLE_HEIGHT - 85, 85, 30, m_Window,
								                      (HMENU)CONSOLE_COMMAND_EXIT, (HINSTANCE)GetWindowLongPtr( m_Window, GWLP_HINSTANCE ), NULL );

		SendMessage( m_ExitButton, WM_SETTEXT, 0, (LPARAM)"Exit" );
				
		const HDC hDC      = GetDC( m_ConsoleTextArea );
		const i32 fontSize = -MulDiv( 8, GetDeviceCaps( hDC, LOGPIXELSY ), 72 );

		const HFONT hFont = CreateFont( fontSize, 0, 0, 0, FW_LIGHT, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
								                          FF_MODERN | FIXED_PITCH, "Courier New" );

		SendMessage( m_ConsoleTextArea,    WM_SETFONT, (WPARAM)hFont, TRUE );
		SendMessage( m_FatalErrorTextArea, WM_SETFONT, (WPARAM)hFont, TRUE );

		ShowWindow( m_Window, SW_SHOWNORMAL );

		EnableMenuItem( GetSystemMenu( m_Window, FALSE ), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );

		SendMessage( m_InfoChannelCheckbox,    BM_SETCHECK, BST_CHECKED, 0 ); 
		SendMessage( m_WarningChannelCheckbox, BM_SETCHECK, BST_CHECKED, 0 );
		SendMessage( m_ErrorChannelCheckbox,   BM_SETCHECK, BST_CHECKED, 0 );

		SendMessage( m_ConsoleTextArea, EM_SETLIMITTEXT, -1, 0 );
				
		SetWindowPos( m_Window, HWND_TOP, 0, 0, CONSOLE_WIDTH, CONSOLE_HEIGHT, SWP_SHOWWINDOW );

		UpdateWindow( m_Window );

		return true;
	}

	/*====================================
	Console::Printf
		Print a text string to the console
			const console_message_channel_t channel : channel to print the text to
			const char* text, ... : text with C's printf formating
	====================================*/
	void Console::Printf( const console_message_channel_t channel, const char* text, ... ) {
		memset( m_ConsoleFinalText, 0, 4096 );

		va_list  argList = {};
		va_start( argList, text );
		const i32 textLength = vsnprintf_s( m_ConsoleFinalText, sizeof( m_ConsoleFinalText ), sizeof( m_ConsoleFinalText ) - 1, text, argList );
		va_end( argList );

		if( m_HistoryPointer >= CONSOLE_HISTORY_SIZE ) {
			m_HistoryPointer = 0;
		}

		m_History[m_HistoryPointer] = m_ConsoleFinalText;
		m_HistoryType[m_HistoryPointer++] = channel;

		if( IsChannelFiltered( channel ) ) {
			return;
		}

		if( GetWindowTextLength( m_ConsoleTextArea ) >= UINT32_MAX ) { 
			ClearConsole();
		}

		SendMessage( m_ConsoleTextArea, EM_SETSEL,     0, -1 );
		SendMessage( m_ConsoleTextArea, EM_SETSEL,    -1, -1 );
		SendMessage( m_ConsoleTextArea, EM_REPLACESEL, 0, (LPARAM)m_ConsoleFinalText );
	}

	/*====================================
	Console::OnFatalError
		Halt the console on a fatal error
			const char* function : function where the fatal error occured (use the __FUNCTION__ macro if possible)
		const char* errorText : error details
	====================================*/
	void Console::OnFatalError( const char* function, const char* errorText ) {
		char fatalErrorString[2048] = {};

		snprintf( fatalErrorString, 2048, "%s() : %s", function, errorText );

		SetWindowText( m_FatalErrorTextArea, fatalErrorString );
		SetActiveWindow( m_Window );

		while( !m_IsDestroyed ) {
			EventPump();
		}
	}

	/*====================================
	Console::ClearConsole
		Clear console text feed
	====================================*/
	void Console::ClearConsole() {
		SetWindowText( m_ConsoleTextArea, "" );

		m_HistoryPointer = 0;

		memset( &m_History,     0, sizeof( m_History ) );
		memset( &m_HistoryType, 0, sizeof( m_HistoryType ) );
	}

	/*====================================
	Console::CopyToFile
		Copy console text content to a log file
	====================================*/
	void Console::CopyToFile() {
		static const char* logFileName = "Console.log";

		const i32 textLength = GetWindowTextLength( m_ConsoleTextArea ) + 1;

		char* consoleContent = new char[textLength];

		GetWindowText( m_ConsoleTextArea, consoleContent, textLength );
		FText::Write( logFileName, consoleContent );

		delete[] consoleContent;
		delete[] logFileName;
	}

	/*====================================
	Console::OnFilterUpdate
		Update console according to its visibility settings
			const console_message_channel_t channel: channel modified
			const bool state : new visibilty
	====================================*/
	void Console::OnFilterUpdate( const console_message_channel_t channel, const bool state ) {
		const u8 binState = ( state ) ? 1 : 0; 
		i32 consolePointer = m_HistoryPointer;

		if ( channel == MESSAGE_CHANNEL_ERROR ) {
			m_Filter ^= ( -binState ^ m_Filter ) & ( 1 << 0 );
		} else if ( channel == MESSAGE_CHANNEL_WARNING ) {
			m_Filter ^= ( -binState ^ m_Filter ) & ( 1 << 1 );
		} else if ( channel == MESSAGE_CHANNEL_INFO ) {
			m_Filter ^= ( -binState ^ m_Filter ) & ( 1 << 2 );
		}

		SetWindowText( m_ConsoleTextArea, "" );

		RefreshConsole( consolePointer, CONSOLE_HISTORY_SIZE );

		consolePointer = 0;

		RefreshConsole( consolePointer, m_HistoryPointer );
	}

	/*====================================
	Console::RefreshConsole
		Update console text feed
			i32 &historyPointer : active history pointer
			i32 historyLimit : max history size
	====================================*/
	void Console::RefreshConsole( i32 &historyPointer, const i32 historyLimit ) {
		while( historyPointer < historyLimit ) {
			if( IsChannelFiltered( m_HistoryType[historyPointer] ) ) {
				historyPointer++;
				continue;
			}

			SendMessage( m_ConsoleTextArea, EM_SETSEL,     0, -1 );
			SendMessage( m_ConsoleTextArea, EM_SETSEL,    -1, -1 );
			SendMessage( m_ConsoleTextArea, EM_REPLACESEL, 0, (LPARAM)m_History[historyPointer].c_str() );

			historyPointer++;
		}
	}
	
	/*====================================
	Console::CopyToClipboard
		Copy console text content to OS clipboard
	====================================*/
	void Console::CopyToClipboard() {
			SendMessage( m_ConsoleTextArea, EM_SETSEL, 0, -1 );
			SendMessage( m_ConsoleTextArea, WM_COPY,   0,  0 );
	}
	
	/*====================================
	Console::EventPump
		Event Pump
	====================================*/
	void Console::EventPump() {
		while( PeekMessage( &m_Message, 0, 0, 0, PM_REMOVE ) ) {
			TranslateMessage( &m_Message );
			DispatchMessage( &m_Message );
		}
	}

	/*====================================
	Console::WndProc
		WINAPI Callback - should not be called manually
	====================================*/
	LRESULT CALLBACK Console::WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
		switch( msg ) {
			case WM_CLOSE:
				DestroyWindow( hwnd );
				Instance.Console->SetIsDestroyed( true );
				break; 

			case WM_DESTROY:
				PostQuitMessage( 0 );
				Instance.Console->SetIsDestroyed( true );
				break;

			case WM_COMMAND:
				if( wParam == CONSOLE_COMMAND_CLIPBOARD ) {
					Instance.Console->CopyToClipboard();
				} else if( wParam == CONSOLE_COMMAND_FILE ) {
					Instance.Console->CopyToFile();
				} else if( wParam == CONSOLE_COMMAND_CLEAR ) {
					Instance.Console->ClearConsole();
				} else if( wParam == CONSOLE_COMMAND_EXIT ) {
					Instance.Engine->SendShutdownSignal();
					Instance.Console->SetIsDestroyed( true );
				} else if( (HWND)lParam == Instance.Console->GetErrorChannelCheckbox() ) {
					CONSOLE_UPDATE_FILTER( MESSAGE_CHANNEL_ERROR );
				} else if( (HWND)lParam == Instance.Console->GetWarningChannelCheckbox() ) {
					CONSOLE_UPDATE_FILTER( MESSAGE_CHANNEL_WARNING );
				} else if( (HWND)lParam == Instance.Console->GetInfoChannelCheckbox() ) {
					CONSOLE_UPDATE_FILTER( MESSAGE_CHANNEL_INFO );
				}
				break;

			case WM_CTLCOLORSTATIC:
				if( (HWND)lParam == Instance.Console->GetConsoleTextArea() ) {
					SetTextColor( (HDC)wParam, RGB( 0xFF, 0xFF, 0xFF ) );
					SetBkColor( (HDC)wParam, RGB( 0x20, 0x20, 0x20 ) );
					return (INT_PTR)Instance.Console->GetConsoleBrush();
				} else if( (HWND)lParam == Instance.Console->GetFatalErrorTextArea() ) {
					if( Instance.Console->GetBlinkingTime() ) {
						SetTextColor( (HDC)wParam, RGB( 0xFF, 0x0, 0x0 ) );
					} else {
						SetTextColor( (HDC)wParam, RGB( 0xFF, 0xFF, 0xFF ) );
					}

					SetBkColor( (HDC)wParam, RGB( 0x20, 0x20, 0x20 ) );
					return (INT_PTR)Instance.Console->GetFatalErrorBrush();
				}
				break;

			case WM_CREATE:
				Instance.Console->SetConsoleBrush( CreateSolidBrush( RGB( 0x20, 0x20, 0x20 ) ) );
				Instance.Console->SetFatalErrorBrush( CreateSolidBrush( RGB( 0x20, 0x20, 0x20 ) ) );
				SetTimer( hwnd, 1, 1000, NULL );
				break;

			case WM_TIMER:
				if( wParam == 1 ) {
					Instance.Console->SetBlinkingTime( !Instance.Console->GetBlinkingTime() );
					InvalidateRect( Instance.Console->GetFatalErrorTextArea(), NULL, FALSE );
				}
				break;

			// instance pointer might be NULL at the initialization of the engine
			case WM_KILLFOCUS:
				if( Instance.MainRenderWindow ) ShowCursor( Instance.MainRenderWindow->GetHasFocus() );
				break;

			case WM_SETFOCUS:
				if( Instance.MainRenderWindow ) ShowCursor( TRUE );
				break;

			default:
				return DefWindowProc( hwnd, msg, wParam, lParam );
		}

		return 0;
	}
#endif
