#include "Common.hpp"
#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer()  : m_Object( 0 ) {
	m_Stride = 0;
	glGenBuffers( 1, &m_Object );
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers( 1, &m_Object );

	m_Object = 0;
	m_Stride = 0;

	for( vertex_buffer_element_t &vertexBufferElement : m_Elements ) {
		vertexBufferElement.Data.clear();
		vertexBufferElement.Stride = 0;
	}

	m_Elements.clear();
}

/*=====================================================================
AddElement
	Appends a vertex_buffer_element_t to a VBO element list
=====================================================================*/
void VertexBuffer::AddElement( vertex_buffer_element_t data ) {
	m_Elements.push_back( data );
}

/*=====================================================================
FastBufferElements
	Builds a VBO with a pre-interleaved data array
=====================================================================*/
void VertexBuffer::FastBufferElements( const i32 usage, f32* buffer, u32 bufferSize,
								                               std::vector<u32> strides ) {
	glBindBuffer( GL_ARRAY_BUFFER, m_Object );
	glBufferData( GL_ARRAY_BUFFER, bufferSize, &buffer[0], OpenGLDrawType( usage ) );

	GLint attributeIndex = 0, 
							vboOffset      = 0;

	for( u32 &element : strides ) {
		m_Stride += element;
	}

	for( u32 &element : strides ) {
		glEnableVertexAttribArray( attributeIndex );
		glVertexAttribPointer( attributeIndex, element, GENERIC_RENDER_FLOAT, GENERIC_RENDER_FALSE, m_Stride * sizeof( GLfloat ),
								                 OPENGL_BUFFER_OFFSET( vboOffset ) );

		vboOffset += element * sizeof( GLfloat );
		attributeIndex++;
	}
}

/*=====================================================================
BufferElements
	Builds a VBO using the element list
	FastBufferElements should be used instead (since runtime buffering
	is slow)
=====================================================================*/
void VertexBuffer::BufferElements( const i32 usage ) {
	i32 vertexCount = 0, elementVertexCount = 0, arrayLength = 0;

	for( vertex_buffer_element_t &element : m_Elements ) {
		arrayLength += (i32)element.Data.size();
		m_Stride += element.Stride;

		if( !vertexCount ) {
			vertexCount = (i32)element.Data.size() / element.Stride;
			continue;
		}

		elementVertexCount = (i32)element.Data.size() / element.Stride;
	}

	GLfloat* finalBuffer   = new GLfloat[arrayLength];
	u32 finalBufferPointer = 0;

	for( i32 i = 0; i < vertexCount; i++ ) {
		for( vertex_buffer_element_t &element : m_Elements ) {
			for( u32 j = 0; j < element.Stride; j++ ) {
				finalBuffer[finalBufferPointer++] = 
					element.Data[i * element.Stride + j];
			}
		}   
	}

	glBindBuffer( GL_ARRAY_BUFFER, m_Object );
	glBufferData( GL_ARRAY_BUFFER, arrayLength * sizeof( GLfloat ), &finalBuffer[0], OpenGLDrawType( usage ) );
	SetupAttributes();

	delete[] finalBuffer;

	for( vertex_buffer_element_t &element : m_Elements ) {
		element.Data.clear();
		element.Stride = 0;
	}
}

/*=====================================================================
SetupAttributes
	Initializes VBO attributes
=====================================================================*/
void VertexBuffer::SetupAttributes() {
	GLint attributeIndex = 0, vboOffset = 0;

	for( vertex_buffer_element_t &element : m_Elements ) {
		glEnableVertexAttribArray( attributeIndex );
		glVertexAttribPointer( attributeIndex, element.Stride, GENERIC_RENDER_FLOAT, GENERIC_RENDER_FALSE, m_Stride * sizeof( GLfloat ), 
								                 OPENGL_BUFFER_OFFSET( vboOffset ) );
			
		vboOffset += element.Stride * sizeof( GLfloat );
		attributeIndex++;
	}
}
