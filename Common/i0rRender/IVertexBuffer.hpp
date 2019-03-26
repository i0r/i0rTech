#pragma once

/*=====================================================================
vertex_buffer_element_t
	'Raw' data that will be used in a VBO
	Can be destroyed after the VBO is built and the data is successfully
	sent to the GPU
=====================================================================*/
struct vertex_buffer_element_t {
	u32                  Stride;
	std::vector<f32>     Data;
};

/*=====================================================================
IVertexBuffer
	VertexBufferObject Interface
=====================================================================*/
class IVertexBuffer {
	DECLARE_MEMBER( protected, i32,                                  Stride )
	DECLARE_MEMBER( protected, std::vector<vertex_buffer_element_t>, Elements )

	public:
		virtual ~IVertexBuffer() {
			m_Stride = 0;

			for( vertex_buffer_element_t &vertexBufferElement : m_Elements ) {
				vertexBufferElement.Data.clear();
				vertexBufferElement.Stride = 0;
			}

			m_Elements.clear();
		};
	
		virtual void AddElement( vertex_buffer_element_t data )                                                    = 0;
		virtual void BufferElements( const i32 usage )                                                             = 0;
		virtual void FastBufferElements( const i32 usage, f32* buffer, u32 bufferSize, std::vector<u32> strides )  = 0;
};
