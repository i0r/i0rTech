#include "../Common.hpp"
#include "../Context.hpp"
#include "Texture.hpp"

/*=====================================================================
ToOpenGLType
Converts a generic texture type to a OpenGL one
=====================================================================*/
static GLint ToOpenGLType( const texture_type_t &type ) {
	switch( type ) {
		case TEXTURE_TYPE_1D:
			return GL_TEXTURE_1D;

		case TEXTURE_TYPE_2D:
			return GL_TEXTURE_2D;

		case TEXTURE_TYPE_3D:
			return GL_TEXTURE_3D;

		case TEXTURE_TYPE_CUBEMAP:
			return GL_TEXTURE_CUBE_MAP;

		case TEXTURE_TYPE_CUBEMAP_ARRAY:
			return GL_TEXTURE_CUBE_MAP_ARRAY;

		case TEXTURE_TYPE_2D_ARRAY:
			return GL_TEXTURE_2D_ARRAY;

		default:
			return GL_TEXTURE_2D;
	}
}

texture_t::texture_t() {
	Width       = 0;
	Height      = 0;
	Format      = TEXTURE_FORMAT_UNKNOWN;
	Type        = TEXTURE_TYPE_UNKNOWN;
	Name        = "Texture";
	Index       = 0;
	MipMapCount = 0;
	IsManaged   = true;
	FixedSlot   = -1;

	memset( &Informations, 0, sizeof( opengl_texture_informations_t ) );
}

texture_t::~texture_t() {
	if( !IsManaged ) {
		glDeleteTextures( 1, (GLuint*)&Index );
	}
}

void texture_t::Bind( const GLushort textureUnit ) {
	glActiveTexture( textureUnit );
	glBindTexture( OpenGLType, (GLuint)Index );
}

void texture_t::CreateTexture2D( const opengl_texture_informations_t infos, const GLsizei width, const GLsizei height, 
								                         const void* data ) {
	Type = TEXTURE_TYPE_2D;
	OpenGLType = ToOpenGLType( Type );
	Informations = infos;
	Width = width;
	Height = height;
	Depth = 1;

	if( !Index ) {
		glGenTextures( 1, &Index );
	}

	glBindTexture( OpenGLType, Index );
	glTexImage2D( OpenGLType, 0, Informations.InternalFormat, width,
		height, Informations.Border, Informations.Format, Informations.Type,
		data );
	glTexParameteri( OpenGLType, GL_TEXTURE_MAG_FILTER,
		Informations.MagFilter );
	glTexParameteri( OpenGLType, GL_TEXTURE_MIN_FILTER,
		Informations.MinFilter );
	glTexParameteri( OpenGLType, GL_TEXTURE_WRAP_S,
		Informations.TextureWrapping );
	glTexParameteri( OpenGLType, GL_TEXTURE_WRAP_T,
		Informations.TextureWrapping );

	if( Informations.MinFilter == GL_LINEAR_MIPMAP_LINEAR ) {
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 11 );
		glGenerateMipmap( GL_TEXTURE_2D );

		MipMapCount = 11;
	} else {
		MipMapCount = 1;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1 );
	}

	glBindTexture( OpenGLType, 0 );
}

void texture_t::CreateTextureCubeMapArray( const opengl_texture_informations_t infos, const GLsizei width,
								                                   const GLsizei height, const GLsizei depth ) {
	Type = TEXTURE_TYPE_CUBEMAP_ARRAY;
	OpenGLType = ToOpenGLType( Type );
	Informations = infos;
	Width = width;
	Height = height;
	Depth = depth;

	if( !Index ) {
		glGenTextures( 1, &Index );
	}

	glBindTexture( GL_TEXTURE_CUBE_MAP_ARRAY, Index );
	glTexImage3D( GL_TEXTURE_CUBE_MAP_ARRAY, 0, Informations.InternalFormat, Width, Height, 6 * Depth, Informations.Border, 
								       Informations.Format, Informations.Type, NULL );

	glTexParameteri( OpenGLType, GL_TEXTURE_MAG_FILTER, Informations.MagFilter );
	glTexParameteri( OpenGLType, GL_TEXTURE_MIN_FILTER, Informations.MinFilter );
	glTexParameteri( OpenGLType, GL_TEXTURE_WRAP_S, Informations.TextureWrapping );
	glTexParameteri( OpenGLType, GL_TEXTURE_WRAP_T, Informations.TextureWrapping );
	glTexParameteri( OpenGLType, GL_TEXTURE_WRAP_R, Informations.TextureWrapping );
	glBindTexture(OpenGLType, 0);
}

void texture_t::CreateTextureCubeMap( const opengl_texture_informations_t infos, const GLsizei width, 
								                              const GLsizei height, const void* data ) {
	Type = TEXTURE_TYPE_CUBEMAP;
	OpenGLType = ToOpenGLType( Type );
	Informations = infos;
	Width = width;
	Height = height;
	Depth = 6;

	if( !Index ) {
		glGenTextures( 1, &Index );
	}

	glBindTexture( OpenGLType, Index );
	for( GLuint i = 0; i < 6; ++i ) {
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Informations.InternalFormat, width, height, Informations.Border,
								        Informations.Format, Informations.Type, data );
	}

	glTexParameteri( OpenGLType, GL_TEXTURE_MAG_FILTER, Informations.MagFilter );
	glTexParameteri( OpenGLType, GL_TEXTURE_MIN_FILTER, Informations.MinFilter );
	glTexParameteri( OpenGLType, GL_TEXTURE_WRAP_S, Informations.TextureWrapping );
	glTexParameteri( OpenGLType, GL_TEXTURE_WRAP_T, Informations.TextureWrapping );
	glTexParameteri( OpenGLType, GL_TEXTURE_WRAP_R, Informations.TextureWrapping );
	glBindTexture(OpenGLType, 0);
}

void texture_t::CreateTexture3D( const opengl_texture_informations_t infos, const GLsizei width, const GLsizei height, 
								                         const GLsizei depth, const void* data ) {
	Type = TEXTURE_TYPE_3D;
	OpenGLType = ToOpenGLType( Type );
	Informations = infos;
	Width = width;
	Height = height;
	Depth = depth;

	if( !Index ) {
		glGenTextures( 1, &Index );
	}

	glBindTexture( OpenGLType, Index );
	glTexImage3D( OpenGLType, 0, Informations.InternalFormat, Width, Height, Depth, Informations.Border, Informations.Format, 
								       Informations.Type, data );
	glTexParameteri( OpenGLType, GL_TEXTURE_MAG_FILTER, Informations.MagFilter );
	glTexParameteri( OpenGLType, GL_TEXTURE_MIN_FILTER, Informations.MinFilter );
	glTexParameteri( OpenGLType, GL_TEXTURE_WRAP_S, Informations.TextureWrapping );
	glTexParameteri( OpenGLType, GL_TEXTURE_WRAP_T, Informations.TextureWrapping );
	glTexParameteri( OpenGLType, GL_TEXTURE_WRAP_R, Informations.TextureWrapping );

	MipMapCount = 1;
	glBindTexture(OpenGLType, 0);
}

void texture_t::CreateTextureDepthArray( const opengl_texture_informations_t infos, const GLsizei width, const GLsizei height, const GLsizei depth,
								                                 const GLint compareFunc, const GLfloat compareMode, const void* data  ) {
	Type = TEXTURE_TYPE_2D_ARRAY;
	OpenGLType = ToOpenGLType( Type );
	Informations = infos;
	Width = width;
	Height = height;
	Depth = depth;

	if( !Index ) {
		glGenTextures( 1, &Index );
	}

	glBindTexture( OpenGLType, Index );
	glTexImage3D( OpenGLType, 0, Informations.InternalFormat, Width, Height, Depth, Informations.Border, Informations.Format,
		Informations.Type, data );
	glTexParameteri( OpenGLType, GL_TEXTURE_MAG_FILTER, Informations.MagFilter );
	glTexParameteri( OpenGLType, GL_TEXTURE_MIN_FILTER, Informations.MinFilter );
	glTexParameteri( OpenGLType, GL_TEXTURE_WRAP_S, Informations.TextureWrapping );
	glTexParameteri( OpenGLType, GL_TEXTURE_WRAP_T, Informations.TextureWrapping );
	glTexParameteri( OpenGLType, GL_TEXTURE_WRAP_R, Informations.TextureWrapping );

	glTexParameteri( OpenGLType, GL_TEXTURE_COMPARE_FUNC, compareFunc );
	glTexParameterf( OpenGLType, GL_TEXTURE_COMPARE_MODE, compareMode );

	MipMapCount = 1;
	glBindTexture( OpenGLType, 0 );
}

void texture_t::AddMipLevel3D( const GLint mipLevel, const GLsizei width, const GLsizei height, const GLsizei depth, 
								                       const void* data ) {
	glBindTexture( GL_TEXTURE_3D, Index );
	glTexImage3D( OpenGLType, mipLevel, Informations.InternalFormat, width, height, depth, Informations.Border, 
								       Informations.Format, Informations.Type, data );
	glBindTexture(GL_TEXTURE_3D, 0);
}
