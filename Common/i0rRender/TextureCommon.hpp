#pragma once

/*=====================================================================
texture_mipmap_t
	Texture mipmap
=====================================================================*/
struct texture_mipmap_t {
	u32               Width;
	u32               Height;
	u32               Depth;
	u32               Size;
	std::vector<u8>   Data;
};

/*=====================================================================
texture_type_t
	Texture type list
=====================================================================*/
enum texture_type_t {
	TEXTURE_TYPE_UNKNOWN,
	TEXTURE_TYPE_1D,
	TEXTURE_TYPE_2D,
	TEXTURE_TYPE_2D_ARRAY,
	TEXTURE_TYPE_3D,
	TEXTURE_TYPE_CUBEMAP,
	TEXTURE_TYPE_CUBEMAP_ARRAY,
	TEXTURE_TYPE_COUNT,
};

/*=====================================================================
texture_format_t
	Texture format list
=====================================================================*/
enum texture_format_t {
	TEXTURE_FORMAT_UNKNOWN,
	TEXTURE_FORMAT_LUMINANCE,
	TEXTURE_FORMAT_RED,
	TEXTURE_FORMAT_RGB,
	TEXTURE_FORMAT_RGBA,
	TEXTURE_FORMAT_RGBA_DXT1,
	TEXTURE_FORMAT_RGBA_DXT3,
	TEXTURE_FORMAT_RGBA_DXT5,
	TEXTURE_FORMAT_BGR,
	TEXTURE_FORMAT_BGRA,
	TEXTURE_FORMAT_COUNT,
};
