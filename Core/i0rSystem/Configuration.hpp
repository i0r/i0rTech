#pragma once

static configuration_shadows_t defaultShadowConfiguration[CONFIGURATION_QUALITY_COUNT] = {
	{
		1024
	},
	{
		2048
	},
	{
		4096
	},
	{
		8192
	},
};

static configuration_t defaultConfiguration = {
	// ***************************************************************************************** Input
	{                                                                                      
		0.05,                                                                         // MouseSensitivity
		0.50,                                                                                 // MouseAcc
		{                                                                                   // KeyMapping
			// ENGINE --------------------------------------------------------------------------------------
			{ 
				HashStringCRC32( "ENGINE_ACTION_SHUTDOWN" ),
				KEY_COMBINAISON( KEY_ESCAPE, KEY_NONE ) 
			},
			{ 
				HashStringCRC32( "ENGINE_ACTION_HQ_SCREENSHOT" ),
				KEY_COMBINAISON( KEY_F12, KEY_NONE ) 
			},
			// DEBUG ---------------------------------------------------------------------------------------
			{ 
				HashStringCRC32( "DEBUG_ACTION_DISPLAY_MENU" ),
				KEY_COMBINAISON( KEY_HOME, KEY_NONE ) 
			},
			{
				HashStringCRC32( "DEBUG_ACTION_MENU_UP" ),
				KEY_COMBINAISON( KEY_PAGEUP, KEY_NONE )
			},
			{
				HashStringCRC32( "DEBUG_ACTION_MENU_DOWN" ),
				KEY_COMBINAISON( KEY_PAGEDOWN, KEY_NONE )
			},
			{
				HashStringCRC32( "DEBUG_ACTION_MENU_USE" ),
				KEY_COMBINAISON( KEY_END, KEY_NONE )
			},
			{
				HashStringCRC32( "DEBUG_ACTION_MENU_INC" ),
				KEY_COMBINAISON( KEY_INSERT, KEY_NONE )
			},
			{
				HashStringCRC32( "DEBUG_ACTION_MENU_DEC" ),
				KEY_COMBINAISON( KEY_DELETEFORWARD, KEY_NONE )
			},
			// PLAYER --------------------------------------------------------------------------------------
			{ 
				HashStringCRC32( "ACTOR_ACTION_MOVE_LEFT" ),
				KEY_COMBINAISON( KEY_Q, KEY_NONE ) 
			},
			{ 
				HashStringCRC32( "ACTOR_ACTION_MOVE_RIGHT" ),
				KEY_COMBINAISON( KEY_D, KEY_NONE ) 
			},
			{ 
				HashStringCRC32( "ACTOR_ACTION_MOVE_FORWARD" ),
				KEY_COMBINAISON( KEY_Z, KEY_NONE ) 
			},
			{ 
				HashStringCRC32( "ACTOR_ACTION_MOVE_BACKWARD" ),
				KEY_COMBINAISON( KEY_S, KEY_NONE ) 
			},
			{ 
				HashStringCRC32( "ACTOR_ACTION_USE" ),
				KEY_COMBINAISON( KEY_E, KEY_NONE ) 
			},
			{
				HashStringCRC32( "GAME_CHANGE_MODE" ),
				KEY_COMBINAISON( KEY_F, KEY_NONE )
			}, 
			// ---------------------------------------------------------------------------------------------
		},
	},
	// *************************************************************************************** Display
	{                                                                                       
		1280,                                                                              // WindowWidth
		720,                                                                              // WindowHeight
		WINDOW_DISPLAY_MODE_WINDOWED,                                                // WindowDisplayMode
		false,                                                                             // EnableVSync
		0,                                                                             // ApiVersionMajor
		0,                                                                             // ApiVersionMinor
		GRAPHICS_API_OPENGL,                                                               // GraphicsApi
		false,                                                                   // EnableApiDebugLogging
	},

	// ****************************************************************************** Graphics Quality
	{
		CONFIGURATION_QUALITY_LOW,                                                             // Shadows
	},

	// ************************************************************************************** Graphics
	{
		70.0f,                                                                                     // Fov
		0.0001f,                                                                             // NearPlane
		1000000.0f,                                                                           // FarPlane
		defaultShadowConfiguration[CONFIGURATION_QUALITY_LOW],                                 // Shadows
	}

	// ***********************************************************************************************
};

extern configuration_t _Configuration;

class Configuration {
	public:
		static void LoadInput( const char* fileName = "Input.cfg" ) ;
		static void LoadVideo( const char* fileName = "Video.cfg" ) ;
		
		static void WriteDefaultInputFile( const char* fileName ) ;
		static void WriteDefaultVideoFile( const char* fileName ) ;

	private:
		static window_display_mode_t StringToDisplayMode( const std::string configurationString ) ;
		static std::string DisplayModeToString( const window_display_mode_t dispMode ) ;
		static configuration_quality_t StringToQualityLevel( const std::string configurationString ) ;
		static graphics_api_t Configuration::StringToGraphicsApi( const std::string configurationString ) ;
};