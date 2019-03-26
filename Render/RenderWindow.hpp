#pragma once

#include <i0rRender/IRenderWindow.hpp>

#ifdef FLAG_WINDOWS
	#define WS_CLIENT_FRAME_WINDOWED ( WS_OVERLAPPEDWINDOW | WS_BORDER | WS_VISIBLE )
	static const LPCSTR winClassName = "i0rTechFrame";
#endif

class RenderWindow : public IRenderWindow {
	public:
		explicit RenderWindow( const u32 width, const u32 height, const char* caption, 
								                 const window_display_mode_t dispMode ) ;
		~RenderWindow() ;

		bool Initialize() ;
		void SetCursorMiddleScreen() ;
		u64  GetElapsedTime() ;
		void EventPump() ;

	private:
		#ifdef FLAG_WINDOWS
			static LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
		#endif
};
