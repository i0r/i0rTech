#include "../Common.hpp"
#include <i0rRender/AABB.hpp>
#include "AABBWireframe.hpp"

IShader* AABBWireframe::m_Shader = nullptr;

bool AABBWireframe::Initialize() {
	m_Shader = Instance.ShaderManager->GetShader( "DebugWireframe", SHADER_STAGE_VERTEX | SHADER_STAGE_FRAGMENT );

	if( !m_Shader ) {
		return false;
	}
	
	m_Shader->Bind();

	m_Shader->AddUniformBlock( "ubo_common_t" );
	m_Shader->SetConstantUniformBufferIndex( "ubo_common_t", UBO_COMMON );

	m_Shader->AddAttribute( "uPos" );
	m_Shader->AddAttribute( "uScale" );

	m_Shader->AddAttribute( "uWireColor" );
	m_Shader->SetAttribute4fv( "uWireColor", colorrgbaf( 1.0f, 0.0f, 0.0f, 1.0f ) );

	return true;
}

void AABBWireframe::Render( renderable_t* prim ) {
	#ifdef FLAG_DEBUG
		aabb_t* aabb = (aabb_t*)( prim->Object );

		m_Shader->Bind();

		m_Shader->SetAttribute4fv( "uWireColor", aabb->DebugColor );

		m_Shader->SetAttribute3fv( "uPos", aabb->WorldPosition );
		m_Shader->SetAttribute3fv( "uScale", aabb->Dimensions / 2.0f );

		Instance.GraphicsApiContext->SetRasterizationMode( GENERIC_RENDER_WIRE );

		Box::VAO->Bind();
		Box::VAO->Draw();
		Box::VAO->Unbind();

		Instance.GraphicsApiContext->SetRasterizationMode( GENERIC_RENDER_FILL );

		Instance.GraphicsApiContext->UnbindShaderProgram();
	#endif
}