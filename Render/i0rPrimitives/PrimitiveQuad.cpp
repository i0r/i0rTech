#include "../Common.hpp"
#include "PrimitiveQuad.hpp"

IIndiceBuffer*  Quad::VAO  = nullptr;
IVertexBuffer*  Quad::VBO  = nullptr;

void Quad::Create() {
	const vertex_buffer_element_t vertex = {
		2,
		{
			0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		}
	},
		texCoords = {
		2,
		{
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		}
	};

	const std::vector<u32> indices = { 0, 1, 2, 2, 3, 0, };

	VAO = Instance.GraphicsApiContext->CreateIndiceBuffer();
	VBO = Instance.GraphicsApiContext->CreateVertexBuffer();

	VAO->Bind();

	VBO->AddElement( vertex );
	VBO->AddElement( texCoords );
	VBO->BufferElements( GENERIC_RENDER_STATIC_DRAW );

	VAO->BufferElements( indices, GENERIC_RENDER_STATIC_DRAW );

	VAO->Unbind();
}
