#include "../Common.hpp"
#include "PrimitiveBox.hpp"

IIndiceBuffer* Box::VAO = nullptr;
IVertexBuffer* Box::VBO = nullptr;

void Box::Create() {
	const vertex_buffer_element_t vertex = {
		3,
		{
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,

			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,

			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,

			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
		}
	};

	const std::vector<u32> indices = { 
		0, 1, 2,
		2, 1, 3,

		4, 5, 6,
		6, 5, 7,

		8, 9, 10,
		10, 9, 11,

		12, 13, 14,
		14, 13, 15,

		16, 17, 18,
		18, 17, 19,

		20, 21, 22,
		22, 21, 23, 
	};

	VAO = Instance.GraphicsApiContext->CreateIndiceBuffer();
	VBO = Instance.GraphicsApiContext->CreateVertexBuffer();

	VAO->Bind();
			
	VBO->AddElement( vertex );
	//VBO->AddElement( texCoords );
	VBO->BufferElements( GENERIC_RENDER_STATIC_DRAW );

	VAO->BufferElements( indices, GENERIC_RENDER_STATIC_DRAW );

	VAO->Unbind();
}
