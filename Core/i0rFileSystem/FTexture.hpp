#pragma once

#include <i0rCore/FTexture.hpp>
#include <i0rRender/TextureCommon.hpp>

/*====================
::_dds_pixel_format_t
.DDS header pixel informations
====================*/
struct dds_pixel_format_t {
	u32    dwSize;
	u32    dwFlags;
	u32    dwFourCC;
	u32    dwRGBBitCount;
	u32    dwRBitMask;
	u32    dwGBitMask;
	u32    dwBBitMask;
	u32    dwABitMask;
};

/*====================
::_dds_header_t
.DDS header
====================*/
struct dds_header_t {
	u32                dwSize;
	u32                dwFlags;
	u32                dwHeight;
	u32                dwWidth;
	u32                dwPitchOrLinearSize;
	u32                dwDepth;
	u32                dwMipMapCount;
	u32                dwReserved1[11];
	dds_pixel_format_t ddspf;
	u32                dwCaps1;
	u32                dwCaps2;
	u32                dwReserved2[3];
};

// surface description flags
const u32 DDSF_CAPS        = 0x00000001;
const u32 DDSF_HEIGHT      = 0x00000002;
const u32 DDSF_WIDTH       = 0x00000004;
const u32 DDSF_PITCH       = 0x00000008;
const u32 DDSF_PIXELFORMAT = 0x00001000;
const u32 DDSF_MIPMAPCOUNT = 0x00020000;
const u32 DDSF_LINEARSIZE  = 0x00080000;
const u32 DDSF_DEPTH       = 0x00800000;

// pixel format flags
const u32 DDSF_ALPHAPIXELS = 0x00000001;
const u32 DDSF_FOURCC      = 0x00000004;
const u32 DDSF_RGB         = 0x00000040;
const u32 DDSF_RGBA        = 0x00000041;

// dwCaps1 flags
const u32 DDSF_COMPLEX     = 0x00000008;
const u32 DDSF_TEXTURE     = 0x00001000;
const u32 DDSF_MIPMAP      = 0x00400000;

// dwCaps2 flags
const u32 DDSF_CUBEMAP           = 0x00000200;
const u32 DDSF_CUBEMAP_POSITIVEX = 0x00000400;
const u32 DDSF_CUBEMAP_NEGATIVEX = 0x00000800;
const u32 DDSF_CUBEMAP_POSITIVEY = 0x00001000;
const u32 DDSF_CUBEMAP_NEGATIVEY = 0x00002000;
const u32 DDSF_CUBEMAP_POSITIVEZ = 0x00004000;
const u32 DDSF_CUBEMAP_NEGATIVEZ = 0x00008000;
const u32 DDSF_CUBEMAP_ALL_FACES = 0x0000FC00;
const u32 DDSF_VOLUME            = 0x00200000;

// compressed texture types
const u32 FOURCC_DXT1 = 0x31545844;
const u32 FOURCC_DXT3 = 0x33545844;
const u32 FOURCC_DXT5 = 0x35545844;

class FTextureDDS {
	public:
		static file_texture_t* Load( char* &ptr ) {
			file_texture_t* tex = new file_texture_t( "Texture" );

			dds_header_t ddsHeader       = {};
			texture_mipmap_t mipmapLevel = {};

			ddsHeader = FILE_READ( dds_header_t );
			tex->Type = TEXTURE_TYPE_2D;

			if( ddsHeader.dwCaps2 & DDSF_CUBEMAP ) {
				tex->Type = TEXTURE_TYPE_CUBEMAP;
			}

			if( ( ddsHeader.dwCaps2 & DDSF_VOLUME ) && ddsHeader.dwDepth ) {
				tex->Type = TEXTURE_TYPE_3D;
			}

			if( ddsHeader.ddspf.dwFlags & DDSF_FOURCC ) {
				switch( ddsHeader.ddspf.dwFourCC ) {
				case FOURCC_DXT1:
					tex->Format = TEXTURE_FORMAT_RGBA_DXT1;
					break;

				case FOURCC_DXT3:
					tex->Format = TEXTURE_FORMAT_RGBA_DXT3;
					break;
				case FOURCC_DXT5:
					tex->Format = TEXTURE_FORMAT_RGBA_DXT5;
					break;
				default:
					CONSOLE_PRINT_ERROR( "%s => Unknown format magic 0x%x\n", __FUNCTION__, ddsHeader.ddspf.dwFourCC );
					delete tex;
					return nullptr;
				}
			}
			else if( ddsHeader.ddspf.dwRGBBitCount == 32      && ddsHeader.ddspf.dwRBitMask == 0x00FF0000 
								 && ddsHeader.ddspf.dwGBitMask == 0x0000FF00 && ddsHeader.ddspf.dwBBitMask == 0x000000FF 
								 && ddsHeader.ddspf.dwABitMask == 0xFF000000 ) {
				tex->Format = TEXTURE_FORMAT_BGRA;
			}
			else if( ddsHeader.ddspf.dwRGBBitCount == 32      && ddsHeader.ddspf.dwRBitMask == 0x000000FF 
								 && ddsHeader.ddspf.dwGBitMask == 0x0000FF00 && ddsHeader.ddspf.dwBBitMask == 0x00FF0000
								 && ddsHeader.ddspf.dwABitMask == 0xFF000000 ) {
				tex->Format = TEXTURE_FORMAT_RGBA;
			}
			else if( ddsHeader.ddspf.dwRGBBitCount == 24      && ddsHeader.ddspf.dwRBitMask == 0x000000FF 
								 && ddsHeader.ddspf.dwGBitMask == 0x0000FF00 && ddsHeader.ddspf.dwBBitMask == 0x00FF0000 ) {
				tex->Format = TEXTURE_FORMAT_RGB;
			}
			else if( ddsHeader.ddspf.dwRGBBitCount == 24      && ddsHeader.ddspf.dwRBitMask == 0x00FF0000
								 && ddsHeader.ddspf.dwGBitMask == 0x0000FF00 && ddsHeader.ddspf.dwBBitMask == 0x000000FF ) {
				tex->Format = TEXTURE_FORMAT_BGR;
			}
			else if( ddsHeader.ddspf.dwRGBBitCount == 8 ) {
				tex->Format = TEXTURE_FORMAT_LUMINANCE;
			}
			else {
				CONSOLE_PRINT_ERROR("%s => Unknown format magic\n", __FUNCTION__);
				delete tex;
				return nullptr;
			}

			// :TODO: add cubemap support
			u32 iterations = ( tex->Type == TEXTURE_TYPE_CUBEMAP ) ? 6 : 1,

			// there should be AT LEAST one mipmap level ( main surface )
			mipMapLeft   = ( ddsHeader.dwMipMapCount ) ? ddsHeader.dwMipMapCount : 0,
			mipMapWidth  = ddsHeader.dwWidth,
			mipMapHeight = ddsHeader.dwHeight,
			mipMapDepth  = ( ddsHeader.dwDepth <= 0 ) ? 1 : ddsHeader.dwDepth;

			for( ; mipMapLeft; mipMapLeft-- ) {
				mipmapLevel.Width  = mipMapWidth;
				mipmapLevel.Height = mipMapHeight;
				mipmapLevel.Depth  = mipMapDepth;
				mipmapLevel.Size   = GetMipMapSizeDDS( tex->Format, mipmapLevel.Width, mipmapLevel.Height, mipmapLevel.Depth );

				if( !mipmapLevel.Size ) { // if the mipmap is too small or empty
					break;
				}

				mipmapLevel.Data.resize( mipmapLevel.Size );
				mipmapLevel.Data.assign( ptr, ptr + mipmapLevel.Size );
				ptr += mipmapLevel.Size * sizeof( u8 );

				tex->MipMaps.push_back( mipmapLevel );

				mipmapLevel = {};

				( ( mipMapWidth  >>= 1 ) <= 0 ) ? 1 : mipMapWidth;
				( ( mipMapHeight >>= 1 ) <= 0 ) ? 1 : mipMapHeight;
			}

			return tex;
		}

		private:
			static u32 GetMipMapSizeDDS( const texture_format_t format, const u32 width, const u32 height, 
								                        const u32 depth ) {
				if( format == TEXTURE_FORMAT_RGB || format == TEXTURE_FORMAT_BGR ) {
					return width * height * 3 * depth;
				}
				else if( format == TEXTURE_FORMAT_RGBA || format == TEXTURE_FORMAT_BGRA ) {
					return ( ( width + 3 ) / 4 ) * ( ( height + 3 ) / 4 ) * 16;
				}
				else {
					return width * height * depth;
				}
			}
};