#include "Common.hpp"
#include <i0rRender/Mesh.hpp>
#include <i0rRender/Lod.hpp>
#include "DeferredTiled.hpp"

	DeferredTiled::DeferredTiled() : m_FrameBuffer( nullptr ), m_Shader( nullptr ) {

	}

	DeferredTiled::~DeferredTiled() {
		m_FrameBuffer->~IFrameBuffer();
		RENDER_ALLOCATED_FREE( m_FrameBuffer );
		m_FrameBuffer = nullptr;

		m_Shader = nullptr;
	}

bool DeferredTiled::Initialize() {
	if( !OnResize() ) {
		return false;
	}

	Instance.GraphicsApiContext->BindBackBuffer();

	m_Shader = Instance.ShaderManager->GetShader( "LightCulling", SHADER_STAGE_COMPUTE );

	if( !m_Shader ) {
		return false;
	}

	m_Shader->Bind();

	m_Shader->AddAttribute( "uFrontParaboloid" );
	m_Shader->SetConstantAttribute1i( "uFrontParaboloid", 5 );

	m_Shader->AddAttribute( "uRearParaboloid" );
	m_Shader->SetConstantAttribute1i( "uRearParaboloid", 6 );

	m_Shader->AddAttribute( "uSSAOSampler" );
	m_Shader->SetConstantAttribute1i( "uSSAOSampler", 14 );

	m_Shader->AddUniformBlock( "ubo_light_t" );
	m_Shader->SetConstantUniformBufferIndex( "ubo_light_t", UBO_LIGHTS );
		
	m_Shader->AddUniformBlock( "ubo_common_t" );
	m_Shader->SetConstantUniformBufferIndex( "ubo_common_t", UBO_COMMON );

	m_Shader->AddUniformBlock( "ubo_materials_t" );
	m_Shader->SetConstantUniformBufferIndex( "ubo_materials_t", UBO_MATERIALS ); 

	m_Shader->AddUniformBlock( "ubo_sky_t" );
	m_Shader->SetConstantUniformBufferIndex( "ubo_sky_t", UBO_SKY );

	m_Shader->AddUniformBlock( "ubo_shadows_t" );
	m_Shader->SetConstantUniformBufferIndex( "ubo_shadows_t", UBO_SHADOWS );

	return true;
}

bool DeferredTiled::OnResize() {
	if( m_FrameBuffer ) {
		m_FrameBuffer->~IFrameBuffer();
		RENDER_ALLOCATED_FREE( m_FrameBuffer );
		m_FrameBuffer = nullptr;
	}

	m_FrameBuffer = Instance.GraphicsApiContext->CreateFrameBuffer( Instance.Configuration->Display.WindowWidth, 
								                                                         Instance.Configuration->Display.WindowHeight );
	m_FrameBuffer->Bind( GENERIC_RENDER_FRAMEBUFFER_RW );
	m_FrameBuffer->AddAttachement2D( GENERIC_RENDER_RGBA32F, GENERIC_RENDER_RGBA, GENERIC_RENDER_FLOAT, GENERIC_RENDER_CLAMP_TO_EDGE ); // ALBEDO + METALNESS
	m_FrameBuffer->AddAttachement2D( GENERIC_RENDER_RGBA32F, GENERIC_RENDER_RGBA, GENERIC_RENDER_FLOAT, GENERIC_RENDER_CLAMP_TO_EDGE ); // COMPRESSED NORMALS + DEPTH
	m_FrameBuffer->AddAttachement2D( GENERIC_RENDER_RGBA32F, GENERIC_RENDER_RGBA, GENERIC_RENDER_FLOAT, GENERIC_RENDER_CLAMP_TO_EDGE ); // POSITION + MAT ID
	m_FrameBuffer->AddAttachement2D( GENERIC_RENDER_RGBA32F, GENERIC_RENDER_RGBA, GENERIC_RENDER_FLOAT, GENERIC_RENDER_CLAMP_TO_EDGE ); // ROUGHNESS

	m_FrameBuffer->AddAttachement2D( GENERIC_RENDER_RGBA32F, GENERIC_RENDER_RGBA, GENERIC_RENDER_FLOAT, GENERIC_RENDER_CLAMP_TO_EDGE ); // DECALS

	m_FrameBuffer->AttachDepth( Instance.Renderer->GetStencil()->Index );

	if( !m_FrameBuffer->Build() ) {
		CONSOLE_PRINT_ERROR("DeferredTiled::Initialize => Failed to build GBuffer!\n");
		return false;
	}

	return true;
}

void DeferredTiled::DrawSingle(renderable_t* mesh) {
	const Mesh* meshData = (Mesh*)( mesh->Object );

	meshData->m_Material->ShaderDefault->Bind();
	meshData->m_Material->ShaderDefault->SetAttribute4x4fv( "uModel", meshData->m_ModelMatrix );
	meshData->m_Material->ShaderDefault->SetAttribute1i( "uMaterialIndex", meshData->m_Material->Index );

	for( std::pair<u32, Itexture_t*> layer : meshData->m_Material->Layers ) {
		if( layer.second ) layer.second->Bind( TEXTURE_BIND_ACTIVE_MATERIAL_BASE + layer.first );
	}

	const f32 lodDistance = abs( floor( Distance3D( Common.ViewPosition, meshData->m_BoundingBox.WorldPosition ) ) );

	for( lod_t* lod : meshData->GetLods() ) {
		if( lod->LOD >= lodDistance ) {
			lod->Draw();
			return;
		}
	}

	// if there is no lod available, pick the cheapest one
	meshData->GetLods()[meshData->GetLods().size() - 1]->Draw();
}

void DeferredTiled::StartGeometryPass() {
	Instance.GraphicsApiContext->SetStencilFunction( GENERIC_RENDER_ALWAYS, 1, 0xFF );
	Instance.GraphicsApiContext->SetStencilOperation( GENERIC_RENDER_STENCIL_OP_REPLACE, GENERIC_RENDER_STENCIL_OP_REPLACE, GENERIC_RENDER_STENCIL_OP_REPLACE );

	m_FrameBuffer->Bind( GENERIC_RENDER_FRAMEBUFFER_RW );
	Instance.GraphicsApiContext->ClearBuffer( GENERIC_RENDER_COLOR_BUFFER_BIT | GENERIC_RENDER_DEPTH_BUFFER_BIT | GENERIC_RENDER_STENCIL_BUFFER_BIT );

	Instance.GraphicsApiContext->SetFrontFace( GENERIC_RENDER_CLOCKWISE );
}

void DeferredTiled::Render() {
	Instance.GraphicsApiContext->SetFrontFace( GENERIC_RENDER_COUNTER_CLOCKWISE );

	// post-lighting pass goes here

	m_Shader->Bind();

	Instance.Renderer->GetFrameBuffer()->Bind( GENERIC_RENDER_FRAMEBUFFER_DRAW );
	Instance.GraphicsApiContext->ClearBuffer( GENERIC_RENDER_COLOR_BUFFER_BIT | GENERIC_RENDER_DEPTH_BUFFER_BIT );

	Instance.GraphicsApiContext->BindImageTexture( 0, Instance.Renderer->GetFrameBuffer()->GetAttachementByIndex( 0 ), 0, GENERIC_RENDER_FALSE, 0, GENERIC_RENDER_WRITE_ONLY, GENERIC_RENDER_RGBA32F );

	Instance.GraphicsApiContext->BindImageTexture( 1, m_FrameBuffer->GetAttachementByIndex( 1 ), 0, GENERIC_RENDER_FALSE, 0, GENERIC_RENDER_READ_ONLY, GENERIC_RENDER_RGBA32F );
	Instance.GraphicsApiContext->BindImageTexture( 2, m_FrameBuffer->GetAttachementByIndex( 0 ), 0, GENERIC_RENDER_FALSE, 0, GENERIC_RENDER_READ_ONLY, GENERIC_RENDER_RGBA32F );
	Instance.GraphicsApiContext->BindImageTexture( 3, m_FrameBuffer->GetAttachementByIndex( 2 ), 0, GENERIC_RENDER_FALSE, 0, GENERIC_RENDER_READ_ONLY, GENERIC_RENDER_RGBA32F );
	Instance.GraphicsApiContext->BindImageTexture( 4, m_FrameBuffer->GetAttachementByIndex( 3 ), 0, GENERIC_RENDER_FALSE, 0, GENERIC_RENDER_READ_ONLY, GENERIC_RENDER_RGBA32F );

	Instance.GraphicsApiContext->DispatchCompute( (u32)ceil( Common.ScreenSize.x / WORK_GROUP_SIZE ), (u32)ceil( Common.ScreenSize.y / WORK_GROUP_SIZE ), 1 );
	Instance.GraphicsApiContext->SetMemoryBarrier( GENERIC_RENDER_TEXTURE_FETCH_BARRIER_BIT | GENERIC_RENDER_SHADER_IMAGE_ACCESS_BARRIER_BIT );

	Instance.GraphicsApiContext->SetStencilFunction( GENERIC_RENDER_NOT_EQUAL, 1, 0xFF );
	Instance.GraphicsApiContext->SetStencilOperation( GENERIC_RENDER_STENCIL_OP_KEEP, GENERIC_RENDER_STENCIL_OP_KEEP, GENERIC_RENDER_STENCIL_OP_KEEP );
}
