#include "../Common.hpp"

#ifdef FLAG_DEBUG
#include "../i0rPrograms/Text.hpp"
#include "DevMenu.hpp"

DevMenu::DevMenu( const i32 x, const i32 y, std::string name ) {
	m_Name = name; 
	m_PosX = x; 
	m_PosY = y;
	m_ActiveItemIndex = -1;
	m_ActiveSubMenuIndex = -1;
	m_HasFocus = false;
	m_Parent = nullptr;
}

DevMenu::~DevMenu() {
	for( dev_menu_item_t* item : m_Items ) {
		SAFE_DELETE( item )
	}

	m_Items.clear();

	for( IDevMenu* subMenu : m_SubMenus ) {
		SAFE_DELETE( subMenu )
	}

	m_SubMenus.clear();

	m_PosX = 0;
	m_PosY = 0;
	m_ActiveItemIndex = 0;
	m_ActiveSubMenuIndex = 0;
	m_Name.clear();
}

void DevMenu::SetAsActiveMenu( const i32 menuIndex ) {
	m_HasFocus = false;
	m_ActiveSubMenuIndex = menuIndex;

	if( m_ActiveSubMenuIndex != -1 )
		m_SubMenus[m_ActiveSubMenuIndex]->SetHasFocus( true );
	else
		m_Parent->SetHasFocus( true );
}

DevMenu* DevMenu::CreateSubMenu( const char* name ) {
	if( m_ActiveItemIndex == -1 ) m_ActiveItemIndex = 0;

	DevMenu* submenu = new DevMenu( m_PosX, m_PosY, name );
	submenu->AddItem( new dev_menu_item_t(  "..",  std::bind( &DevMenu::SetAsActiveMenu, submenu, -1 ), DEV_MENU_ITEM_TYPE_MENU ) );
	submenu->SetParent( this );

	dev_menu_item_t* item = new dev_menu_item_t();
	item->Label = name;
	item->Action1 = std::bind( &DevMenu::SetAsActiveMenu, this, (i32)m_SubMenus.size() );
	item->Type = DEV_MENU_ITEM_TYPE_MENU;

	m_SubMenus.push_back( submenu );
	m_Items.push_back( item );

	return submenu;
}

void DevMenu::Compute() const  {
	if( m_HasFocus ) {
		PrintName();
	
		i32 subMenuIt = -1;

		for( dev_menu_item_t* item : m_Items ) {
			++subMenuIt;

			Instance.Renderer->GetTextProgram()->Compute(
				Instance.Renderer->GetTextProgram()->GetFontDebug(),
				m_PosX + 5,
				( m_PosY + 15 ) + ( subMenuIt * 15 ),
				( ( subMenuIt == m_ActiveItemIndex ) ) ? colorrgbaf( 1.0f, 0.0f, 0.0f, 1.0f ) : colorrgbaf( 1.0f, 1.0f, 0.0f, 1.0f ),
				colorrgbaf( 0.0f ),
				1.0f,
				item->Label.c_str()
			);
		}
	} else if( m_ActiveSubMenuIndex != -1 ) {
		m_SubMenus[m_ActiveSubMenuIndex]->Compute();
	}
}

void DevMenu::PrintName( bool enableHighlight ) const  {
	Instance.Renderer->GetTextProgram()->Compute(
		Instance.Renderer->GetTextProgram()->GetFontDebug(),
		m_PosX,
		m_PosY,
		( enableHighlight ) ? colorrgbaf( 1.0f, 0.0f, 0.0f, 1.0f ) : colorrgbaf( 1.0f, 1.0f, 0.0f, 1.0f ),
		colorrgbaf( 0.0f ),
		1.0f,
		m_Name.c_str()
	);
}

void DevMenu::MoveIndex( const i32 whereTo ) {
	if( m_HasFocus ) {
		m_ActiveItemIndex += whereTo;

		if( m_ActiveItemIndex < 0 ) {
			m_ActiveItemIndex = (i32)m_Items.size() - 1;
		} else if( m_ActiveItemIndex >= m_Items.size() ) {
			m_ActiveItemIndex = 0;
		}
	} else if( m_ActiveSubMenuIndex != -1 ) {
		m_SubMenus[m_ActiveSubMenuIndex]->MoveIndex( whereTo );
	}
}

void DevMenu::OnEntrySelection() {
	if( m_HasFocus ) {
		if( m_ActiveItemIndex != -1 ) {
			const dev_menu_item_t* curItem = m_Items[m_ActiveItemIndex];
			if( curItem->Type != DEV_MENU_ITEM_TYPE_MENU ) return;
			curItem->Action1();
		}
	} else if( m_ActiveSubMenuIndex != -1 ) {
		m_SubMenus[m_ActiveSubMenuIndex]->OnEntrySelection();
	}
}

void DevMenu::OnEntryUpdate( const bool incUpdate ) {
	if( m_HasFocus ) {
		if( m_ActiveItemIndex != -1 ) {
			const dev_menu_item_t* curItem = m_Items[m_ActiveItemIndex];
			if( curItem->Type != DEV_MENU_ITEM_TYPE_VARIABLE ) return;

			( incUpdate ) ? curItem->Action1() : curItem->Action2();
		}
	} else if( m_ActiveSubMenuIndex != -1 ) {
		m_SubMenus[m_ActiveSubMenuIndex]->OnEntryUpdate( incUpdate );
	}
}

IDevMenu* DevMenu::FindMenuFromString( const std::string path ) {
	IDevMenu* curMenu = this;

	if( path != "" ) {
		std::vector<std::string> subPath = SplitString( path, '\\' );

		for( std::string piece : subPath ) {
			for( IDevMenu* subMenu : curMenu->GetSubMenus() ) {
				if( subMenu->GetName() == piece ) {
					curMenu = subMenu;
					break;
				}
			}
		}
	}

	return curMenu;
}

void DevMenu::RegisterBoolean( bool* value, std::string name, std::string path, callback_t updateRoutine ) {
	IDevMenu* curMenu = FindMenuFromString( path );

	dev_menu_item_t* i = new dev_menu_item_t();
	i->Label = name + " : " + ( ( *value ) ? "true" : "false" );
	i->Type = DEV_MENU_ITEM_TYPE_VARIABLE;

	callback_t actionFunc = std::bind( []( bool* v, dev_menu_item_t* i, std::string n, callback_t r ) {
		*v = !*v;
		i->Label = n + " : " + ( ( *v ) ? "true" : "false" );
		if( r ) r();
	}, value, i, name, updateRoutine );

	i->Action1 = actionFunc;
	i->Action2 = actionFunc;

	curMenu->AddItem( i );
}

void DevMenu::RegisterLabelOverlay( const char* label, const i32 x, const i32 y, std::string path ) {
	IDevMenu* curMenu = FindMenuFromString( path );

	dev_menu_item_t* i = new dev_menu_item_t();
	i->Label = label;
	i->Type = DEV_MENU_ITEM_TYPE_STRING;

	curMenu->AddItem( i );
}

void DevMenu::RegisterLabel( const char* label, std::string path ) {
	IDevMenu* curMenu = FindMenuFromString( path );

	dev_menu_item_t* i = new dev_menu_item_t();
	i->Label = label;
	i->Type = DEV_MENU_ITEM_TYPE_STRING;

	curMenu->AddItem( i );
}

void DevMenu::RegisterFloat( f32* value, f32 step, f32 min, f32 max, std::string name, std::string path, callback_t updateRoutine ) {
	IDevMenu* curMenu = FindMenuFromString( path );

	dev_menu_item_t* i = new dev_menu_item_t();
	i->Label = name + " : " + std::to_string( *value );
	i->Type = DEV_MENU_ITEM_TYPE_VARIABLE;

	i->Action1 = std::bind( []( f32* v, f32 s, f32 m, dev_menu_item_t* i, std::string n, callback_t r ) {
		*v += s;
		if( *v > m ) *v = m;
		i->Label = n + " : " + std::to_string( *v );

		if( r ) r();
	}, value, step, max, i, name, updateRoutine );

	i->Action2 = std::bind( []( f32* v, f32 s, f32 m, dev_menu_item_t* i, std::string n, callback_t r ) {
		*v -= s;
		if( *v < m ) *v = m;
		i->Label = n + " : " + std::to_string( *v );

		if( r ) r();
	}, value, step, min, i, name, updateRoutine );

	curMenu->AddItem( i );
}

void DevMenu::RegisterInteger( i32* value, i32 step, i32 min, i32 max, std::string name, std::string path, callback_t updateRoutine ) {
	IDevMenu* curMenu = FindMenuFromString( path );

	dev_menu_item_t* i = new dev_menu_item_t();
	i->Label = name + " : " + std::to_string( *value );
	i->Type = DEV_MENU_ITEM_TYPE_VARIABLE;

	i->Action1 = std::bind( []( i32* v, i32 s, i32 m, dev_menu_item_t* i, std::string n, callback_t r ) {
		*v += s;
		if( *v > m ) *v = m;
		i->Label = n + " : " + std::to_string( *v );

		if( r ) r();
	}, value, step, max, i, name, updateRoutine );

	i->Action2 = std::bind( []( i32* v, i32 s, i32 m, dev_menu_item_t* i, std::string n, callback_t r ) {
		*v -= s;
		if( *v < m ) *v = m;
		i->Label = n + " : " + std::to_string( *v );

		if( r ) r();
	}, value, step, min, i, name, updateRoutine );

	curMenu->AddItem( i );
}
#endif