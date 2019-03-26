#pragma once

enum console_message_channel_t {
	MESSAGE_CHANNEL_ALL,
	MESSAGE_CHANNEL_INFO,
	MESSAGE_CHANNEL_WARNING,
	MESSAGE_CHANNEL_ERROR
};

class IConsole {
	#ifdef FLAG_WINDOWS
		DECLARE_MEMBER( protected, HWND,      Window )
		DECLARE_MEMBER( protected, HINSTANCE, Instance )
		DECLARE_MEMBER( protected, HWND,      ConsoleTextArea )
		DECLARE_MEMBER( protected, HWND,      FatalErrorTextArea )
		DECLARE_MEMBER( protected, HWND,      InfoChannelCheckbox )
		DECLARE_MEMBER( protected, HWND,      WarningChannelCheckbox )
		DECLARE_MEMBER( protected, HWND,      ErrorChannelCheckbox )
		DECLARE_MEMBER( protected, HWND,      ClipboardCopyButton )
		DECLARE_MEMBER( protected, HWND,      FileCopyButton )
		DECLARE_MEMBER( protected, HWND,      ClearButton )
		DECLARE_MEMBER( protected, HWND,      ExitButton )
		DECLARE_MEMBER( protected, HBRUSH,    ConsoleBrush )
		DECLARE_MEMBER( protected, HBRUSH,    FatalErrorBrush )
		DECLARE_MEMBER( protected, MSG,       Message )
	#endif

	DECLARE_MEMBER( protected, u8,         Filter )
	DECLARE_MEMBER( protected, u16,        HistoryPointer )
	DECLARE_MEMBER( protected, bool,       IsDestroyed )
	DECLARE_MEMBER( protected, bool,       BlinkingTime )

	public:
		virtual ~IConsole() {}

		virtual bool Initialize()                                                                 = 0;
		virtual void Printf( const console_message_channel_t channel, const char* text, ... )     = 0;
		virtual void EventPump()                                                                  = 0;
		virtual void OnFatalError( const char* function, const char* errorText )                  = 0;
		virtual void CopyToClipboard()                                                            = 0;
		virtual void CopyToFile()                                                                 = 0;
		virtual void ClearConsole()                                                               = 0;
		virtual void OnFilterUpdate( const console_message_channel_t channel, const bool state )  = 0;

	protected:
		INLINE bool IsChannelFiltered( const console_message_channel_t c ) const  {
			return c == MESSAGE_CHANNEL_ERROR   && !( ( m_Filter >> 0 ) & 1ll )
							|| c == MESSAGE_CHANNEL_WARNING && !( ( m_Filter >> 1 ) & 1ll ) 
							|| c == MESSAGE_CHANNEL_INFO    && !( ( m_Filter >> 2 ) & 1ll );
		}
};
