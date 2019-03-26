#pragma once

#include "TextureCommon.hpp"

/*=====================================================================
opengl_texture_informations_t
Informations required to build a texture on GPU
=====================================================================*/
struct opengl_texture_informations_t {
	i32  InternalFormat;
	u32  Format;
	u32  Type;
	i32  TextureWrapping;
	i32  Border;
	i32  MagFilter;
	i32  MinFilter;
};

/*=====================================================================
texture_t
	Texture
=====================================================================*/
struct Itexture_t {
	virtual ~Itexture_t() {
		Name.clear();

		Width = 0;
		Height = 0;
		Format = TEXTURE_FORMAT_UNKNOWN;
		Type = TEXTURE_TYPE_UNKNOWN;
		OpenGLType = 0;
		Index = 0;
		MipMapCount = 0;
		IsManaged = false;
		FixedSlot = 0;

		memset( &Informations, 0, sizeof( opengl_texture_informations_t ) );
	};

	virtual void Bind( const u16 textureUnit = 0x84C0 )  = 0;
	
	virtual void CreateTexture2D( const opengl_texture_informations_t infos, const i32 width, const i32 height,
								               const void* data = nullptr )  = 0;

	virtual void CreateTextureCubeMapArray( const opengl_texture_informations_t infos, const i32 width, const i32 height,
								                        const i32 depth )  = 0;

	virtual void CreateTextureCubeMap( const opengl_texture_informations_t infos, const i32 width, const i32 height,
								                    const void* data = nullptr )  = 0;

	virtual void CreateTexture3D( const opengl_texture_informations_t infos, const i32 width, const i32 height, const i32 depth,
								               const void* data = nullptr )  = 0;

	virtual void CreateTextureDepthArray( const opengl_texture_informations_t infos, const i32 width, const i32 height,
								                       const i32 depth, const i32 compareFunc, const f32 compareMode, 
								                       const void* data = nullptr )  = 0;

	virtual void AddMipLevel3D( const i32 mipLevel, const i32 width, const i32 height, const i32 depth,
								             const void* data = nullptr )  = 0;

	std::string                   Name;
	i32                           Width;
	i32                           Height;
	i32                           Depth;
	texture_format_t              Format;
	texture_type_t                Type;
	i32                           OpenGLType;
	u32                           Index;
	i32                           MipMapCount;
	bool                          IsManaged;
	opengl_texture_informations_t Informations;
	i32                           FixedSlot;
};