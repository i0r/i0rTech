#pragma once

class IFrameBuffer {
	DECLARE_MEMBER( protected, i32, FrameWidth )
	DECLARE_MEMBER( protected, i32, FrameHeight )

	public:
		virtual ~IFrameBuffer() {
			m_FrameWidth = 0;
			m_FrameHeight = 0;
		};

		virtual bool Build()  = 0;
		virtual void TemporaryAttach2D( const u32 attachement, const u32 stencil = 0 )  = 0;
		virtual void AddAttachement2D( const i32 internalFormat, const i32 format, const i32 type,
								                         const i32 textureWrapping, const void* data = nullptr, 
								                         const i32 border = 0 )  = 0;
		virtual void AddAttachementDepthArray2D( const i32 depthInternalFormat, const i32 format, const i32 depthType,
								                         const i32 textureWrapping, const i32 layerCount, const void* data = nullptr,
								                         const i32 border = 0 )  = 0;
		
		virtual void AddAttachementDepth( const i32 depthInternalFormat, const i32 depthType, 
								                            const i32 textureWrapping )  = 0;

		virtual void AttachDepth( const i32 depthAttachement )  = 0;
		virtual void Attach2D( const u32 attachement )  = 0;
		virtual void Attach3D( const u32 attachement )  = 0;

		virtual void Bind( const i32 mode )  = 0;
		virtual void BindDepthBuffer( const u32 position, const i32 mode )  = 0;
		virtual void BindWriteDepthLayer( const u32 pos, const u32 depthMap )  = 0;
		virtual void BindTexture( const u32 index, const i32 bindPoint )  = 0;
		virtual void BindAttachement( const u32 attachementIndex, const i32 attachementSlot )  = 0;
		virtual void BindDepthAttachement( const i32 attachementSlot )  = 0;

		virtual void ReadBuffer( const u32 index )  = 0;

		virtual void WriteBuffer( const u32 index )  = 0;
		virtual void WriteBuffer( const std::vector<u32> indexes )  = 0;  
		
		virtual void BindDepthWrite( const u32 pos )  = 0;
		virtual void BindDepthMap( const u32 pos )  = 0;

		virtual u32 GetAttachementByIndex( const u32 index )  = 0;
}; // class FrameBuffer
