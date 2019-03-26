#include "../Common.hpp"
#include <i0rRender/IRenderWindow.hpp>
#include <i0rCore/Keys.hpp>
#include "Configuration.hpp"

configuration_t _Configuration = defaultConfiguration;

void Configuration::WriteDefaultVideoFile( const char* fileName ) {
	std::vector<std::string> content = {};

	content.push_back( "window_width = " + std::to_string( _Configuration.Display.WindowWidth ) );
	content.push_back( "window_height = " + std::to_string( _Configuration.Display.WindowHeight ) );
	content.push_back( "window_display_mode = " + DisplayModeToString( _Configuration.Display.WindowDisplayMode ) );

	content.push_back( "game_fov = " + std::to_string( _Configuration.Graphics.Fov ) );
	content.push_back( "game_far_plane = " + std::to_string( _Configuration.Graphics.FarPlane ) );
	content.push_back( "render_vsync = " + BoolToString( _Configuration.Display.EnableVSync ) );

	std::string tContent = TextFileToString( content );
	FText::Write( fileName, tContent.c_str() );

	CONSOLE_PRINT_WARNING( "Configuration::WriteDefaultVideoFile => Wrote configuration file %s\n", fileName );
}

void Configuration::WriteDefaultInputFile( const char* fileName ) {
	std::string content = "";
	FText::Write( fileName, content.c_str() );
}

void Configuration::LoadInput( const char* fileName ) {
	std::map<std::string, std::string> dictionary = FDictionary::Read( fileName );
	std::map<std::string, std::string>::const_iterator dictionaryIterator = {};

	if( dictionary.empty() ) {
		WriteDefaultInputFile( fileName );
		return;
	}

	DICTIONARY_FIND_KEY( "mouse_sensitivity" ) {
		_Configuration.Input.MouseSensitivity = atof( dictionaryIterator->second.c_str() );
		dictionary.erase( dictionaryIterator );
	}

	DICTIONARY_FIND_KEY( "mouse_acceleration" ) {
		_Configuration.Input.MouseAcc = atof( dictionaryIterator->second.c_str() );
		dictionary.erase( dictionaryIterator );
	}

	// any input option should be registered before the lazy keymapping parsing
	std::string firstKey = "", secondKey = "";
	std::size_t separator = -1;

	for( dictionaryIterator = dictionary.begin(); dictionaryIterator !=
		dictionary.end(); dictionaryIterator++ ) {
		separator = dictionaryIterator->second.find_first_of( ',' );
		firstKey = dictionaryIterator->second.substr( 0, separator );
		secondKey = dictionaryIterator->second.substr( separator + 1 );

		TrimString( firstKey );
		TrimString( secondKey );

		_Configuration.Input.KeyMapping[HashStringCRC32( dictionaryIterator->first )] =
			KEY_COMBINAISON( atoi( firstKey.c_str() ), atoi( secondKey.c_str() ) );
	}
}

void Configuration::LoadVideo( const char* fileName ) {
	std::map<std::string, std::string> dictionary                         = FDictionary::Read( fileName );
	std::map<std::string, std::string>::const_iterator dictionaryIterator = {};

	if( dictionary.empty() ) {
		CONSOLE_PRINT_WARNING( "Configuration::LoadVideo => Configuration file %s not found!\n", fileName );
		
		WriteDefaultVideoFile( fileName );
		return;
	}

	DICTIONARY_FIND_KEY( "window_width" ) {
		_Configuration.Display.WindowWidth = atoi( dictionaryIterator->second.c_str() );
	}

	DICTIONARY_FIND_KEY( "window_height" ) {
		_Configuration.Display.WindowHeight = atoi( dictionaryIterator->second.c_str() );
	}

	DICTIONARY_FIND_KEY( "window_display_mode" ) {
		_Configuration.Display.WindowDisplayMode = StringToDisplayMode( dictionaryIterator->second );
	}

	DICTIONARY_FIND_KEY( "game_fov" ) {
		_Configuration.Graphics.Fov = (f32)atof( dictionaryIterator->second.c_str() );
	}

	DICTIONARY_FIND_KEY( "game_far_plane" ) {
		_Configuration.Graphics.FarPlane = (f32)atof( dictionaryIterator->second.c_str() );
	}

	DICTIONARY_FIND_KEY( "game_near_plane" ) {
		_Configuration.Graphics.NearPlane = (f32)atof( dictionaryIterator->second.c_str() );
	}

	DICTIONARY_FIND_KEY( "render_vsync" ) {
		_Configuration.Display.EnableVSync = StringToBool( dictionaryIterator->second.c_str() );
	}

	DICTIONARY_FIND_KEY( "shadows_quality" ) {
		_Configuration.Quality.Shadows = StringToQualityLevel( dictionaryIterator->second.c_str() );
		_Configuration.Graphics.Shadows = defaultShadowConfiguration[_Configuration.Quality.Shadows];
	}

	#ifdef FLAG_DEBUG
		DICTIONARY_FIND_KEY( "debug_enable_api_logging" ) {
			_Configuration.Display.EnableApiDebugLogging = StringToBool( dictionaryIterator->second.c_str() );
		}
	#endif

	DICTIONARY_FIND_KEY( "render_api" ) {
		_Configuration.Display.GraphicsApi = StringToGraphicsApi( dictionaryIterator->second.c_str() );

		if( _Configuration.Display.GraphicsApi == GRAPHICS_API_DIRECTX_11 ) {
			_Configuration.Display.ApiVersionMajor = 11;
			_Configuration.Display.ApiVersionMinor = 0;
		} else if( _Configuration.Display.GraphicsApi == GRAPHICS_API_DIRECTX_12 ) {
			_Configuration.Display.ApiVersionMajor = 12;
			_Configuration.Display.ApiVersionMinor = 0;
		}
	}
}

window_display_mode_t Configuration::StringToDisplayMode( const std::string configurationString ) {
	std::string cfgString = configurationString;

	RemoveSpaces( cfgString );
	ToLower( cfgString );

	if( cfgString == "windowed" || cfgString == "displaywindowed" ||
		cfgString == "displaymodewindowed" || cfgString == "i0rTechwindowed" ) {
		return WINDOW_DISPLAY_MODE_WINDOWED;
	} else if( cfgString == "fullscreen" || cfgString == "displayfullscreen" ||
		cfgString == "displaymodefullscreen" ||
		cfgString == "i0rTechfullscreen" ) {
		return WINDOW_DISPLAY_MODE_FULLSCREEN;
	} else if( cfgString == "fullscreenborderless" ||
		cfgString == "displayfullscreenborderless" ||
		cfgString == "displaymodefullscreenborderless" ||
		cfgString == "i0rTechfullscreenborderless" ||
		cfgString == "borderless" ||
		cfgString == "displayborderless" ||
		cfgString == "displaymodeborderless" ||
		cfgString == "i0rTechborderless" ) {
		return WINDOW_DISPLAY_MODE_FULLSCREEN_BORDERLESS;
	}

	return WINDOW_DISPLAY_MODE_WINDOWED;
}

std::string Configuration::DisplayModeToString( const window_display_mode_t dispMode ) {
	if( dispMode == WINDOW_DISPLAY_MODE_FULLSCREEN ) 
		return "fullscreen";
	else if( dispMode == WINDOW_DISPLAY_MODE_FULLSCREEN_BORDERLESS ) 
		return "borderless";
	else 
		return "windowed";
}

configuration_quality_t Configuration::StringToQualityLevel( const std::string configurationString ) {
	std::string cfgString( configurationString );

	RemoveSpaces( cfgString );
	ToLower( cfgString );

	if( cfgString == "low" || cfgString == "lowquality" || cfgString == "0" ) {
		return CONFIGURATION_QUALITY_LOW;
	} else if( cfgString == "medium" || cfgString == "mediumquality" || cfgString == "1" ) {
		return CONFIGURATION_QUALITY_MEDIUM;
	} else if( cfgString == "high" || cfgString == "highquality" || cfgString == "2" ) {
		return CONFIGURATION_QUALITY_HIGH;
	} else if( cfgString == "ultra" || cfgString == "ultraquality" || cfgString == "3" ) {
		return CONFIGURATION_QUALITY_ULTRA;
	}

	return CONFIGURATION_QUALITY_LOW;
}

graphics_api_t Configuration::StringToGraphicsApi( const std::string configurationString ) {
	std::string cfgString( configurationString );

	RemoveSpaces( cfgString );
	ToLower( cfgString );

	if( cfgString == "gl" || cfgString == "opengl" || cfgString == "ogl" || cfgString == "0" ) {
		return GRAPHICS_API_OPENGL;
	} else if( cfgString == "dx11" || cfgString == "directx11" || cfgString == "d11" || cfgString == "1" ) {
		return GRAPHICS_API_DIRECTX_11;
	} else if( cfgString == "dx12" || cfgString == "directx12" || cfgString == "d12" || cfgString == "2" ) {
		return GRAPHICS_API_DIRECTX_11;
	} else if( cfgString == "vulkan" || cfgString == "vk" || cfgString == "3" ) {
		return GRAPHICS_API_VULKAN;
	}

	return GRAPHICS_API_OPENGL; // 'safest' graphics api available
}
