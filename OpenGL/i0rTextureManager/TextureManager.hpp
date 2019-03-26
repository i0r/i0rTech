#pragma once

#include <i0rRender/ITextureManager.hpp>

enum texture_format_t;

const std::string textureFolder    = "Textures/";
const std::string textureDefault   = "default";
const std::string textureExtension = ".dds";

/*=====================================================================
TextureManager
=====================================================================*/
class TextureManager : public ITextureManager {
	public:
		explicit TextureManager() ;
		~TextureManager() ;

		bool        Initialize() ;
		Itexture_t* GetTexture2D( const std::string textureName = textureDefault ) ;
		void        Reload( std::string &textureName ) ;

	private:
		void CreateTexture2D( Itexture_t* texture, const char* texturePath ) ;

		std::map<std::string, Itexture_t*>                  m_Textures;
		std::map<std::string, Itexture_t*>::const_iterator  m_TextureFind;
};
