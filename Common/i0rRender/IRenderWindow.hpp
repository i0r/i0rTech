#pragma once

enum window_display_mode_t {
	WINDOW_DISPLAY_MODE_NONE                  = 0,
	WINDOW_DISPLAY_MODE_WINDOWED              = 1,
	WINDOW_DISPLAY_MODE_FULLSCREEN            = 2,
	WINDOW_DISPLAY_MODE_FULLSCREEN_BORDERLESS = 3,
};

class IRenderWindow {
	DECLARE_MEMBER( protected,  bool,                  HasFocus )
	
	#ifdef FLAG_WINDOWS
		DECLARE_MEMBER( protected, HWND,                  Window )
		DECLARE_MEMBER( protected, HINSTANCE,             WindowInstance )
		DECLARE_MEMBER( protected, HDC,                   DrawContext )
		DECLARE_MEMBER( protected, HGLRC,                 RenderingContext ) 
		DECLARE_MEMBER( protected, MSG,                   Message )
	#endif

	DECLARE_MEMBER( protected,  u32,                   Width )
	DECLARE_MEMBER( protected,  u32,                   Height )
	DECLARE_MEMBER( protected,  u64,                   Time )
	DECLARE_MEMBER( protected,  window_display_mode_t, DisplayMode )
	DECLARE_MEMBER( protected,  std::string,           Caption )

	public:
		virtual ~IRenderWindow() {}
		virtual bool Initialize()             = 0;
		virtual void SetCursorMiddleScreen()  = 0;
		virtual u64  GetElapsedTime()         = 0;
		virtual void EventPump()              = 0;
};
