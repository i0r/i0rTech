#pragma once

#include <i0rCore/IInputHandler.hpp>

struct device_mouse_t;

#define INPUT_DECLARE_ACTION( x ) m_ActionList[HashStringCRC32( x )] = { 0, nullptr }

class InputHandler : public IInputHandler {
	public:
		explicit InputHandler() ;
		~InputHandler() ;
			
		bool Initialize() ;
			
		#if defined( FLAG_WINDOWS )
			void OnUpdate( const u32 message, const u64 virtualKeyCode, const i64 extraInformations, const void* messageSender = nullptr ) ;
		#endif
			
		void Poll() ;
		void ReadMapping() ;

		void UpdateRelativeMouse() ;
		void UpdateMouse() ;

		void ResetOncePerSecKeys() ;

		INLINE void RegisterCallback( const u32 callback, const u32 keyCombinaison ) {
			m_Callback.insert( std::make_pair( keyCombinaison, callback ) );
		}
			
		INLINE void UpdateAction( const u32 actionHashcode, const key_action_t keyAction ) {
			m_ActionList[actionHashcode] = keyAction;
		}

	private:
		#if defined( FLAG_WINDOWS )
			void EnableRawInput() ;
			void ReadRawInput( const u64 extraInformations ) ;
		#endif
		
		std::map<u32, key_action_t> m_ActionList;
		std::map<u32, u32>          m_Callback;
};
