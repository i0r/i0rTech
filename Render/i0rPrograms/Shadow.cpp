#include "../Common.hpp"
#include <i0rRender/Mesh.hpp>
#include <i0rRender/Lod.hpp>
#include "Shadow.hpp"

Shadow::Shadow(): m_OrthoNear( 1.0f ), m_OrthoFar( 16.0f ) {

}

Shadow::~Shadow() {

}

bool Shadow::Initialize() {
	m_Shader = Instance.ShaderManager->GetShader( "DepthDebug", SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT );

	m_Shader->Bind();

	m_Shader->AddAttribute( "uDepthSampler" );
	m_Shader->SetConstantAttribute1i( "uDepthSampler", 7 );

	return m_Shader != nullptr;
}

void Shadow::RenderScene( mat4x4f viewProjection ) {
	m_ViewProjection = viewProjection;

	for( Mesh* m : Instance.SceneManager->GetMeshes() ) {
		DrawSingleMesh( m );
	}

	Instance.GraphicsApiContext->UnbindShaderProgram();
}

void Shadow::DrawSingleMesh( Mesh* meshData ) {
	meshData->m_Material->ShaderDepth->Bind();
	meshData->m_Material->ShaderDepth->SetAttribute4x4fv( "uViewProjection", m_ViewProjection );
	meshData->m_Material->ShaderDepth->SetAttribute4x4fv( "uModel", meshData->m_ModelMatrix );

	meshData->GetLods()[0]->Draw();
}
