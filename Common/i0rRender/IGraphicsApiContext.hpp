#pragma once

class IRenderWindow;
class IIndiceBuffer;
class IVertexBuffer;
class IFrameBuffer;

class IGraphicsApiContext {
	DECLARE_MEMBER( protected, IShaderManager*,  ShaderManager )
	DECLARE_MEMBER( protected, ITextureManager*, TextureManager )

	public:
		virtual void Shutdown( IRenderWindow* renderWindow )                                               = 0;
		virtual bool Initialize( IRenderWindow* renderWindow, instances_t* &instances )                    = 0;

		virtual void BindBackBuffer() const                                                                = 0;
		virtual void UnbindShaderProgram() const                                                           = 0;
		virtual void EnableBackBufferDepthWrite( const i32 state ) const                                   = 0;
		virtual void EnableDepthTest( const i32 state ) const                                              = 0;

		virtual void CreateUBO( void* data, const i32 &size, void* &index )                                = 0;
		virtual void UploadUBO( void* data, const i32 &size, void* &index )                                = 0;
		virtual void BindUBO( void* &index, const u32 &bindPoint )                                         = 0;
		virtual void FreeUBO( void* &index )                                                               = 0;

		virtual IIndiceBuffer* CreateIndiceBuffer()                                                        = 0;
		virtual IVertexBuffer* CreateVertexBuffer()                                                        = 0;
		virtual IFrameBuffer*  CreateFrameBuffer( const i32 width, const i32 height )                      = 0;
		virtual Itexture_t*    CreateTexture()                                                             = 0;

		virtual void DeleteIndiceBuffer( IIndiceBuffer* &buffer )  = 0;
		virtual void DeleteVertexBuffer( IVertexBuffer* &buffer )  = 0;

		virtual void ClearBuffer( const i32 clearFlags ) const                                             = 0;
		virtual void UpdateViewport( const i32 x, const i32 y, const i32 width, const i32 height ) const   = 0;
		virtual void SetRasterizationMode( const i32 flag ) const                                          = 0;
		virtual void SetFaceCulling( const i32 state ) const                                               = 0;
		virtual void EnableBlending( const i32 state ) const                                               = 0;
		virtual void SetBlending( const i32 source, const i32 dest ) const                                 = 0;
		virtual void SetFrontFace( const i32 state ) const                                                 = 0;

		virtual void SetStencilFunction( const i32 func, const i32 ref, const i32 mask ) const                = 0;
		virtual void SetStencilOperation( const i32 stencilFail, const i32 depthFail, const i32 pass ) const  = 0;

		virtual void DrawIndicelessBuffer( const i32 primitive, const i32 first, const i32 count ) const  = 0;

		virtual void SetMemoryBarrier( const i32 flags ) const  = 0;
		virtual void BindImageTexture( const u32 unit, const u32 texture, const i32 level, const bool layered, const i32 layer,
								                         const u32 access, const u32 format ) const  = 0;
		virtual void DispatchCompute( const u32 groupX, const u32 groupY, const u32 groupZ ) const  = 0;
		virtual void GenerateMipMap( const u32 target ) const  = 0;
};
