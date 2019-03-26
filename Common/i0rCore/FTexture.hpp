#pragma once

#include <i0rRender/TextureCommon.hpp>

struct file_texture_t {
	explicit file_texture_t( const char* fileName = "FTexture" )  : Name( fileName ),
								                                                                 Type( TEXTURE_TYPE_UNKNOWN ),
								                                                                 Format( TEXTURE_FORMAT_UNKNOWN ) {
	
	}

	~file_texture_t() {
		for( texture_mipmap_t mipLevel : MipMaps ) {
			mipLevel.Width  = 0;
			mipLevel.Height = 0;
			mipLevel.Depth  = 0;
			mipLevel.Size   = 0;
			mipLevel.Data.clear();
		}

		MipMaps.clear();
		memset( &Name, 0, strlen( Name ) );
		Type   = TEXTURE_TYPE_UNKNOWN;
		Format = TEXTURE_FORMAT_UNKNOWN;
	}

	INLINE bool IsValid() const  {
		return MipMaps.size() ? true : false;
	}

	const char*                   Name;
	texture_type_t                Type;
	texture_format_t              Format;

	std::vector<texture_mipmap_t> MipMaps;
};
