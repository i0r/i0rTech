#pragma once

#include "DevMenu.hpp"

#ifdef FLAG_DEBUG
	class DebugMenu {
		public:
			static DevMenu* Create() {
				DevMenu* debugMenu = new DevMenu( 5, 150, "i0rTech - Debug Menu");

				DevMenu* worldMenu = debugMenu->CreateSubMenu( "World" );
				DevMenu* skyMenu = worldMenu->CreateSubMenu( "Skydome" );

				DevMenu* renderMenu = debugMenu->CreateSubMenu( "Rendering" );
				DevMenu* postfxMenu = renderMenu->CreateSubMenu( "Post Processing" );
				
				DevMenu* sysMenu = debugMenu->CreateSubMenu( "System" );
				DevMenu* pageMenu = sysMenu->CreateSubMenu( "Memory Paging" );

				DevMenu* gameplayMenu = debugMenu->CreateSubMenu( "Gameplay" );
				DevMenu* audioMenu = debugMenu->CreateSubMenu( "Audio" );
				DevMenu* streamMenu = debugMenu->CreateSubMenu( "Streaming" );
				DevMenu* cheatMenu = debugMenu->CreateSubMenu( "Cheats" );

				// update input callbacks 
				Instance.InputHandler->UpdateAction(
					HashStringCRC32( "DEBUG_ACTION_DISPLAY_MENU" ),
					{ 1, std::bind( &IRenderer::SetDebugMenuState, Instance.Renderer ) }
				);

				Instance.InputHandler->UpdateAction(
					HashStringCRC32( "DEBUG_ACTION_MENU_UP" ),
					{ 1, std::bind( &IDevMenu::MoveIndex, debugMenu, -1 ) }
				);

				Instance.InputHandler->UpdateAction(
					HashStringCRC32( "DEBUG_ACTION_MENU_DOWN" ),
					{ 1, std::bind( &IDevMenu::MoveIndex, debugMenu, 1 ) }
				);

				Instance.InputHandler->UpdateAction(
					HashStringCRC32( "DEBUG_ACTION_MENU_USE" ),
					{ 1, std::bind( &IDevMenu::OnEntrySelection, debugMenu ) }
				);

				Instance.InputHandler->UpdateAction(
					HashStringCRC32( "DEBUG_ACTION_MENU_INC" ),
					{ 1, std::bind( &IDevMenu::OnEntryUpdate, debugMenu, true ) }
				);

				Instance.InputHandler->UpdateAction(
					HashStringCRC32( "DEBUG_ACTION_MENU_DEC" ),
					{ 1, std::bind( &IDevMenu::OnEntryUpdate, debugMenu, false ) }
				);

				return debugMenu;
			}
	};
#endif
