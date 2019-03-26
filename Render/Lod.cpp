#include "Common.hpp"
#include <i0rRender/Lod.hpp>

lod_t::lod_t()  : LOD( F32_MAX ) {
	VAO = Instance.GraphicsApiContext->CreateIndiceBuffer();
	VBO = Instance.GraphicsApiContext->CreateVertexBuffer();
}

lod_t::~lod_t() {
	LOD = 0.0f;

	VAO->~IIndiceBuffer();
	RENDER_ALLOCATED_FREE( VAO );
	VAO = nullptr;

	VBO->~IVertexBuffer();
	RENDER_ALLOCATED_FREE( VBO );
	VBO = nullptr;
}

void lod_t::Draw() const  {
	VAO->Bind();
	VAO->Draw();
	VAO->Unbind();
}
