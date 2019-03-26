#pragma once

#include <i0rRender/IFrameBuffer.hpp>

struct framebuffer_attachement_t {
	u32 Attachement;
	bool   IsShared;
};

class FrameBuffer : public IFrameBuffer {
	DECLARE_MEMBER( protected, u32,                                 Object )
	DECLARE_MEMBER( private,   framebuffer_attachement_t,              DepthAttachement )
	DECLARE_MEMBER( private,   std::vector<framebuffer_attachement_t>, Attachements )

	public:
		explicit FrameBuffer( const GLsizei frameWidth, const GLsizei frameHeight ) ;
		explicit FrameBuffer() ;
		~FrameBuffer() ;

		bool Build() ;

		void TemporaryAttach2D( const u32 attachement, const u32 stencil = 0 ) ;

		void AddAttachement2D( const i32 internalFormat, const i32 format, const i32 type, 
								                 const i32 textureWrapping, const void* data = nullptr, const i32 border = 0 ) ;
		void AddAttachementDepthArray2D( const i32 depthInternalFormat, const i32 format, const i32 depthType,
								                           const i32 textureWrapping, const i32 layerCount, const void* data = nullptr,
								                           const i32 border = 0 ) ;
		void AddAttachementDepth( const i32 depthInternalFormat, const i32 depthType, const i32 textureWrapping ) ;
		void BindAttachement( const u32 attachementIndex, const i32 attachementSlot = GL_TEXTURE0 ) ;
		void BindDepthAttachement( const i32 attachementSlot = GL_TEXTURE0 ) ;

		void AttachDepth( const i32 depthAttachement ) ;

		void Attach2D( const u32 attachement ) ;
		void Attach3D( const u32 attachement ) ;
		void WriteBuffer( const std::vector<u32> indexes ) ;

		INLINE void Bind( const i32 mode = GL_FRAMEBUFFER ) {
			glBindFramebuffer( mode, m_Object );
		}

		INLINE void BindDepthBuffer( const u32 position, const i32 mode = GL_FRAMEBUFFER ) {
			Bind( mode );
			glFramebufferTextureLayer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthAttachement.Attachement, 0, position );
		}

		INLINE void BindWriteDepthLayer( const u32 pos, const u32 depthMap ) {
			glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_Object );
			glFramebufferTextureLayer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0, pos );
		}

		INLINE void ReadBuffer( const u32 index ) {
			glReadBuffer( GL_COLOR_ATTACHMENT0 + index );
		}

		INLINE void WriteBuffer( const u32 index ) {
			glDrawBuffer( GL_COLOR_ATTACHMENT0 + index );
		}

		INLINE u32 GetAttachementByIndex( const u32 index ) {
			return ( index >= 0 && index < m_Attachements.size() ) ? m_Attachements.at( index ).Attachement : 0;
		}
			
		INLINE void BindTexture( const u32 index, const i32 bindPoint = GL_TEXTURE0 ) {
			glActiveTexture( bindPoint );
			glBindTexture( GL_TEXTURE_2D, ( index >= 0 && index < m_Attachements.size() ) 
								                         ? m_Attachements.at( index ).Attachement : 0 );
		}

		void BindDepthWrite( const u32 pos ) ;
		void BindDepthMap( const u32 pos ) ;
}; // class FrameBuffer
