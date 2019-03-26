#pragma once

#include "Keys.hpp"
#include "Mouse.hpp"

struct key_action_t {
	i32        ActionState; // 0 - Default; 1 - Once Per Frame (NOT FIRED YET); 2 - OPF (FIRED)
	callback_t Callback;
};

class IInputHandler {
	DECLARE_MEMBER( public, device_mouse_t, Mouse )

	public:
		virtual               ~IInputHandler() {}
		virtual bool          Initialize()                                                        = 0;
			
		#if defined( FLAG_WINDOWS )
			virtual void OnUpdate( const u32 message, const u64 virtualKeyCode, const i64 extraInformations, 
								                  const void* messageSender = nullptr )                             = 0;
		#endif
			
		virtual void Poll()                                                                       = 0;
		virtual void ReadMapping()                                                                = 0;
		virtual void UpdateRelativeMouse()                                                        = 0;
		virtual void UpdateMouse()                                                                = 0;
		virtual void RegisterCallback( const u32 callback, const u32 keyCombinaison )             = 0;
		virtual void UpdateAction( const u32 actionHashcode, const key_action_t keyAction )       = 0;
		virtual void ResetOncePerSecKeys()                                                      = 0;

		bool m_Keyboard[INPUT_HANDLER_KEYBOARD_KEY_COUNT];
};
