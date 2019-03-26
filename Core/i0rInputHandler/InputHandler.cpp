#include "../Common.hpp"
#include <i0rCore/IEngine.hpp>
#include <i0rRender/IRenderer.hpp>
#include <i0rRender/IRenderWindow.hpp>
#include <i0rGame/IGameLogic.hpp>
#include <i0rDebug/IDevMenu.hpp>
#include "InputHandler.hpp"

InputHandler::InputHandler() {
	memset( &m_Keyboard, 0, INPUT_HANDLER_KEYBOARD_KEY_COUNT * sizeof( bool ) );
	memset( &m_Mouse, 0, sizeof( m_Mouse ) );
}

InputHandler::~InputHandler() {
	memset( &m_Keyboard, 0, INPUT_HANDLER_KEYBOARD_KEY_COUNT * sizeof( bool ) );
	memset( &m_Mouse, 0, sizeof( m_Mouse ) );

	m_ActionList.clear();
	m_Callback.clear();
}

void InputHandler::ReadMapping() {
	std::map<u32, key_action_t>::const_iterator iterator = {};

	for( std::pair<u32, u32> entry : Instance.Configuration->Input.KeyMapping ) {
		// listen to an action only if a keymap has been done
		iterator = m_ActionList.find( entry.first );
		if( iterator != m_ActionList.end() ) {
			m_Callback.insert( std::make_pair( entry.second, entry.first ) );
		} else {
			CONSOLE_PRINT_WARNING( "InputHandler::ReadMapping => Unknown action 0x%X\n", entry.first );
		}
	}
}

void InputHandler::ResetOncePerSecKeys() {
	std::map<u32, key_action_t>::iterator it = m_ActionList.begin();

	for( ; it != m_ActionList.end(); it++ ) {
		if( it->second.ActionState != 0 ) it->second.ActionState = 1;
	}
}

bool InputHandler::Initialize() {
	#if defined( FLAG_WINDOWS )
		EnableRawInput();
	#endif

	// register engine-specific actions
	INPUT_DECLARE_ACTION( "ENGINE_ACTION_SHUTDOWN" );

	INPUT_DECLARE_ACTION( "DEBUG_ACTION_DISPLAY_MENU" );
	INPUT_DECLARE_ACTION( "DEBUG_ACTION_MENU_UP" );
	INPUT_DECLARE_ACTION( "DEBUG_ACTION_MENU_DOWN" );
	INPUT_DECLARE_ACTION( "DEBUG_ACTION_MENU_USE" );
	INPUT_DECLARE_ACTION( "DEBUG_ACTION_MENU_INC" );
	INPUT_DECLARE_ACTION( "DEBUG_ACTION_MENU_DEC" );

	INPUT_DECLARE_ACTION( "ACTOR_ACTION_MOVE_RIGHT" );
	INPUT_DECLARE_ACTION( "ACTOR_ACTION_MOVE_LEFT" );
	INPUT_DECLARE_ACTION( "ACTOR_ACTION_MOVE_FORWARD" );
	INPUT_DECLARE_ACTION( "ACTOR_ACTION_MOVE_BACKWARD" );

	INPUT_DECLARE_ACTION( "GAME_CHANGE_MODE" );
	
	m_ActionList[HashStringCRC32( "ENGINE_ACTION_SHUTDOWN" )] = { 1, std::bind( &IEngine::SendShutdownSignal, 
								                                                                     Instance.Engine ) };

	ReadMapping();

	return true;
}
			
void InputHandler::Poll() {
	if( m_Mouse.RelativeMouse && Instance.MainRenderWindow->GetHasFocus() 
		&& Instance.GameLogic->GetActiveGameMode() == GAME_LOGIC_MODE_PLAYING ) {
		Instance.MainRenderWindow->SetCursorMiddleScreen();
	}
	
	for( std::pair<u32, u32> keyBinding : m_Callback ) {
		// check if a modifier + key or key alone combinaison
		if( ( ( !( keyBinding.first & 0xFF ) && m_Keyboard[( keyBinding.first >> 16 ) & 0xFF] ) 
			|| ( m_Keyboard[( keyBinding.first >> 16 ) & 0xFF] && m_Keyboard[keyBinding.first & 0xFF] ) ) ) {
			if( m_ActionList[keyBinding.second].Callback && m_ActionList[keyBinding.second].ActionState < 2 ) {
				m_ActionList[keyBinding.second].Callback();

				if( m_ActionList[keyBinding.second].ActionState == 1 ) m_ActionList[keyBinding.second].ActionState++;
			}
		}
	}
}

#if defined( FLAG_WINDOWS )
	void InputHandler::EnableRawInput() {
		RAWINPUTDEVICE rawInputDevice = { 0x1, 0x2, 0x0, 0x0 };
		RegisterRawInputDevices( &rawInputDevice, 1, sizeof( RAWINPUTDEVICE ) );
	}

	void InputHandler::UpdateRelativeMouse() {
		m_Mouse.PreviousX = Instance.Configuration->Display.WindowWidth / 2;
		m_Mouse.PreviousY = Instance.Configuration->Display.WindowHeight / 2;

		m_Mouse.PositionX = 0;
		m_Mouse.PositionY = 0;
	}

	void InputHandler::UpdateMouse() {
		m_Mouse.PreviousWheelX = m_Mouse.WheelX;
		m_Mouse.PreviousWheelY = m_Mouse.WheelY;

		m_Mouse.WheelX = 0;
		m_Mouse.WheelY = 0;
	}

	void InputHandler::ReadRawInput( const u64 extraInformations ) {
		RAWINPUT rawInput = {};
		UINT szData       = sizeof( RAWINPUT );

		GetRawInputData( ( HRAWINPUT )extraInformations, RID_INPUT, &rawInput, &szData, sizeof( RAWINPUTHEADER ) );

		if( rawInput.header.dwType == RIM_TYPEMOUSE ) {
			m_Mouse.PositionX += rawInput.data.mouse.lLastX;
			m_Mouse.PositionY += rawInput.data.mouse.lLastY;
		}
	
		if( !m_Mouse.RelativeMouse ) {
			/*Clamp( m_Mouse.PositionX, 0, Instance.Configuration->Display.WindowWidth );
			Clamp( m_Mouse.PositionY, -Instance.Configuration->Display.WindowHeight, 0 );*/
		}
	}

		void InputHandler::OnUpdate( const u32 message, const u64 virtualKeyCode, const i64 extraInformations, 
								                       const void* messageSender ) {
			switch( message ) {
				case WM_SYSKEYDOWN:
				case WM_KEYDOWN:
					m_Keyboard[WindowsKeys[virtualKeyCode]] = true;
					break;

				case WM_SYSKEYUP:
				case WM_KEYUP:
					m_Keyboard[WindowsKeys[virtualKeyCode]] = false;
					break;

				case WM_LBUTTONDOWN:
					m_Mouse.LeftButton = true;
					break;

				case WM_LBUTTONUP:
					m_Mouse.LeftButton = false;
					break;
	
				case WM_RBUTTONDOWN:
					m_Mouse.RightButton = true;
					break;

				case WM_RBUTTONUP:
					m_Mouse.RightButton = false;
					break;
				
				case WM_MBUTTONDOWN:
					m_Mouse.MiddleButton = true;
					break;

				case WM_MBUTTONUP:
					m_Mouse.MiddleButton = false;
					break;
				
				case WM_XBUTTONDOWN:
					switch( ( virtualKeyCode >> ( 8 * 2 ) ) & 0xff ) {
						case 0x01:
							m_Mouse.Extra1Button = true;
							break;

						case 0x02:
							m_Mouse.Extra2Button = true;
							break;
					}   
				break;

				case WM_XBUTTONUP:
					switch( ( virtualKeyCode >> ( 8 * 2 ) ) & 0xff ) {
						case 0x01:
							m_Mouse.Extra1Button = false;
							break;

						case 0x02:
							m_Mouse.Extra2Button = false;
							break;
					}
				break;

				case WM_INPUT:
					ReadRawInput( extraInformations );
					break;

				case WM_MOUSEWHEEL:
					m_Mouse.PreviousWheelX = m_Mouse.WheelX;
					m_Mouse.PreviousWheelY = m_Mouse.WheelY;

					m_Mouse.WheelX = ( ( i32 )( i16 )LOWORD( extraInformations ) ) / GET_WHEEL_DELTA_WPARAM( virtualKeyCode );
					m_Mouse.WheelY = ( ( i32 )( i16 )HIWORD( extraInformations ) ) / GET_WHEEL_DELTA_WPARAM( virtualKeyCode );
					break;

				default:
					break;
			};
		}
	#endif
