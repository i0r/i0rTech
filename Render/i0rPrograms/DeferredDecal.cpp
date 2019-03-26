#include "../Common.hpp"
#include <i0rRender/Decal.hpp>
#include "DeferredTiled.hpp"
#include "DeferredDecal.hpp"

IShader* DeferredDecal::Shader_ = nullptr;

bool DeferredDecal::Initialize() {
	Shader_ = Instance.ShaderManager->GetShader( "Decal", SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT );

	if( !Shader_ ) {
		return false;
	}

	Shader_->Bind();

	Shader_->AddUniformBlock( "ubo_common_t" );
	Shader_->SetConstantUniformBufferIndex( "ubo_common_t", UBO_COMMON );
	
	Shader_->AddAttribute( "uModel" );
	Shader_->AddAttribute( "uInvModel" );

	Shader_->AddAttribute( "ugDiffuse" );
	Shader_->SetConstantAttribute1i( "ugDiffuse", 0 );

	Shader_->AddAttribute( "ugPosition" );
	Shader_->SetConstantAttribute1i( "ugPosition", 14 );

	Shader_->AddAttribute( "ugNormals" );
	Shader_->SetConstantAttribute1i( "ugNormals", 16 );

	Shader_->AddAttribute( "uDiffuseSampler" );
	Shader_->SetConstantAttribute1i( "uDiffuseSampler", 8 );

	Shader_->AddAttribute( "uNormalSampler" );
	Shader_->SetConstantAttribute1i( "uNormalSampler", 10 );

	return true;
}

void DeferredDecal::Render( renderable_t* decal ) {
	const decal_t* decalData = (decal_t*)( decal->Object );

	Instance.GraphicsApiContext->EnableBlending( GENERIC_RENDER_TRUE );
	Instance.GraphicsApiContext->SetBlending( GENERIC_RENDER_SOURCE_ALPHA, GENERIC_RENDER_ONE_MINUS_SOURCE_ALPHA );

	Shader_->Bind();
	Shader_->SetAttribute4x4fv( "uModel", decalData->ModelMatrix );

	mat4x4f invModel = Inverse( decalData->ModelMatrix );
	invModel.r3.y = invModel.r2.z;

	Shader_->SetAttribute4x4fv( "uInvModel", invModel );

	for( std::pair<u32, Itexture_t*> layer : decalData->Material->Layers ) {
		if( layer.second ) layer.second->Bind( TEXTURE_BIND_ACTIVE_MATERIAL_BASE + layer.first );
	}

	IFrameBuffer* gBuffer = Instance.Renderer->GetDeferredTiledProgram()->GetFrameBuffer();

	gBuffer->Bind( GENERIC_RENDER_FRAMEBUFFER_RW );
	gBuffer->BindTexture( 0, GENERIC_RENDER_TEXTURE_SLOT_0 );
	gBuffer->BindTexture( 2, GENERIC_RENDER_TEXTURE_SLOT_14 );
	gBuffer->BindTexture( 1, GENERIC_RENDER_TEXTURE_SLOT_16 );

	/* gBuffer->WriteBuffer( { 4 } );

	Box::VAO->Bind();
	Box::VAO->Draw();
	Box::VAO->Unbind(); */

	Instance.GraphicsApiContext->EnableBlending( GENERIC_RENDER_FALSE );

	gBuffer->Build();
}
