#pragma once

#include "../IFrameBuffer.hpp"
#include <i0rRender/Texture.hpp>

static void DumpToFile( i32 width, i32 height, std::string name, bool read = false ) {
	// ONLY USE THIS FOR DEBUG (= SUPER SLOW)
	const i32 buffSize = static_cast<i32>( width * height * 3 );

	GLubyte tmp = 0x0;
	GLubyte* frameBuff = new GLubyte[buffSize];
	std::ofstream fileStream = {};

	const unsigned char TGAheader[12] = {
		0x0,
		0x0,
		0x2,
		0x0,
		0x0,
		0x0,
		0x0,
		0x0,
		0x0,
		0x0,
		0x0,
		0x0,
	};

	const unsigned char header[6] = {
		static_cast<const unsigned char>( (i32)width % 256 ),
		static_cast<const unsigned char>( width / 256 ),
		static_cast<const unsigned char>( (i32)height % 256 ),
		static_cast<const unsigned char>( height / 256 ),
		24,
		0x0
	};

	memset( frameBuff, 0xAA, buffSize ); // using 0xAA instead of 0x0 for easier debugging (hex inspection)
	
if( read )
		glReadPixels( 0, 0, static_cast<GLsizei>( width ), static_cast<GLsizei>( height ), GL_BGR, GENERIC_RENDER_UCHAR, frameBuff ); // since .TGA rgb storage is backward
	else
		glGetTexImage( GL_TEXTURE_2D, 0, GL_BGR, GENERIC_RENDER_UCHAR, frameBuff );

	fileStream.open( name + ".tga", std::ios::binary );
	fileStream.write( (char*)TGAheader, 12 );
	fileStream.write( (char*)header, 6 );
	fileStream.write( (char*)frameBuff, buffSize );
	fileStream.close();

	delete[] frameBuff;
}

static void DumpTextureToFile( texture_t* tex ) {
	DumpToFile( (i32)tex->Width, (i32)tex->Height, tex->Name );
}

static void DumpFrameBufferAttachementToFile( FrameBuffer* fbo, u32 attachement, const std::string name ) {
	fbo->Bind();
	fbo->BindAttachement( attachement );

	DumpToFile( (i32)fbo->GetFrameWidth(), (i32)fbo->GetFrameHeight(), name + std::to_string( attachement ), true );
}