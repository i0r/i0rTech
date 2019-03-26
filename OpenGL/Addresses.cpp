#include <i0rCore/Common.hpp>
#include "Addresses.hpp"

PFNGLATTACHSHADERPROC pglAttachShader = NULL;
PFNGLBINDBUFFERPROC pglBindBuffer = NULL;
PFNGLBINDVERTEXARRAYPROC pglBindVertexArray = NULL;
PFNGLBUFFERDATAPROC pglBufferData = NULL;
PFNGLCOMPILESHADERPROC pglCompileShader = NULL;
PFNGLCREATEPROGRAMPROC pglCreateProgram = NULL;
PFNGLCREATESHADERPROC pglCreateShader = NULL;
PFNGLDELETEBUFFERSPROC pglDeleteBuffers = NULL;
PFNGLDELETEPROGRAMPROC pglDeleteProgram = NULL;
PFNGLDELETESHADERPROC pglDeleteShader = NULL;
PFNGLDELETEVERTEXARRAYSPROC pglDeleteVertexArrays = NULL;
PFNGLDETACHSHADERPROC pglDetachShader = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC pglEnableVertexAttribArray = NULL;
PFNGLGENBUFFERSPROC pglGenBuffers = NULL;
PFNGLGENVERTEXARRAYSPROC pglGenVertexArrays = NULL;
PFNGLGETATTRIBLOCATIONPROC pglGetAttribLocation = NULL;
PFNGLGETPROGRAMINFOLOGPROC pglGetProgramInfoLog = NULL;
PFNGLGETPROGRAMIVPROC pglGetProgramiv = NULL;
PFNGLGETSHADERINFOLOGPROC pglGetShaderInfoLog = NULL;
PFNGLGETSHADERIVPROC pglGetShaderiv = NULL;
PFNGLLINKPROGRAMPROC pglLinkProgram = NULL;
PFNGLSHADERSOURCEPROC pglShaderSource = NULL;
PFNGLUSEPROGRAMPROC pglUseProgram = NULL;
PFNGLVERTEXATTRIBPOINTERPROC pglVertexAttribPointer = NULL;
PFNGLBINDATTRIBLOCATIONPROC pglBindAttribLocation = NULL;
PFNGLGETUNIFORMLOCATIONPROC pglGetUniformLocation = NULL;
PFNGLUNIFORMMATRIX2FVPROC pglUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX3FVPROC pglUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX4FVPROC pglUniformMatrix4fv = NULL;
PFNGLACTIVETEXTUREPROC pglActiveTexture = NULL;
PFNGLUNIFORM1IPROC pglUniform1i = NULL;
PFNGLUNIFORM1FPROC pglUniform1f = NULL;
PFNGLUNIFORM1IVPROC pglUniform1iv = NULL;
PFNGLUNIFORM1FVPROC pglUniform1fv = NULL;
PFNGLUNIFORM2IPROC pglUniform2i = NULL;
PFNGLUNIFORM2FPROC pglUniform2f = NULL;
PFNGLUNIFORM2IVPROC pglUniform2iv = NULL;
PFNGLUNIFORM2FVPROC pglUniform2fv = NULL;
PFNGLGENERATEMIPMAPPROC pglGenerateMipmap = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC pglDisableVertexAttribArray = NULL;
PFNGLUNIFORM3IPROC pglUniform3i = NULL;
PFNGLUNIFORM3FPROC pglUniform3f = NULL;
PFNGLUNIFORM3IVPROC pglUniform3iv = NULL;
PFNGLUNIFORM3FVPROC pglUniform3fv = NULL;
PFNGLUNIFORM4IPROC pglUniform4i = NULL;
PFNGLUNIFORM4FPROC pglUniform4f = NULL;
PFNGLUNIFORM4IVPROC pglUniform4iv = NULL;
PFNGLUNIFORM4FVPROC pglUniform4fv = NULL;
PFNGLGENFRAMEBUFFERSPROC pglGenFramebuffers = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC pglFramebufferTexture2D = NULL;
PFNGLDELETEFRAMEBUFFERSPROC pglDeleteFramebuffers = NULL;
PFNGLDRAWBUFFERSPROC pglDrawBuffers = NULL;
PFNGLBINDFRAMEBUFFERPROC pglBindFramebuffer = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC pglCheckFramebufferStatus = NULL;
PFNGLMAPBUFFERARBPROC pglMapBuffer = NULL;
PFNGLUNMAPBUFFERARBPROC pglUnmapBuffer = NULL;
PFNGLBINDBUFFERBASE pglBindBufferBase = NULL;
PFNGLGETUNIFORMBLOCKINDEXPROC pglGetUniformBlockIndex = NULL;
PFNGLUNIFORMBLOCKBINDINGPROC pglUniformBlockBinding = NULL;
PFNGLBINDIMAGETEXTUREEXTPROC pglBindImageTexture = NULL;
PFNGLMEMORYBARRIERPROC pglMemoryBarrier = NULL;
PFNGLTEXBUFFERPROC pglTexBuffer = NULL;
PFNGLGETBUFFERSUBDATA pglGetBufferSubData = NULL;
PFNGLBLITFRAMEBUFFERPROC pglBlitFramebuffer = NULL;
PFNGLDISPATCHCOMPUTEPROC pglDispatchCompute = NULL;
PFNGLGETPROGRAMRESOURCEINDEXPROC pglGetProgramResourceIndex = NULL;
PFNGLSHADERSTORAGEBLOCKBINDINGPROC pglShaderStorageBlockBinding = NULL;
PFNGLFRAMEBUFFERTEXTURELAYERPROC pglFramebufferTextureLayer = NULL;
PFNGLBLENDEQUATIONPROC pglBlendEquation = NULL;
PFNGLFRAMEBUFFERTEXTURE3D pglFrameBufferTexture3D = NULL;
PFNGLTEXIMAGE3DPROC pglTexImage3D = NULL;
PFNGLFRAMEBUFFERTEXTUREPROC pglFramebufferTexture = NULL;
PFNGLBLENDEQUATIONSEPARATEPROC pglBlendEquationSeparate = NULL;
PFNGLBLENDFUNCSEPARATEPROC pglBlendFuncSeparate = NULL;
PFNGLFRAMEBUFFERPARAMETERIPROC pglFramebufferParameteri = NULL;
PFNGLTEXSTORAGE3DPROC pglTexStorage3D = NULL;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
PFNGLBUFFERSTORAGEPROC pglBufferStorage = NULL;
PFNGLMAPBUFFERRANGEPROC pglMapBufferRange = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DPROC pglCompressedTexImage2D = NULL;
PFNGLDEBUGMESSAGECALLBACKPROC pglDebugMessageCallback = NULL;

#define OPENGL_GET_ADDRESS( x ) wglGetProcAddress( x );
#define OPENGL_LOAD_FUNC( x ) ::x = (PFN##x)OPENGL_GET_ADDRESS( (char*)x ); if( !::x ) return false;

bool OpenGLAddresses::LoadExtensionList() {
	// OPENGL_LOAD_FUNC( wglChoosePixelFormatARB )

	#if defined( FLAG_WINDOWS )
	::wglChoosePixelFormatARB =
		(PFNWGLCHOOSEPIXELFORMATARBPROC)
		OPENGL_GET_ADDRESS( "wglChoosePixelFormatARB" );
	if( !::wglChoosePixelFormatARB ) {
		return false;
	}

	::wglCreateContextAttribsARB =
		(PFNWGLCREATECONTEXTATTRIBSARBPROC)
		OPENGL_GET_ADDRESS( "wglCreateContextAttribsARB" );
	if( !::wglCreateContextAttribsARB ) {
		return false;
	}

	::wglSwapIntervalEXT =
		(PFNWGLSWAPINTERVALEXTPROC)
		OPENGL_GET_ADDRESS( "wglSwapIntervalEXT" );
	if( !::wglSwapIntervalEXT ) {
		return false;
	}

	::glActiveTexture =
		(PFNGLACTIVETEXTUREPROC)
		OPENGL_GET_ADDRESS( "glActiveTexture" );
	if( !::glActiveTexture ) {
		return false;
	}
	::glCompressedTexImage2D =
		(PFNGLCOMPRESSEDTEXIMAGE2DPROC)
		OPENGL_GET_ADDRESS( "glCompressedTexImage2D" );
	if( !::glCompressedTexImage2D ) {
		return false;
	}

	::glBufferStorage =
		(PFNGLBUFFERSTORAGEPROC)
		OPENGL_GET_ADDRESS( "glBufferStorage" );
	if( !::glBufferStorage ) {
		return false;
	}

	::glMapBufferRange =
		(PFNGLMAPBUFFERRANGEPROC)
		OPENGL_GET_ADDRESS( "glMapBufferRange" );
	if( !::glMapBufferRange ) {
		return false;
	}

	#elif defined( FLAG_LINUX )
	::glXSwapIntervalEXT =
		(PFNGLXSWAPINTERVALEXTPROC)
		OPENGL_GET_ADDRESS( "glXSwapIntervalEXT" );
	if( !::glXSwapIntervalEXT ) {
		return false;
	}
	#endif

	::glAttachShader =
		(PFNGLATTACHSHADERPROC)
		OPENGL_GET_ADDRESS( "glAttachShader" );
	if( !::glAttachShader ) {
		return false;
	}

	::glBindBuffer =
		(PFNGLBINDBUFFERPROC)
		OPENGL_GET_ADDRESS( "glBindBuffer" );
	if( !::glBindBuffer ) {
		return false;
	}

	::glBindVertexArray =
		(PFNGLBINDVERTEXARRAYPROC)
		OPENGL_GET_ADDRESS( "glBindVertexArray" );
	if( !::glBindVertexArray ) {
		return false;
	}

	::glBufferData =
		(PFNGLBUFFERDATAPROC)
		OPENGL_GET_ADDRESS( "glBufferData" );
	if( !::glBufferData ) {
		return false;
	}

	::glCompileShader =
		(PFNGLCOMPILESHADERPROC)
		OPENGL_GET_ADDRESS( "glCompileShader" );
	if( !::glCompileShader ) {
		return false;
	}

	::glCreateProgram =
		(PFNGLCREATEPROGRAMPROC)
		OPENGL_GET_ADDRESS( "glCreateProgram" );
	if( !::glCreateProgram ) {
		return false;
	}

	::glCreateShader =
		(PFNGLCREATESHADERPROC)
		OPENGL_GET_ADDRESS( "glCreateShader" );
	if( !::glCreateShader ) {
		return false;
	}

	::glDeleteBuffers =
		(PFNGLDELETEBUFFERSPROC)
		OPENGL_GET_ADDRESS( "glDeleteBuffers" );
	if( !::glDeleteBuffers ) {
		return false;
	}

	::glDeleteProgram =
		(PFNGLDELETEPROGRAMPROC)
		OPENGL_GET_ADDRESS( "glDeleteProgram" );
	if( !::glDeleteProgram ) {
		return false;
	}

	::glDeleteShader =
		(PFNGLDELETESHADERPROC)
		OPENGL_GET_ADDRESS( "glDeleteShader" );
	if( !::glDeleteShader ) {
		return false;
	}

	::glDeleteVertexArrays =
		(PFNGLDELETEVERTEXARRAYSPROC)
		OPENGL_GET_ADDRESS( "glDeleteVertexArrays" );
	if( !::glDeleteVertexArrays ) {
		return false;
	}

	::glDetachShader =
		(PFNGLDETACHSHADERPROC)
		OPENGL_GET_ADDRESS( "glDetachShader" );
	if( !::glDetachShader ) {
		return false;
	}

	::glEnableVertexAttribArray =
		(PFNGLENABLEVERTEXATTRIBARRAYPROC)
		OPENGL_GET_ADDRESS( "glEnableVertexAttribArray" );
	if( !::glEnableVertexAttribArray ) {
		return false;
	}

	::glGenBuffers =
		(PFNGLGENBUFFERSPROC)
		OPENGL_GET_ADDRESS( "glGenBuffers" );
	if( !::glGenBuffers ) {
		return false;
	}

	::glGenVertexArrays =
		(PFNGLGENVERTEXARRAYSPROC)
		OPENGL_GET_ADDRESS( "glGenVertexArrays" );
	if( !::glGenVertexArrays ) {
		return false;
	}

	::glGetAttribLocation =
		(PFNGLGETATTRIBLOCATIONPROC)
		OPENGL_GET_ADDRESS( "glGetAttribLocation" );
	if( !::glGetAttribLocation ) {
		return false;
	}

	::glGetProgramInfoLog =
		(PFNGLGETPROGRAMINFOLOGPROC)
		OPENGL_GET_ADDRESS( "glGetProgramInfoLog" );
	if( !::glGetProgramInfoLog ) {
		return false;
	}

	::glGetProgramiv =
		(PFNGLGETPROGRAMIVPROC)
		OPENGL_GET_ADDRESS( "glGetProgramiv" );
	if( !::glGetProgramiv ) {
		return false;
	}

	::glGetShaderInfoLog =
		(PFNGLGETSHADERINFOLOGPROC)
		OPENGL_GET_ADDRESS( "glGetShaderInfoLog" );
	if( !::glGetShaderInfoLog ) {
		return false;
	}

	::glGetShaderiv =
		(PFNGLGETSHADERIVPROC)
		OPENGL_GET_ADDRESS( "glGetShaderiv" );
	if( !::glGetShaderiv ) {
		return false;
	}

	::glLinkProgram =
		(PFNGLLINKPROGRAMPROC)
		OPENGL_GET_ADDRESS( "glLinkProgram" );
	if( !::glLinkProgram ) {
		return false;
	}

	::glShaderSource =
		(PFNGLSHADERSOURCEPROC)
		OPENGL_GET_ADDRESS( "glShaderSource" );
	if( !::glShaderSource ) {
		return false;
	}

	::glUseProgram =
		(PFNGLUSEPROGRAMPROC)
		OPENGL_GET_ADDRESS( "glUseProgram" );
	if( !::glUseProgram ) {
		return false;
	}

	::glVertexAttribPointer =
		(PFNGLVERTEXATTRIBPOINTERPROC)
		OPENGL_GET_ADDRESS( "glVertexAttribPointer" );
	if( !::glVertexAttribPointer ) {
		return false;
	}

	::glBindAttribLocation =
		(PFNGLBINDATTRIBLOCATIONPROC)
		OPENGL_GET_ADDRESS( "glBindAttribLocation" );
	if( !::glBindAttribLocation ) {
		return false;
	}

	::glGetUniformLocation =
		(PFNGLGETUNIFORMLOCATIONPROC)
		OPENGL_GET_ADDRESS( "glGetUniformLocation" );
	if( !::glGetUniformLocation ) {
		return false;
	}

	::glUniformMatrix2fv =
		(PFNGLUNIFORMMATRIX2FVPROC)
		OPENGL_GET_ADDRESS( "glUniformMatrix2fv" );
	if( !::glUniformMatrix2fv ) {
		return false;
	}

	::glUniformMatrix3fv =
		(PFNGLUNIFORMMATRIX3FVPROC)
		OPENGL_GET_ADDRESS( "glUniformMatrix3fv" );
	if( !::glUniformMatrix3fv ) {
		return false;
	}

	::glUniformMatrix4fv =
		(PFNGLUNIFORMMATRIX4FVPROC)
		OPENGL_GET_ADDRESS( "glUniformMatrix4fv" );
	if( !::glUniformMatrix4fv ) {
		return false;
	}

	::glUniform1i =
		(PFNGLUNIFORM1IPROC)
		OPENGL_GET_ADDRESS( "glUniform1i" );
	if( !::glUniform1i ) {
		return false;
	}

	::glUniform1f =
		(PFNGLUNIFORM1FPROC)
		OPENGL_GET_ADDRESS( "glUniform1f" );
	if( !::glUniform1f ) {
		return false;
	}

	::glUniform1iv =
		(PFNGLUNIFORM1IVPROC)
		OPENGL_GET_ADDRESS( "glUniform1iv" );
	if( !::glUniform1iv ) {
		return false;
	}

	::glUniform1fv =
		(PFNGLUNIFORM1FVPROC)
		OPENGL_GET_ADDRESS( "glUniform1fv" );
	if( !::glUniform1fv ) {
		return false;
	}

	::glGenerateMipmap =
		(PFNGLGENERATEMIPMAPPROC)
		OPENGL_GET_ADDRESS( "glGenerateMipmap" );
	if( !::glGenerateMipmap ) {
		return false;
	}

	::glDisableVertexAttribArray =
		(PFNGLDISABLEVERTEXATTRIBARRAYPROC)
		OPENGL_GET_ADDRESS( "glDisableVertexAttribArray" );
	if( !::glDisableVertexAttribArray ) {
		return false;
	}

	glUniform2fv =
		(PFNGLUNIFORM2FVPROC)
		OPENGL_GET_ADDRESS( "glUniform2fv" );
	if( !::glUniform2fv ) {
		return false;
	}

	glUniform2iv =
		(PFNGLUNIFORM2IVPROC)
		OPENGL_GET_ADDRESS( "glUniform2iv" );
	if( !::glUniform2iv ) {
		return false;
	}

	glUniform2f =
		(PFNGLUNIFORM2FPROC)
		OPENGL_GET_ADDRESS( "glUniform2f" );
	if( !::glUniform2f ) {
		return false;
	}

	glUniform2i =
		(PFNGLUNIFORM2IPROC)
		OPENGL_GET_ADDRESS( "glUniform2i" );
	if( !::glUniform2i ) {
		return false;
	}

	glUniform3fv =
		(PFNGLUNIFORM3FVPROC)
		OPENGL_GET_ADDRESS( "glUniform3fv" );
	if( !::glUniform3fv ) {
		return false;
	}

	glUniform3iv =
		(PFNGLUNIFORM3IVPROC)
		OPENGL_GET_ADDRESS( "glUniform3iv" );
	if( !::glUniform3iv ) {
		return false;
	}

	glUniform3f =
		(PFNGLUNIFORM3FPROC)
		OPENGL_GET_ADDRESS( "glUniform3f" );
	if( !::glUniform3f ) {
		return false;
	}

	glUniform3i =
		(PFNGLUNIFORM3IPROC)
		OPENGL_GET_ADDRESS( "glUniform3i" );
	if( !::glUniform3i ) {
		return false;
	}

	::glUniform4fv =
		(PFNGLUNIFORM4FVPROC)
		OPENGL_GET_ADDRESS( "glUniform4fv" );
	if( !::glUniform4fv ) {
		return false;
	}

	::glUniform4f =
		(PFNGLUNIFORM4FPROC)
		OPENGL_GET_ADDRESS( "glUniform4f" );
	if( !::glUniform4f ) {
		return false;
	}

	::glUniform4i =
		(PFNGLUNIFORM4IPROC)
		OPENGL_GET_ADDRESS( "glUniform4i" );
	if( !::glUniform4i ) {
		return false;
	}

	::glUniform4iv =
		(PFNGLUNIFORM4IVPROC)
		OPENGL_GET_ADDRESS( "glUniform4iv" );
	if( !::glUniform4iv ) {
		return false;
	}

	::glGenFramebuffers =
		(PFNGLGENFRAMEBUFFERSPROC)
		OPENGL_GET_ADDRESS( "glGenFramebuffers" );
	if( !::glGenFramebuffers ) {
		return false;
	}

	::glFramebufferTexture2D =
		(PFNGLFRAMEBUFFERTEXTURE2DPROC)
		OPENGL_GET_ADDRESS( "glFramebufferTexture2D" );
	if( !::glFramebufferTexture2D ) {
		return false;
	}

	::glDeleteFramebuffers =
		(PFNGLDELETEFRAMEBUFFERSPROC)
		OPENGL_GET_ADDRESS( "glDeleteFramebuffers" );
	if( !::glDeleteFramebuffers ) {
		return false;
	}

	::glCheckFramebufferStatus =
		(PFNGLCHECKFRAMEBUFFERSTATUSPROC)
		OPENGL_GET_ADDRESS( "glCheckFramebufferStatus" );
	if( !::glCheckFramebufferStatus ) {
		return false;
	}

	::glDrawBuffers =
		(PFNGLDRAWBUFFERSPROC)
		OPENGL_GET_ADDRESS( "glDrawBuffers" );
	if( !::glDrawBuffers ) {
		return false;
	}

	::glBindFramebuffer =
		(PFNGLBINDFRAMEBUFFERPROC)
		OPENGL_GET_ADDRESS( "glBindFramebuffer" );
	if( !::glBindFramebuffer ) {
		return false;
	}

	::glMapBuffer =
		(PFNGLMAPBUFFERARBPROC)
		OPENGL_GET_ADDRESS( "glMapBuffer" );
	if( !::glMapBuffer ) {
		return false;
	}

	::glUnmapBuffer =
		(PFNGLUNMAPBUFFERARBPROC)
		OPENGL_GET_ADDRESS( "glUnmapBuffer" );
	if( !::glUnmapBuffer ) {
		return false;
	}

	::glBindBufferBase =
		(PFNGLBINDBUFFERBASE)
		OPENGL_GET_ADDRESS( "glBindBufferBase" );
	if( !::glBindBufferBase ) {
		return false;
	}

	::glGetUniformBlockIndex =
		(PFNGLGETUNIFORMBLOCKINDEXPROC)
		OPENGL_GET_ADDRESS( "glGetUniformBlockIndex" );
	if( !::glGetUniformBlockIndex ) {
		return false;
	}

	::glUniformBlockBinding =
		(PFNGLUNIFORMBLOCKBINDINGPROC)
		OPENGL_GET_ADDRESS( "glUniformBlockBinding" );
	if( !::glUniformBlockBinding ) {
		return false;
	}

	::glBindImageTexture =
		(PFNGLBINDIMAGETEXTUREEXTPROC)
		OPENGL_GET_ADDRESS( "glBindImageTexture" );
	if( !::glBindImageTexture ) {
		return false;
	}

	::glTexBuffer =
		(PFNGLTEXBUFFERPROC)
		OPENGL_GET_ADDRESS( "glTexBuffer" );
	if( !::glTexBuffer ) {
		return false;
	}

	::glMemoryBarrier =
		(PFNGLMEMORYBARRIERPROC)
		OPENGL_GET_ADDRESS( "glMemoryBarrier" );
	if( !::glMemoryBarrier ) {
		return false;
	}

	::glGetBufferSubData =
		(PFNGLGETBUFFERSUBDATA)
		OPENGL_GET_ADDRESS( "glGetBufferSubData" );
	if( !::glGetBufferSubData ) {
		return false;
	}

	::glBlitFramebuffer =
		(PFNGLBLITFRAMEBUFFERPROC)
		OPENGL_GET_ADDRESS( "glBlitFramebuffer" );
	if( !::glBlitFramebuffer ) {
		return false;
	}

	::glDispatchCompute =
		(PFNGLDISPATCHCOMPUTEPROC)
		OPENGL_GET_ADDRESS( "glDispatchCompute" );
	if( !::glDispatchCompute ) {
		return false;
	}

	::glGetProgramResourceIndex =
		(PFNGLGETPROGRAMRESOURCEINDEXPROC)
		OPENGL_GET_ADDRESS( "glGetProgramResourceIndex" );
	if( !::glGetProgramResourceIndex ) {
		return false;
	}

	::glShaderStorageBlockBinding =
		(PFNGLSHADERSTORAGEBLOCKBINDINGPROC)
		OPENGL_GET_ADDRESS( "glShaderStorageBlockBinding" );
	if( !::glShaderStorageBlockBinding ) {
		return false;
	}

	::glFramebufferTextureLayer =
		(PFNGLFRAMEBUFFERTEXTURELAYERPROC)
		OPENGL_GET_ADDRESS( "glFramebufferTextureLayer" );
	if( !::glFramebufferTextureLayer ) {
		return false;
	}

	::glBlendEquation =
		(PFNGLBLENDEQUATIONPROC)
		OPENGL_GET_ADDRESS( "glBlendEquation" );
	if( !::glBlendEquation ) {
		return false;
	}

	::glFramebufferTexture3D =
		(PFNGLFRAMEBUFFERTEXTURE3D)
		OPENGL_GET_ADDRESS( "glFramebufferTexture3D" );
	if( !::glFramebufferTexture3D ) {
		return false;
	}

	::glTexImage3D =
		(PFNGLTEXIMAGE3DPROC)
		OPENGL_GET_ADDRESS( "glTexImage3D" );
	if( !::glTexImage3D ) {
		return false;
	}

	::glFramebufferTexture =
		(PFNGLFRAMEBUFFERTEXTUREPROC)
		OPENGL_GET_ADDRESS( "glFramebufferTexture" );
	if( !::glFramebufferTexture ) {
		return false;
	}

	::glBlendEquationSeparate =
		(PFNGLBLENDEQUATIONSEPARATEPROC)
		OPENGL_GET_ADDRESS( "glBlendEquationSeparate" );
	if( !::glBlendEquationSeparate ) {
		return false;
	}

	::glBlendFuncSeparate =
		(PFNGLBLENDFUNCSEPARATEPROC)
		OPENGL_GET_ADDRESS( "glBlendFuncSeparate" );
	if( !::glBlendFuncSeparate ) {
		return false;
	}

	::glFramebufferParameteri = (PFNGLFRAMEBUFFERPARAMETERIPROC)
		OPENGL_GET_ADDRESS( "glFramebufferParameteri" );
	if( !::glFramebufferParameteri ) {
		return false;
	}

	::glTexStorage3D = (PFNGLTEXSTORAGE3DPROC)
		OPENGL_GET_ADDRESS( "glTexStorage3D" );
	if( !::glTexStorage3D ) {
		return false;
	}

	::glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)
		OPENGL_GET_ADDRESS( "glDebugMessageCallback" );
	if( !::glDebugMessageCallback ) {
		return false;
	}

	return true;
}
