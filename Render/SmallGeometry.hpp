#pragma once

#include <i0rCore/FSmallGeometry.hpp>

class IIndiceBuffer;
class IVertexBuffer;

struct smallgeometry_t {
	explicit smallgeometry_t() {
		m_VAO = Instance.GraphicsApiContext->CreateIndiceBuffer();
		m_VBO = Instance.GraphicsApiContext->CreateVertexBuffer();
	}

	~smallgeometry_t() {
		m_VAO->~IIndiceBuffer();
		RENDER_ALLOCATED_FREE( m_VAO );
		m_VAO = nullptr;

		m_VBO->~IVertexBuffer();
		RENDER_ALLOCATED_FREE( m_VBO );
		m_VBO = nullptr;
	}

	void Draw() const  {
		m_VAO->Bind();
		m_VAO->Draw();
		m_VAO->Unbind();
	}

	void Build( file_small_geometry_t* file ) {
		f32* vboData = new f32[file->Infos.VboSize];
		memcpy( vboData, file->BaseAddress, file->Infos.VboSize );
		file->BaseAddress += file->Infos.VboSize;

		std::vector<u32> indices( (u32*)file->BaseAddress, (u32*)( file->BaseAddress + file->Infos.VaoSize ) );

		file->BaseAddress += file->Infos.VaoSize;

		m_VAO->Bind();
		m_VBO->FastBufferElements( GENERIC_RENDER_STATIC_DRAW, vboData, file->Infos.VboSize, { 3, 2 } );
		m_VAO->BufferElements( indices, GENERIC_RENDER_STATIC_DRAW );
		m_VAO->Unbind();

		delete[] vboData;
	}

	IVertexBuffer*                  m_VBO;
	IIndiceBuffer*                  m_VAO;
};
