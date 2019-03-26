#pragma once

#include <i0rDebug/IDevMenu.hpp>

class DevMenu : public IDevMenu {
	public:
		explicit DevMenu( const i32 x = 0, const i32 y = 0, std::string name = "Menu" ) ;
		~DevMenu() ;

		void Compute() const ;
		void PrintName( bool enableHighlight = false ) const ;

		DevMenu* CreateSubMenu( const char* name ) ;

		void MoveIndex( const i32 whereTo ) ;

		void OnEntrySelection() ;
		void OnEntryUpdate( const bool incUpdate ) ;

		void RegisterLabelOverlay( const char* label, const i32 x, const i32 y, std::string path ) ;
		void RegisterLabel( const char* label, std::string path ) ;
		void RegisterFloat( f32* value, f32 step, f32 min, f32 max, std::string name, std::string path = "", callback_t updateRoutine = nullptr ) ;
		void RegisterBoolean( bool* value, std::string name, std::string path = "", callback_t updateRoutine = nullptr ) ;
		void RegisterInteger( i32* value, i32 step, i32 min, i32 max, std::string name, std::string path = "", callback_t updateRoutine = nullptr ) ;

		private:
			void SetAsActiveMenu( const i32 menuIndex ) ;
			IDevMenu* FindMenuFromString( const std::string path ) ;
};
