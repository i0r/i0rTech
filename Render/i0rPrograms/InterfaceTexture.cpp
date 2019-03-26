#include "../Common.hpp"
#include "InterfaceTexture.hpp"

IShader* InterfaceTexture::Shader_ = nullptr;

bool InterfaceTexture::Initialize() {
	Shader_ = Instance.ShaderManager->GetShader( "RenderUITexture", SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT, { "CUSTOM_SCREENPOS" } );

	if( !Shader_ ) {
		return false;
	}

	Shader_->Bind();

	Shader_->AddAttribute( "uSampler" );
	Shader_->SetConstantAttribute1i( "uSampler", 15 );

	Shader_->AddAttribute( "uScreenPosX" );
	Shader_->AddAttribute( "uScreenPosY" );

	Shader_->AddAttribute( "uScreenScaleX" );
	Shader_->AddAttribute( "uScreenScaleY" );

	return true;
}

void InterfaceTexture::Render( renderable_t* renderable ) {
	const renderable_ui_texture_t* obj = (renderable_ui_texture_t*)renderable->Object;
	obj->Texture->Bind( TEXTURE_BIND_ACTIVE_INTERFACE_TEXTURE );

	Instance.GraphicsApiContext->EnableBlending( GENERIC_RENDER_TRUE );
	Instance.GraphicsApiContext->SetBlending( GENERIC_RENDER_SOURCE_ALPHA, GENERIC_RENDER_ONE_MINUS_SOURCE_ALPHA );

	const f32 screenScaleX = (f32)obj->Width / Instance.Configuration->Display.WindowWidth;
	const f32 screenScaleY = (f32)obj->Height / Instance.Configuration->Display.WindowHeight;

	const f32 screenX = ( (f32)obj->X - Instance.Configuration->Display.WindowWidth / 2.0f  ) / ( obj->Width  / 2.0f );
	const f32 screenY = ( (f32)obj->Y - Instance.Configuration->Display.WindowHeight / 2.0f ) / ( obj->Height / 2.0f );

	Shader_->Bind();

	Shader_->SetAttribute1fv( "uScreenPosX", &screenX );
	Shader_->SetAttribute1fv( "uScreenPosY", &screenY );
	Shader_->SetAttribute1fv( "uScreenScaleX", &screenScaleX );
	Shader_->SetAttribute1fv( "uScreenScaleY", &screenScaleY );

	ScreenQuad::VAO->Bind();
	ScreenQuad::VAO->Draw();
	ScreenQuad::VAO->Unbind();

	Instance.GraphicsApiContext->EnableBlending( GENERIC_RENDER_FALSE );

	Instance.GraphicsApiContext->UnbindShaderProgram();
}
