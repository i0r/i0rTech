#pragma once

#include <i0rRender/ITexture.hpp>

/*=====================================================================
texture_t
	Texture
=====================================================================*/
struct texture_t : public Itexture_t {
	explicit texture_t() ;
	~texture_t() ;

	void Bind( const u16 textureUnit = GENERIC_RENDER_TEXTURE_SLOT_0 ) ;
	
	void CreateTexture2D( const opengl_texture_informations_t infos, const i32 width, const i32 height, 
								               const void* data = nullptr ) ;

	void CreateTextureCubeMapArray( const opengl_texture_informations_t infos, const i32 width, const i32 height, 
								                        const i32 depth ) ;

	void CreateTextureCubeMap( const opengl_texture_informations_t infos, const i32 width, const i32 height, 
								                    const void* data = nullptr ) ;

	void CreateTexture3D( const opengl_texture_informations_t infos, const i32 width, const i32 height, const i32 depth, 
								               const void* data = nullptr ) ;

	void CreateTextureDepthArray( const opengl_texture_informations_t infos, const i32 width, const i32 height, 
								                       const i32 depth, const i32 compareFunc, const f32 compareMode, 
								                       const void* data = nullptr ) ;

	void AddMipLevel3D( const i32 mipLevel, const i32 width, const i32 height, const i32 depth, 
								             const void* data = nullptr ) ;
};
