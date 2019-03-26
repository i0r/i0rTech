#include "Common.hpp"
#include "../Font.hpp"
#include <stdarg.h>
#include "Text.hpp"

	Text::Text() : m_Shader( NULL ) {
		memset( m_FinalTextBuffer, 0, TEXT_MAX_LENGTH * sizeof( char ) );
	}

	Text::~Text() {
		memset( m_FinalTextBuffer, 0, TEXT_MAX_LENGTH * sizeof( char ) );
		m_Shader = NULL;

		#ifdef FLAG_DEBUG
			SAFE_DELETE( m_FontDebug )
		#endif
	}

	bool Text::Initialize() {
		m_Shader = Instance.ShaderManager->GetShader( "DebugText", SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT );

		if( !m_Shader ) {
			return false;
		}

		m_Shader->Bind();
	
		m_Shader->AddAttribute( "uVertVector4" );
		m_Shader->AddAttribute( "uFragVector4" );
		m_Shader->AddAttribute( "uFragVector4_1" );
		m_Shader->AddAttribute( "uFragVector4_2" );
		m_Shader->AddAttribute( "uSampler2D" );
	
		m_Shader->SetConstantAttribute1i( "uSampler2D", 2 );
		
		#ifdef FLAG_DEBUG
			m_FontDebug = new font_t();

			if (!m_FontDebug->Load( ( fontFolder + "default" + fontExtension ).c_str() ) ) {
				return false;
			}
		#endif

		return true;
	}

void Text::Compute( font_t* font, const i32 x, const i32 y, colorrgbaf textColor, colorrgbaf outlineColor, 
								            const f32 &fontSize, const char* text, ... ) {
		memset( m_FinalTextBuffer, 0, TEXT_MAX_LENGTH * sizeof( char ) );

		va_list  argList = {};
		va_start( argList, text );
		const i32 textLength = vsnprintf_s( m_FinalTextBuffer, TEXT_MAX_LENGTH, TEXT_MAX_LENGTH - 1, text, argList );
		va_end( argList );

		renderable_t* rObject = new renderable_t();

		rObject->Type   = RENDERABLE_TYPE_TEXT;
		rObject->Object = new renderable_string_t();
		( (renderable_string_t*)rObject->Object )->Font         = font;
		( (renderable_string_t*)rObject->Object )->TextColor    = textColor;
		( (renderable_string_t*)rObject->Object )->OutlineColor = outlineColor;
		( (renderable_string_t*)rObject->Object )->TextLength   = textLength;
		( (renderable_string_t*)rObject->Object )->Characters   = new renderable_char_t[textLength];

		static vec4f vertex = vec4f( 0.0f );
	
		f32 fontSizeX = 2.00f 
			* static_cast<f32>( font->Infos.glyphWidth * fontSize ) 
			/ Instance.Configuration->Display.WindowWidth,
		fontSizeY = 2.00f 
			* static_cast<f32>( font->Infos.glyphHeight * fontSize ) 
			/ Instance.Configuration->Display.WindowHeight;

		for( i32 i = 0, ly = 0, lx = 0; i < textLength; ++i ) {
			ly = m_FinalTextBuffer[i] / ( font->Infos.width 
				/ font->Infos.glyphWidth );
			lx = m_FinalTextBuffer[i] % ( font->Infos.width 
				/ font->Infos.glyphWidth );
		
			vertex.x = static_cast<f32>( font->Infos.glyphWidth * lx )  
				/ font->Infos.width;
			vertex.y = static_cast<f32>( font->Infos.glyphHeight * ly ) 
				/ font->Infos.height;
			vertex.z = static_cast<f32>( font->Infos.glyphWidth ) 
				/ font->Infos.width;
			vertex.w = static_cast<f32>( font->Infos.glyphHeight ) 
				/ font->Infos.height;

			( (renderable_string_t*)rObject->Object )->Characters[i].T1 = vertex;

			vertex[0] = i * fontSizeX - 1.0f + 2.0f * x 
				/ Instance.Configuration->Display.WindowWidth;
			vertex[1] = -2.0f * y 
				/ Instance.Configuration->Display.WindowHeight + 1.0f
				- fontSizeY;
			vertex[2] = fontSizeX;
			vertex[3] = fontSizeY;

			( (renderable_string_t*)rObject->Object )->Characters[i].T2 = vertex;
		}

		Instance.Renderer->PushRenderableInterface( rObject );
	}

void Text::Print( renderable_t* textString ) {
	Instance.GraphicsApiContext->EnableBackBufferDepthWrite( GENERIC_RENDER_FALSE );
	Instance.GraphicsApiContext->SetFaceCulling( GENERIC_RENDER_FRONT );
	Instance.GraphicsApiContext->EnableBlending( GENERIC_RENDER_TRUE );

	Quad::VAO->Bind();
	m_Shader->Bind();

	m_Shader->SetAttribute4fv( "uFragVector4_1", ( (renderable_string_t*)textString->Object )->TextColor );
	m_Shader->SetAttribute4fv( "uFragVector4_2", ( (renderable_string_t*)textString->Object )->OutlineColor );

	( (renderable_string_t*)textString->Object )->Font->Atlas->Bind( TEXTURE_BIND_ACTIVE_FONT );

	for( i32 i = 0; i < ( (renderable_string_t*)textString->Object )->TextLength; ++i ) {
		m_Shader->SetAttribute4fv( "uFragVector4", ( (renderable_string_t*)textString->Object )->Characters[i].T1 );
		m_Shader->SetAttribute4fv( "uVertVector4", ( (renderable_string_t*)textString->Object )->Characters[i].T2 );
		Instance.GraphicsApiContext->DrawIndicelessBuffer( GENERIC_RENDER_TRIANGLE, 0, 6 );
	}

	Quad::VAO->Unbind();
	Instance.GraphicsApiContext->EnableBlending( GENERIC_RENDER_FALSE );
	Instance.GraphicsApiContext->SetFaceCulling( GENERIC_RENDER_BACK );
	Instance.GraphicsApiContext->EnableBackBufferDepthWrite( GENERIC_RENDER_TRUE );

	delete (renderable_string_t*)textString->Object;
}
