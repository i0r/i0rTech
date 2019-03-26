#include "Common.hpp"
#include "FrameBuffer.hpp"

FrameBuffer::FrameBuffer( const i32 frameWidth, const i32 frameHeight )  : m_Object( 0 ),
								                                                                           m_DepthAttachement( { 0, false } ) {
	m_FrameWidth  = frameWidth;
	m_FrameHeight = frameHeight;
	glGenFramebuffers( 1, &m_Object );
}

FrameBuffer::FrameBuffer()  : m_Object( 0 ),
								                              m_DepthAttachement( { 0, false } ) {
	m_FrameWidth  = 0;
	m_FrameHeight = 0;
}

FrameBuffer::~FrameBuffer() {
	if( m_DepthAttachement.Attachement && !m_DepthAttachement.IsShared ) {
		glDeleteTextures( 1, &m_DepthAttachement.Attachement );
		m_DepthAttachement.Attachement = 0;
	}

	for( framebuffer_attachement_t attachement : m_Attachements ) {
		if( !attachement.IsShared ) {
			glDeleteTextures( 1, &attachement.Attachement );
		}

		attachement.Attachement = 0;
	}
	
	m_Attachements.clear();

	glDeleteFramebuffers( 1, &m_Object );
	m_Object      = 0;
}

bool FrameBuffer::Build() {
	GLenum* buffers = new GLenum[m_Attachements.size()];

	for( u32 i = 0; i < m_Attachements.size(); ++i ) {
		buffers[i] = ( GL_COLOR_ATTACHMENT0 + i );
	}

	glDrawBuffers( (i32)m_Attachements.size(), buffers );

	delete[] buffers;

	return ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE );
}
	
void FrameBuffer::BindAttachement( const u32 attachementIndex, const i32 attachementSlot ) {
	if( attachementIndex >= m_Attachements.size() ) {
		CONSOLE_PRINT_ERROR( "FrameBuffer::BindAttachement => Attachement index %i out of bounds!\n", attachementIndex );
		return;
	}

	glActiveTexture( attachementSlot );
	glBindTexture( GL_TEXTURE_2D, m_Attachements[attachementIndex].Attachement );
}

void FrameBuffer::BindDepthAttachement( const i32 attachementSlot ) {
	if( !m_DepthAttachement.Attachement ) {
		CONSOLE_PRINT_ERROR( "FrameBuffer::BindDepthAttachement => Framebuffer %i have no depth buffer registered!\n", m_Object );
		return;
	}

	glActiveTexture( attachementSlot );
	glBindTexture( GL_TEXTURE_2D, m_DepthAttachement.Attachement );
}

void FrameBuffer::AttachDepth( const i32 depthAttachement ) {
	m_DepthAttachement.Attachement = depthAttachement;
	m_DepthAttachement.IsShared    = true;

	glBindTexture( GL_TEXTURE_2D, m_DepthAttachement.Attachement );
	const static GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachement.Attachement, 0 );
}

void FrameBuffer::TemporaryAttach2D( const GLuint attachement, const GLuint stencil ) {
	for( i32 i = 0; i < m_Attachements.size(); ++i ) {
		glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, 0, 0 );
	}

	m_Attachements.clear();

	if( m_Object ) {
		glDeleteFramebuffers( 1, &m_Object );
		m_Object = NULL;
	}

	glGenFramebuffers( 1, &m_Object );
	glBindFramebuffer( GL_FRAMEBUFFER, m_Object );

	if( stencil ) {
		AttachDepth( stencil );
	}

	glDrawBuffer( GL_COLOR_ATTACHMENT0 );

	GLenum buffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers( 1, buffers );

	glBindTexture( GL_TEXTURE_2D, attachement );

	framebuffer_attachement_t attach = {
		attachement,
		false
	};

	m_Attachements.push_back( attach );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + ( GLenum )m_Attachements.size() - 1, GL_TEXTURE_2D,
								                 attachement, 0 );
}

void FrameBuffer::Attach2D( const u32 attachement ) {
	const framebuffer_attachement_t attachement2D = { attachement, true };

	glBindTexture( GL_TEXTURE_2D, attachement2D.Attachement );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (i32)m_Attachements.size(), GL_TEXTURE_2D, 
								                 attachement2D.Attachement, 0 );

	m_Attachements.push_back( attachement2D );
}

void FrameBuffer::Attach3D( const u32 attachement ) {
	const framebuffer_attachement_t attachement3D = { attachement, true };

	glBindTexture( GL_TEXTURE_3D, attachement3D.Attachement );
	glFramebufferTexture3D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (i32)m_Attachements.size(), GL_TEXTURE_3D, 
								                 attachement3D.Attachement, 0, 0 );

	m_Attachements.push_back( attachement3D );
}

void FrameBuffer::AddAttachementDepth( const GLint depthInternalFormat, const i32 depthType,
								                               const GLint textureWrapping ) {
	if( m_DepthAttachement.Attachement ) {
		CONSOLE_PRINT_WARNING( "FrameBuffer::AddAttachementDepth => Framebuffer %i already have a depth buffer registered!\n", 
								                 m_Object );
		return;
	}

	glGenTextures( 1, &m_DepthAttachement.Attachement );
	glBindTexture( GL_TEXTURE_2D, m_DepthAttachement.Attachement );
	glTexImage2D( GL_TEXTURE_2D, 0, depthInternalFormat, m_FrameWidth, m_FrameHeight, 0, GL_DEPTH_COMPONENT, depthType, 0 );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapping );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapping );
	
	const static GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachement.Attachement, 0 );
}

void FrameBuffer::AddAttachement2D( const GLint internalFormat, const i32 format, const i32 type, const GLint textureWrapping, 
								                            const void* data, const GLint border ) {
	framebuffer_attachement_t attachement = { 0, false };

	glGenTextures( 1, &attachement.Attachement );
	glBindTexture( GL_TEXTURE_2D, attachement.Attachement );
	glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, m_FrameWidth, m_FrameHeight, border, format, type, data );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapping );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapping );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (i32)m_Attachements.size(), GL_TEXTURE_2D, 
								                 attachement.Attachement, 0 );

	m_Attachements.push_back( attachement );
}

void FrameBuffer::WriteBuffer( const std::vector<u32> indexes ) {
	GLenum* buffers = new GLenum[indexes.size()];

	for( u32 i = 0; i < indexes.size(); ++i ) {
		buffers[i] = ( GL_COLOR_ATTACHMENT0 + i );
	}

	glDrawBuffers( (i32)indexes.size(), buffers );

	delete[] buffers;
}

void FrameBuffer::BindDepthWrite( const u32 pos ) {
	Instance.GraphicsApiContext->UpdateViewport( 0, 0, m_FrameWidth, m_FrameHeight );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_Object );
	
	glFramebufferTextureLayer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, (GLuint)m_DepthAttachement.Attachement, 0, pos );
}

void FrameBuffer::BindDepthMap( const u32 pos ) {
	glActiveTexture( pos );
	glBindTexture( GL_TEXTURE_2D_ARRAY, (GLuint)m_DepthAttachement.Attachement );
}

void FrameBuffer::AddAttachementDepthArray2D( const i32 depthInternalFormat, const i32 format, const i32 depthType,
								                                      const i32 textureWrapping, const i32 layerCount, const void* data, 
								                                      const i32 border ) {
		m_DepthAttachement.IsShared = false;

		glGenTextures( 1, &m_DepthAttachement.Attachement );
		glBindTexture( GL_TEXTURE_2D_ARRAY, m_DepthAttachement.Attachement );
		glTexImage3D( GL_TEXTURE_2D_ARRAY, 0, depthInternalFormat, m_FrameWidth, m_FrameHeight, layerCount, 0,
								        format, depthType, data );
		glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, textureWrapping );
		glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, textureWrapping );
		glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, textureWrapping );
	
		glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
		glTexParameterf( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
	
		const f32 color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, color );
	
		glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthAttachement.Attachement, 0 );
	
		glDrawBuffer( GL_NONE );
		glReadBuffer( GL_NONE );                                        
}