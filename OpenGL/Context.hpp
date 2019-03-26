#pragma once

#pragma comment(lib, "opengl32.lib")
#include <GL/gl.h>

#include <i0rRender/IGraphicsApiContext.hpp>

class OpenGL : public IGraphicsApiContext {
	public:
		OpenGL() ;
		void Shutdown( IRenderWindow* renderWindow ) ;
		bool Initialize( IRenderWindow* renderWindow, instances_t* &instances ) ;
		
		void BindBackBuffer() const ;

		INLINE void UnbindShaderProgram() const  {
			glUseProgram( 0 );
		}

		void EnableBackBufferDepthWrite( const i32 state ) const ;
		void EnableDepthTest( const i32 state ) const ;

		void CreateUBO( void* data, const i32 &size, void* &index ) ;
		void UploadUBO( void* data, const i32 &size, void* &index ) ;
		void BindUBO( void* &index, const u32 &bindPoint ) ;
		void FreeUBO( void* &index ) ;

		IIndiceBuffer* CreateIndiceBuffer() ;
		IVertexBuffer* CreateVertexBuffer() ;
		IFrameBuffer*  CreateFrameBuffer( const i32 width, const i32 height ) ;
		Itexture_t*    CreateTexture() ;

		void DeleteIndiceBuffer( IIndiceBuffer* &buffer ) ;
		void DeleteVertexBuffer( IVertexBuffer* &buffer ) ;

		void ClearBuffer( const i32 clearFlags ) const ;
		void UpdateViewport( const i32 x, const i32 y, const i32 width, const i32 height ) const ;
		void SetRasterizationMode( const i32 flag ) const ;
		void SetFaceCulling( const i32 state ) const ;
		void EnableBlending( const i32 state ) const ;
		void SetBlending( const i32 source, const i32 dest ) const ;
		void SetFrontFace( const i32 state ) const ;
		void SetStencilFunction( const i32 func, const i32 ref, const i32 mask ) const ;
		void SetStencilOperation( const i32 stencilFail, const i32 depthFail, const i32 pass ) const ;
		void DrawIndicelessBuffer( const i32 primitive, const i32 first, const i32 count ) const ;
		void SetMemoryBarrier( const i32 flags ) const ;
		void BindImageTexture( const u32 unit, const u32 texture, const i32 level, const bool layered, const i32 layer,
								                 const u32 access, const u32 format ) const ;
		void DispatchCompute( const u32 groupX, const u32 groupY, const u32 groupZ ) const ;
		void GenerateMipMap( const u32 target ) const ;
};
