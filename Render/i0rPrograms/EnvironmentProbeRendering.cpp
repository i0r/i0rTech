#include <i0rCore/Common.hpp>
#include "../EnvironmentProbe.hpp"
#include <i0rRender/Mesh.hpp>
#include <i0rRender/Lod.hpp>
#include <i0rRender/Camera.hpp>
#include "Skydome.hpp"
#include "EnvironmentProbeRendering.hpp"

EnvironmentProbeRendering::EnvironmentProbeRendering() : m_FrameBuffer( nullptr ), m_Stencil( nullptr ), m_Direction( -1.0f ) {

}

EnvironmentProbeRendering::~EnvironmentProbeRendering() {
	m_Direction = 0.0f;

	m_FrameBuffer->~IFrameBuffer();
	RENDER_ALLOCATED_FREE( m_FrameBuffer );
	m_FrameBuffer = nullptr;

	m_Stencil->~Itexture_t();
	RENDER_ALLOCATED_FREE( m_Stencil );
	m_Stencil = nullptr;
}

bool EnvironmentProbeRendering::Initialize() {
	m_FrameBuffer = Instance.GraphicsApiContext->CreateFrameBuffer( ENVIRONMENT_PROBE_SIZE, ENVIRONMENT_PROBE_SIZE );

	m_Stencil = Instance.GraphicsApiContext->CreateTexture();
	m_Stencil->IsManaged = false;
	m_Stencil->CreateTexture2D( { GENERIC_RENDER_DEPTH24_STENCIL8, GENERIC_RENDER_DEPTH_STENCIL, GENERIC_RENDER_R8G8B8A8, GENERIC_RENDER_CLAMP_TO_EDGE, 
								                       0, GENERIC_RENDER_LINEAR, GENERIC_RENDER_LINEAR }, ENVIRONMENT_PROBE_SIZE, ENVIRONMENT_PROBE_SIZE );

	return true;
}

void EnvironmentProbeRendering::DrawSingleMesh( Mesh* meshData ) {
	meshData->m_Material->ShaderEnvProbe->Bind();
	meshData->m_Material->ShaderEnvProbe->SetAttribute4x4fv( "uModel",      meshData->m_ModelMatrix );
	meshData->m_Material->ShaderEnvProbe->SetAttribute4x4fv( "uView",       m_ViewMatrix );
	meshData->m_Material->ShaderEnvProbe->SetAttribute1i( "uMaterialIndex", meshData->m_Material->Index );
	meshData->m_Material->ShaderEnvProbe->SetAttribute1fv( "uDirection",    &m_Direction );

	for( std::pair<u32, Itexture_t*> layer : meshData->m_Material->Layers ) {
		if( layer.second ) layer.second->Bind( TEXTURE_BIND_ACTIVE_MATERIAL_BASE + layer.first );
	}

	meshData->GetLods()[meshData->GetLods().size() - 1]->Draw();
}

void EnvironmentProbeRendering::RenderProbe( environment_probe_t* probe ) {
	Instance.GraphicsApiContext->UpdateViewport( 0, 0, ENVIRONMENT_PROBE_SIZE, ENVIRONMENT_PROBE_SIZE );
	const Camera* currentCamera = Instance.SceneManager->GetActiveCamera();
	m_FrameBuffer->Bind( GENERIC_RENDER_FRAMEBUFFER_RW );

	mat4x4f curVP = Common.ViewProjection;
	mat4x4f proj  = Perspective( 90.0f, 1.0f, 0.1f, 1200.0f );
	for( i32 i = 0; i < 2; ++i ) {
		m_FrameBuffer->TemporaryAttach2D( ( !i ) ? probe->FrontHemisphere->Index : probe->RearHemisphere->Index, m_Stencil->Index );
		Instance.GraphicsApiContext->ClearBuffer( GENERIC_RENDER_COLOR_BUFFER_BIT | GENERIC_RENDER_DEPTH_BUFFER_BIT
								                                  | GENERIC_RENDER_STENCIL_BUFFER_BIT );

		m_Direction = ( !i ) ? -1.0f : 1.0f;
		m_ViewMatrix = LookAt( *currentCamera->GetCenter(), ( *currentCamera->GetCenter() + vec3f( 0.0f, 0.0f, m_Direction ) ), vec3f( 0.0f, 1.0f, 0.0f ) );
		Common.ViewProjection = proj * m_ViewMatrix;
		Instance.GraphicsApiContext->UploadUBO( &Common, sizeof( Common ), uboCommon );

		Instance.GraphicsApiContext->SetStencilFunction( GENERIC_RENDER_ALWAYS, 1, 0xFF );
		Instance.GraphicsApiContext->SetStencilOperation( GENERIC_RENDER_STENCIL_OP_REPLACE, GENERIC_RENDER_STENCIL_OP_REPLACE, GENERIC_RENDER_STENCIL_OP_REPLACE );

		for( Mesh* m : Instance.SceneManager->GetMeshes() ) {
			DrawSingleMesh( m );
		}

		Instance.GraphicsApiContext->SetStencilFunction( GENERIC_RENDER_NOT_EQUAL, 1, 0xFF );
		Instance.GraphicsApiContext->SetStencilOperation( GENERIC_RENDER_STENCIL_OP_KEEP, GENERIC_RENDER_STENCIL_OP_KEEP, GENERIC_RENDER_STENCIL_OP_KEEP );

		Instance.GraphicsApiContext->EnableDepthTest( GENERIC_RENDER_FALSE );
		Instance.Renderer->GetSkydomeProgram()->Render();
		Instance.GraphicsApiContext->EnableDepthTest( GENERIC_RENDER_TRUE );

			( ( !i ) ? probe->FrontHemisphere : probe->RearHemisphere )->Bind( GENERIC_RENDER_TEXTURE_SLOT_0 );

		Instance.GraphicsApiContext->GenerateMipMap( GENERIC_RENDER_TEXTURE_2D );// cheap mip generation
		// DumpToFile( (i32)m_FrameBuffer->GetFrameWidth(), (i32)m_FrameBuffer->GetFrameHeight(), ( !i ) ? "front" : "rear", true );
	}

	Common.ViewProjection = curVP;
	Instance.GraphicsApiContext->UploadUBO( &Common, sizeof( Common ), uboCommon );
	Instance.GraphicsApiContext->BindBackBuffer();
	Instance.GraphicsApiContext->UpdateViewport( 0, 
								                                      0, 
								                                      Instance.Configuration->Display.WindowWidth, 
								                                      Instance.Configuration->Display.WindowHeight );
}

void EnvironmentProbeRendering::BuildSpecular() {
	Itexture_t* specularCube = Instance.GraphicsApiContext->CreateTexture();
}
