#pragma once

/*=====================================================================
glVertexBuffer
	VertexBufferObject (OpenGL)
=====================================================================*/
class VertexBuffer : public IVertexBuffer {
	DECLARE_MEMBER( private, GLuint, Object )

	public:
		explicit VertexBuffer() ;
		~VertexBuffer() ;
	
		void AddElement( vertex_buffer_element_t data ) ;
		void BufferElements( const i32 usage ) ;
		void FastBufferElements( const i32 usage, f32* buffer, u32 bufferSize, std::vector<u32> strides ) ;

	private:
		void SetupAttributes() ;
};
