#include "../Common.hpp"
#include <i0rCore/FTexture.hpp>
#include <i0rRender/TextureConstants.hpp>
#include "TextureManager.hpp"

/*=====================================================================
ToOpenGLFormat
Converts a generic texture format to a OpenGL one
=====================================================================*/
static GLushort ToOpenGLFormat( const texture_format_t texFormat ) {
	switch( texFormat ) {
		case TEXTURE_FORMAT_RGBA_DXT1:
			return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;

		case TEXTURE_FORMAT_RGBA_DXT3:
			return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;

		case TEXTURE_FORMAT_RGBA_DXT5:
			return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;

		default:
			return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
	}
}

TextureManager::TextureManager() {

}

TextureManager::~TextureManager() {
	std::vector<GLuint> toDelete = {};
	for( std::pair<std::string, Itexture_t*> texture : m_Textures ) {
		glDeleteTextures( 1, (GLuint*)&texture.second->Index );

		texture.second->~Itexture_t();
		RENDER_ALLOCATED_FREE( texture.second );
		texture.second = nullptr;

		CONSOLE_PRINT_INFO( "TextureManager::~TextureManager => Texture %s released\n", texture.first.c_str() );

		texture.first = "";
	}

	m_Textures.clear();
}

/*=====================================================================
Initialize
	Initializes the TextureManager
=====================================================================*/
bool TextureManager::Initialize() {
	Itexture_t* defaultTexture = GetTexture2D();
	defaultTexture->Bind( TEXTURE_BIND_DEFAULT_TEXTURE );

	return ( defaultTexture != nullptr );
}

/*=====================================================================
Reload
	Reloads a texture that was modified on the disk
=====================================================================*/
void TextureManager::Reload( std::string &textureName ) {
	for( std::pair<std::string, Itexture_t*> texture : m_Textures ) {
		if( texture.second->Name == textureName ) {
			glDeleteTextures( 1, (GLuint*)&texture.second->Index );
			CreateTexture2D( texture.second, textureName.c_str() );

			CONSOLE_PRINT_INFO( "TextureManager::Reload => Texture %s has b"
								               "een reloaded\n", texture.first.c_str() );
			return;
		}
	}

	CONSOLE_PRINT_WARNING( "TextureManager::Reload => Tried to reload non"
								                "managed texture %s\n", textureName.c_str() );
}

/*=====================================================================
CreateTexture2D
	Creates a 2D texture using a texture ressource file
=====================================================================*/
void TextureManager::CreateTexture2D( Itexture_t* texture, const char* texturePath ) {
	file_texture_t* textureFile = Instance.FileSystem->GetFileTexture( texturePath );

	if( !textureFile || !textureFile->MipMaps.size() ) {
		CONSOLE_PRINT_ERROR( "TextureManager::CreateTexture2D => Failed to create texture %s (file is empty)\n", texturePath );
		texture = m_Textures[textureDefault];
		return;
	}

	texture->IsManaged   = true;
	texture->Format      = textureFile->Format;
	texture->Width       = textureFile->MipMaps[0].Width;
	texture->Height      = textureFile->MipMaps[0].Height;
	texture->MipMapCount = (i32)textureFile->MipMaps.size();
	texture->Name        = texturePath;
	texture->Type        = TEXTURE_TYPE_2D;
	texture->OpenGLType  = GL_TEXTURE_2D;
	texture->Index       = 0;

	glGenTextures( 1, (GLuint*)&texture->Index );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, (GLuint)texture->Index );

	u16 mipLevel = 0;
	for( texture_mipmap_t &mipMapLevel : textureFile->MipMaps ) {
		// ogl doesnt like mipmaps smaller than 2x2 pixels
		if( mipMapLevel.Width <= 2 || mipMapLevel.Height <= 2 ) {
			texture->MipMapCount = mipLevel - 1;
			break;
		}

		glCompressedTexImage2D( GL_TEXTURE_2D, mipLevel++, 
								                  ToOpenGLFormat( textureFile->Format ),
								                  mipMapLevel.Width, mipMapLevel.Height, 
								                  0, mipMapLevel.Size, &mipMapLevel.Data[0] );
	}

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GENERIC_RENDER_LINEAR );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, texture->MipMapCount );

	glBindTexture( GL_TEXTURE_2D, 0 );

	if( texture->FixedSlot != -1 ) {
		texture->Bind( texture->FixedSlot ); 
	}
}

/*=====================================================================
GetTexture2D
	Gets a 2D texture from the manager; if it is not loaded yet, loads it 
	from the disk
=====================================================================*/
Itexture_t* TextureManager::GetTexture2D( const std::string textureName ) {
	m_TextureFind = m_Textures.find( textureName );

	if( m_TextureFind != m_Textures.end() ) {
		if( m_TextureFind->second->Type == TEXTURE_TYPE_2D ) {
			return m_TextureFind->second;
		} else {
			CONSOLE_PRINT_ERROR( "TextureManager::GetTexture2D => Texture found"
				" type does not match with expected type ( expected: %i; got: %i )"
				"\n", TEXTURE_TYPE_2D, m_TextureFind->second->Type );
				return m_Textures[textureDefault];
		}
	}

	const std::string textureFullPath = textureFolder + textureName + textureExtension;

	Itexture_t* texture = Instance.GraphicsApiContext->CreateTexture();
	CreateTexture2D( texture, textureFullPath.c_str() );
	m_Textures.insert( std::make_pair( textureName, texture ) );

	return texture;
}
