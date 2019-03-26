#pragma once

static i32 OpenGLDrawType( const i32 genericAttribute ) {
	switch( genericAttribute ) {
		case GENERIC_RENDER_STATIC_DRAW:
			return GL_STATIC_DRAW;

		case GENERIC_RENDER_DYNAMIC_DRAW:
			return GL_DYNAMIC_DRAW;

		default:
			return GL_STATIC_DRAW;
	}
}