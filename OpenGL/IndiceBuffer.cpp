#include "Common.hpp"
#include "IndiceBuffer.hpp"

IndiceBuffer::IndiceBuffer()  : m_Object ( 0 ),
								                                m_Indices( 0 ) {
	m_Size = 0;
	glGenVertexArrays( 1, &m_Object );
	glGenBuffers( 1, &m_Indices );
}

IndiceBuffer::~IndiceBuffer() {
	glDeleteBuffers( 1, &m_Object );
	glDeleteBuffers( 1, &m_Indices );

	m_Object  = 0;
	m_Indices = 0;
}

/*=====================================================================
BufferElements
	Uploads VAO CPU data to the GPU
=====================================================================*/
void IndiceBuffer::BufferElements( std::vector<u32> data, const i32 usage ) {
	m_Size = (GLsizei)data.size() * sizeof( u32 );
	
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_Indices );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_Size, &data[0], OpenGLDrawType( usage ) );
}

/*=====================================================================
Draw
	Draws geometry
=====================================================================*/
void IndiceBuffer::Draw() const  {
	glDrawElements( GL_TRIANGLES, m_Size, GL_UNSIGNED_INT, (const GLvoid*)nullptr );
}

/*=====================================================================
Bind
	Binds VAO
=====================================================================*/
void IndiceBuffer::Bind() const  {
	if( m_Object ) {
		glBindVertexArray( m_Object );
	} else {
		CONSOLE_PRINT_WARNING( "VertexArray::Bind => Tried to bind an undefined VAO\n" );
	}
}


/*=====================================================================
Unbind
	Unbinds VAO
=====================================================================*/
void IndiceBuffer::Unbind() const  {
	glBindVertexArray( 0 );
}

