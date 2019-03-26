#pragma once

/*=====================================================================
VertexArray
	VertexArrayObject (OpenGL)
=====================================================================*/
class IndiceBuffer : public IIndiceBuffer {
	DECLARE_MEMBER( private, GLuint,  Object )
	DECLARE_MEMBER( private, GLuint,  Indices )
	DECLARE_MEMBER( private, GLsizei, Size )

	public:
		explicit IndiceBuffer() ;
		~IndiceBuffer() ;

		void BufferElements( std::vector<u32> data, const i32 usage ) ;

		void Draw()   const ;
		void Bind()   const ;
		void Unbind() const ;
};
