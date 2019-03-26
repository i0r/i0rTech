#include "../Common.hpp"
#include "Composition.hpp"

IShader* Composition::Shader_      = nullptr;

bool EnableFXAA       = true;
bool EnableVignette   = false;
bool EnableTonemap    = true;

f32  VignetteStrength = 0.1f;
i32  ExposureFactor   = 16;
f32  ExposureBias     = 2.0f;

bool Composition::Initialize() {
	Instance.DebugMenu->RegisterBoolean( &EnableFXAA, "Enable FXAA", "\\Rendering\\Post Processing", std::bind( &Composition::CreateShader ) );

	Instance.DebugMenu->RegisterBoolean( &EnableVignette, "Enable Vignette", "\\Rendering\\Post Processing", std::bind( &Composition::CreateShader ) );
	Instance.DebugMenu->RegisterFloat( &VignetteStrength, 0.1f, 0.0f, 1.0f, "Vignette Strength", "\\Rendering\\Post Processing", std::bind( &Composition::CreateShader ) );

	Instance.DebugMenu->RegisterBoolean( &EnableTonemap, "Enable Tonemapping", "\\Rendering\\Post Processing", std::bind( &Composition::CreateShader ) );
	Instance.DebugMenu->RegisterInteger( &ExposureFactor, 1, 1, 32, "Tonemapping exposure factor", "\\Rendering\\Post Processing", std::bind( &Composition::CreateShader ) );
	Instance.DebugMenu->RegisterFloat( &ExposureBias, 0.5f, 0.0f, 16.0f, "Tonemapping exposure bias", "\\Rendering\\Post Processing", std::bind( &Composition::CreateShader ) );
	
	CreateShader();
	return true;
}

void Composition::Render( IFrameBuffer* fbo ) {
	Instance.GraphicsApiContext->EnableBackBufferDepthWrite( GENERIC_RENDER_FALSE );

	Shader_->Bind();

	ScreenQuad::VAO->Bind();
	ScreenQuad::VAO->Draw();
	ScreenQuad::VAO->Unbind();

	Instance.GraphicsApiContext->UnbindShaderProgram();
	Instance.GraphicsApiContext->EnableBackBufferDepthWrite( GENERIC_RENDER_TRUE );
}

void Composition::CreateShader() {
	std::vector<std::string> shaderFlags = {};

	if( EnableFXAA )     shaderFlags.push_back( "FXAA" );

	if( EnableVignette ) {
		shaderFlags.push_back( "VIGNETTING" );
		shaderFlags.push_back( "VIGNETTE_STRENGTH " + std::to_string( VignetteStrength ) );
	}

	if( EnableTonemap ) {
		shaderFlags.push_back( "TONEMAPPING" );
		shaderFlags.push_back( "TONEMAPPING_EXPOSURE " + std::to_string( ExposureFactor ) );
		shaderFlags.push_back( "TONEMAPPING_EXPOSURE_BIAS " + std::to_string( ExposureBias ) );
	}

	Shader_ = Instance.ShaderManager->GetShader( "Composition", SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT, shaderFlags );

	if( !Shader_ ) {
		return;
	}

	Shader_->Bind();

	Shader_->AddAttribute( "uSampler" );
	Shader_->SetConstantAttribute1i( "uSampler", 1 );

	Shader_->AddUniformBlock( "ubo_common_t" );
	Shader_->SetConstantUniformBufferIndex( "ubo_common_t", UBO_COMMON );

}