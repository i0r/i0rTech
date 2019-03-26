#include "Common.hpp"
#include <i0rCore/IEngine.hpp>
#include <i0rCore/IInputHandler.hpp>
#include "RenderWindow.hpp"

RenderWindow::RenderWindow( const u32 width, const u32 height, const char* caption, 
								                    const window_display_mode_t dispMode ) {
	m_Width            = width;
	m_Height           = height;
	m_Caption          = caption;
	m_DisplayMode      = dispMode;
	m_Time             = GetClientTimeAsMis();
	m_HasFocus         = true;
	
	#ifdef FLAG_WINDOWS
		m_RenderingContext = nullptr;
		memset( &m_Message, 0, sizeof( MSG ) );
	#endif
}

RenderWindow::~RenderWindow() {
	ShowWindow( m_Window, SW_HIDE );
	CloseWindow( m_Window );

	if( m_DisplayMode != WINDOW_DISPLAY_MODE_WINDOWED ) {
		ShowCursor( TRUE );
	}

	UnregisterClass( winClassName, m_WindowInstance );
	m_WindowInstance = nullptr;

	m_DrawContext    = NULL;
	m_Window         = NULL;
	m_DisplayMode    = WINDOW_DISPLAY_MODE_NONE;

	m_Caption.clear();
}

bool RenderWindow::Initialize() {
	#ifdef FLAG_WINDOWS
		m_WindowInstance = GetModuleHandle( NULL );

		WNDCLASSEX windowClass = {};
		windowClass.cbSize          = sizeof( WNDCLASSEX );
		windowClass.style           = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		windowClass.lpfnWndProc     = WndProc;
		windowClass.cbClsExtra      = NULL;
		windowClass.cbWndExtra      = NULL;
		windowClass.hInstance       = m_WindowInstance;
		windowClass.hIcon           = LoadIcon( 0, IDI_APPLICATION );
		windowClass.hCursor         = LoadCursor( 0, IDC_ARROW );
		windowClass.hbrBackground   = (HBRUSH)( COLOR_WINDOW + 1 );
		windowClass.lpszMenuName    = NULL;
		windowClass.lpszClassName   = winClassName;
		windowClass.hIcon           = LoadIcon( 0, IDI_APPLICATION );

		if( !RegisterClassEx( &windowClass ) ) {
			return false;
		}

		DWORD windowExFlags = WS_EX_APPWINDOW, 
								windowFlags   = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		RECT windowRect = {};

		if( m_DisplayMode == WINDOW_DISPLAY_MODE_FULLSCREEN || m_DisplayMode == WINDOW_DISPLAY_MODE_FULLSCREEN_BORDERLESS ) {
			DEVMODE devMode = {};
			memset( &devMode, 0, sizeof( devMode ) );

			devMode.dmSize       = sizeof( devMode );
			devMode.dmPelsWidth  = m_Width;
			devMode.dmPelsHeight = m_Height;
			devMode.dmBitsPerPel = 32;
			devMode.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			i32 screenWidth = GetSystemMetrics( SM_CXSCREEN ),
								   screenHeight = GetSystemMetrics( SM_CYSCREEN );

			if( m_DisplayMode == WINDOW_DISPLAY_MODE_FULLSCREEN_BORDERLESS ) {
				m_Width = screenWidth;
				m_Height = screenHeight;
			} else if( m_Width != screenWidth && m_Height != screenHeight ) {
				if( ChangeDisplaySettings( &devMode, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL ) {
					return false;
				}
			}

			windowFlags   |= WS_POPUP;
		} else {
			windowExFlags |= WS_EX_WINDOWEDGE;
			windowFlags   |= WS_CLIENT_FRAME_WINDOWED;
		}

		ShowCursor( FALSE );

		m_Window = CreateWindowEx( windowExFlags, winClassName, m_Caption.c_str(), windowFlags, CW_USEDEFAULT, CW_USEDEFAULT, 
								                     m_Width, m_Height, NULL, NULL, m_WindowInstance, NULL );

		if( !m_Window ) {
			return false;
		}

		m_DrawContext = GetDC( m_Window );
		if( !m_DrawContext ) {
			return false;
		}

		ShowWindow( m_Window, SW_SHOWNORMAL );

		#if defined( FLAG_DEBUG ) // better console / client frame placement
			if( Instance.Configuration->Display.WindowDisplayMode == WINDOW_DISPLAY_MODE_WINDOWED )
				SetWindowPos( m_Window, HWND_TOP, 640, 0, m_Width, m_Height, SWP_SHOWWINDOW );
		#endif

		GetWindowRect( m_Window, &windowRect );

		ClipCursor( &windowRect );
		UpdateWindow( m_Window );

		SetForegroundWindow( m_Window );
		SetFocus( m_Window );
	#endif
	return true;
}

void RenderWindow::SetCursorMiddleScreen() {
	#ifdef FLAG_WINDOWS
		POINT screenPoint = {
			( LONG )m_Width / 2,
			( LONG )m_Height / 2,
		};

		ClientToScreen( m_Window, &screenPoint );
		SetCursorPos( screenPoint.x, screenPoint.y );
	#endif
}

u64 RenderWindow::GetElapsedTime() {
	return GetClientTimeAsMis() - m_Time;
}

void RenderWindow::EventPump() {
	#ifdef FLAG_WINDOWS
		m_Time = GetTickCount64();

		while( PeekMessage( &m_Message, 0, 0, 0, PM_REMOVE ) ) {
			TranslateMessage( &m_Message );
			DispatchMessage( &m_Message );
		}
	#endif
}

#ifdef FLAG_WINDOWS
LRESULT CALLBACK RenderWindow::WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	switch( msg ) {
		case WM_CLOSE:
		case WM_DESTROY:
			Instance.Engine->SendShutdownSignal();
			PostQuitMessage( 0 );
			break;

		case WM_SIZE:
			// ignore the minimize message, otherwise this will lead to a crash...
			if( wParam != SIZE_MINIMIZED ) {
				Instance.Renderer->OnResize( &hwnd );
			}
			break;

		case WM_KILLFOCUS:
			if( Instance.MainRenderWindow ) {
				Instance.MainRenderWindow->SetHasFocus( false );
				ShowCursor( TRUE );
			}
			break;

		case WM_SETFOCUS:
			if( Instance.MainRenderWindow ) {
				Instance.MainRenderWindow->SetHasFocus( true );
				ShowCursor( FALSE );

				RECT rRect = {};
				GetClientRect( hwnd, &rRect );
				ClipCursor( &rRect );
			}
			break;

		case WM_INPUT:
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYUP:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEWHEEL:
			Instance.InputHandler->OnUpdate( msg, wParam, lParam, (void*)hwnd );
			break;

		case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lpMinMaxInfo = (LPMINMAXINFO)lParam;
			lpMinMaxInfo->ptMinTrackSize.x = 256;
			lpMinMaxInfo->ptMinTrackSize.y = 256;
		}
		break;

		default:
			return DefWindowProc( hwnd, msg, wParam, lParam );
	}

	return 0;
}
#endif
