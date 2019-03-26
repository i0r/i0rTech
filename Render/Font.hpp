#pragma once

#include <i0rRender/Font.hpp>

static const std::string fontFolder    = "Fonts/";
static const std::string fontExtension = ".fnt";

struct Itexture_t;
	
struct font_t {
	explicit font_t()  : Name( "Font" ), Infos( {} ), Atlas( nullptr ) {
			
	}
	
	~font_t() {
		Atlas->~Itexture_t();
		RENDER_ALLOCATED_FREE( Atlas );
		Atlas = nullptr;

		Name = "";
		memset( &Infos, 0, sizeof( Infos ) );
	}
	
	bool Load( const char* fontName ) {
		Name = fontName;

		void* texels = Instance.FileSystem->GetFileFont( fontName, Infos );
		
		if( !texels ) {
			return false;
		}

		Atlas = Instance.GraphicsApiContext->CreateTexture();
		Atlas->CreateTexture2D( { GENERIC_RENDER_R8, GENERIC_RENDER_RED, GENERIC_RENDER_UCHAR, GENERIC_RENDER_CLAMP_TO_BORDER, 0, GENERIC_RENDER_LINEAR, GENERIC_RENDER_LINEAR }, Infos.width,
								                  Infos.height, texels );
		Atlas->IsManaged = false;

		CORE_ALLOCATED_FREE( texels );
		texels = nullptr;

		return true;
	}

	std::string          Name;
	font_informations_t  Infos;
	Itexture_t*          Atlas;
};
