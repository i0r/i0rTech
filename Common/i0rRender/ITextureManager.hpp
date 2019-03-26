#pragma once

enum texture_format_t;
struct texture_t;

/*=====================================================================
TextureManager
=====================================================================*/
class ITextureManager {
	public:
		virtual ~ITextureManager() {}
		virtual bool        Initialize()                                              = 0;
		virtual Itexture_t* GetTexture2D( const std::string textureName )             = 0;
		virtual void        Reload( std::string &textureName )                        = 0;

	protected:
		virtual void CreateTexture2D( Itexture_t* texture, const char* texturePath )  = 0;
};
