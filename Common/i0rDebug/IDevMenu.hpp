#pragma once

enum dev_menu_item_type_t {
	DEV_MENU_ITEM_TYPE_UNKNOWN,
	DEV_MENU_ITEM_TYPE_EMPTY,
	DEV_MENU_ITEM_TYPE_SEPARATOR,
	DEV_MENU_ITEM_TYPE_MENU,
	DEV_MENU_ITEM_TYPE_STRING,
	DEV_MENU_ITEM_TYPE_VARIABLE,
};

struct dev_menu_item_t {
	explicit dev_menu_item_t( const std::string l, const callback_t a )  : Label( l ),
								                                                                        Type( DEV_MENU_ITEM_TYPE_UNKNOWN ),
								                                                                        Action1( a ),
								                                                                        Action2( nullptr ),
								                                                                        Action3( nullptr ) {
	}

	explicit dev_menu_item_t( const std::string l, const callback_t a, const dev_menu_item_type_t t )  
		: Label( l ), 
				Type( t ),
				Action1( a ),
				Action2( nullptr ),
				Action3( nullptr ) {

	}

	explicit dev_menu_item_t( const std::string l, const callback_t a, const callback_t b )  
		: Label( l ), 
				Type( DEV_MENU_ITEM_TYPE_UNKNOWN ),
				Action1( a ), 
				Action2( b ),
				Action3( nullptr ) { 

	}

	explicit dev_menu_item_t( const std::string l, const callback_t a, const callback_t b, const dev_menu_item_type_t t ) 
			: Label( l ), 
								     Type( t ),
								     Action1( a ), 
								     Action2( b ),
								     Action3( nullptr ) {
	
	}

	explicit dev_menu_item_t( const std::string l, const callback_t a, const callback_t b, const callback_t c )  
		: Label( l ),
				Type( DEV_MENU_ITEM_TYPE_VARIABLE ),
				Action1( a ),
				Action2( b ),
				Action3( nullptr ) {

	}

	explicit dev_menu_item_t( std::string l )  : Label( l ), 
								                                              Type( DEV_MENU_ITEM_TYPE_UNKNOWN ),
								                                              Action1( nullptr ),
								                                              Action2( nullptr ),
								                                              Action3( nullptr ) {

	}

	explicit dev_menu_item_t()  : Label( "" ),
								                               Type( DEV_MENU_ITEM_TYPE_UNKNOWN ),
								                               Action1( nullptr ),
								                               Action2( nullptr ),
								                               Action3( nullptr ) {
	
	}

	std::string          Label;
	dev_menu_item_type_t Type;
	callback_t           Action1;
	callback_t           Action2; // only used by variables ( decrement slot )
	callback_t           Action3; // only used by variables ( decrement slot )
};

class IDevMenu {
	DECLARE_MEMBER( protected, std::vector<dev_menu_item_t*>, Items )
	DECLARE_MEMBER( protected, std::vector<IDevMenu*>,        SubMenus )
	DECLARE_MEMBER( protected, i32,                           ActiveItemIndex )
	DECLARE_MEMBER( protected, i32,                           ActiveSubMenuIndex )
	DECLARE_MEMBER( protected, i32,                           PosX )
	DECLARE_MEMBER( protected, i32,                           PosY )
	DECLARE_MEMBER( protected, std::string,                   Name )
	DECLARE_MEMBER( protected, bool,                          HasFocus )
	DECLARE_MEMBER( protected, IDevMenu*,                     Parent )

	public:
		virtual ~IDevMenu() {}
		virtual void MoveIndex( const i32 whereTo )                                                          = 0;
		virtual void OnEntrySelection()                                                                      = 0;
		virtual void Compute() const                                                                         = 0;
		virtual void OnEntryUpdate( const bool incUpdate )                                                   = 0;
		virtual void RegisterLabel( const char* label, std::string path )                                    = 0;
		virtual void RegisterFloat( f32* value, f32 step, f32 min, f32 max, std::string name, std::string path = "", 
								                      callback_t updateRoutine = nullptr )                                     = 0;
		virtual void RegisterBoolean( bool* value, std::string name, std::string path = "", 
								                        callback_t updateRoutine = nullptr )                                   = 0;
		virtual void RegisterInteger( i32* value, i32 step, i32 min, i32 max, std::string name, std::string path = "",
								                        callback_t updateRoutine = nullptr )  = 0;
		

		INLINE void UpdateFocus() {
			m_HasFocus = !m_HasFocus;
		}

		INLINE void AddItem( dev_menu_item_t* item ) {
			if( m_ActiveItemIndex == -1 ) m_ActiveItemIndex = 0;

			m_Items.push_back( item );
		}
};