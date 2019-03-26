#include "Common.hpp"

#include "i0rShaderManager/ShaderManager.hpp"
#include "i0rTextureManager/TextureManager.hpp"

#include "i0rTextureManager/Texture.hpp"

#include "VertexBuffer.hpp"
#include "IndiceBuffer.hpp"
#include "FrameBuffer.hpp"

#include "Context.hpp"

MODULE( OpenGL )

OpenGL::OpenGL() {
	m_ShaderManager  = nullptr;
	m_TextureManager = nullptr;
}

void OpenGL::Shutdown( IRenderWindow* window ) {
	SAFE_DELETE( m_ShaderManager )
	SAFE_DELETE( m_TextureManager )

	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( window->GetRenderingContext() );
}

#ifdef FLAG_WINDOWS
	LRESULT CALLBACK FakeWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
		return DefWindowProc( hwnd, msg, wParam, lParam );
	}
#endif

static void APIENTRY DebugCallback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, 
								                            const GLchar *message, const void *userParam ) {
	std::string srcStr = "???", typeStr = "???";

	if( source == GL_DEBUG_SOURCE_API )                  srcStr = "OpenGL";
	else if( source == GL_DEBUG_SOURCE_WINDOW_SYSTEM )   srcStr = "Window API";
	else if( source == GL_DEBUG_SOURCE_SHADER_COMPILER ) srcStr = "Shader Compiler";

	if( type == GL_DEBUG_TYPE_ERROR ) typeStr = "ERROR";
	else if( type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR ) typeStr = "DEPRECATED";
	else if( type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR ) typeStr = "UNDEFINED BEHAVIOR";
	else if( type == GL_DEBUG_TYPE_PORTABILITY ) typeStr = "UNPORTABLE";
	else if( type == GL_DEBUG_TYPE_PERFORMANCE ) typeStr = "PERFORMANCES";

	if( severity == GL_DEBUG_SEVERITY_NOTIFICATION || severity == GL_DEBUG_SEVERITY_LOW ) {
		CONSOLE_PRINT_INFO( "[%i] %s - %s : %s\n", id, srcStr.c_str(), typeStr.c_str(), message );
	} else if( severity == GL_DEBUG_SEVERITY_MEDIUM ) {
		CONSOLE_PRINT_WARNING( "[%i] %s - %s : %s\n", id, srcStr.c_str(), typeStr.c_str(), message );
	} else {
		CONSOLE_PRINT_ERROR( "[%i] %s - %s : %s\n", id, srcStr.c_str(), typeStr.c_str(), message );
	}
}

bool OpenGL::Initialize( IRenderWindow* renderWindow, instances_t* &instances ) {
	Instance = *instances;
	
	PIXELFORMATDESCRIPTOR pixelFormat = {};

	// fake window creation to test the environement
	WNDCLASSEX wc       = {};
	HINSTANCE hInstance = GetModuleHandle( NULL );

	wc.cbSize        = sizeof( WNDCLASSEX );
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = FakeWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon( hInstance, IDI_APPLICATION );
	wc.hIconSm       = LoadIcon( hInstance, IDI_APPLICATION );
	wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)( COLOR_MENUBAR + 1 );
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = "i0rTechFakeFrame";

	if( !RegisterClassEx( &wc ) ) {
		return false;
	}

	HWND fakeWindow = CreateWindow( "i0rTechFakeFrame", "i0rTechFakeWindow", WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, NULL, NULL, 
								                         hInstance, NULL );

	if( !fakeWindow ) {
		return false;
	}

	HDC fakeHdc = GetDC( fakeWindow );

	pixelFormat.nSize      = sizeof( PIXELFORMATDESCRIPTOR );
	pixelFormat.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormat.nVersion   = 1;
	pixelFormat.iPixelType = PFD_TYPE_RGBA;
	pixelFormat.cColorBits = 32;
	pixelFormat.cDepthBits = 32;
	pixelFormat.iLayerType = PFD_MAIN_PLANE;

	i32 pixelFormatResult  = ChoosePixelFormat( fakeHdc, &pixelFormat );

	if( !pixelFormatResult ) {
		return false;
	}

	if( !SetPixelFormat( fakeHdc, pixelFormatResult, &pixelFormat ) ) {
		return false;
	}

	HGLRC fakeHglrc = wglCreateContext( fakeHdc );
	wglMakeCurrent( fakeHdc, fakeHglrc );

	if( !OpenGLAddresses::LoadExtensionList() ) {
		return false;
	}

	wglMakeCurrent( NULL, NULL );
	UnregisterClass( "i0rTechFakeFrame", hInstance );
	wglDeleteContext( fakeHglrc );
	DestroyWindow( fakeWindow );
	ReleaseDC( fakeWindow, fakeHdc );

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor = {};

	const i32 pFormat[] = {
		WGL_DRAW_TO_WINDOW_ARB, GENERIC_RENDER_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GENERIC_RENDER_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GENERIC_RENDER_TRUE,
		WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 24,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0
	};


#if defined ( FLAG_DEBUG )
	const i32 contextFlags = ( Instance.Configuration->Display.EnableApiDebugLogging ) ? WGL_CONTEXT_DEBUG_BIT_ARB : 0;
#else
	const i32 contextFlags = 0;
#endif

	i32 context[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB,
		4,
		WGL_CONTEXT_MINOR_VERSION_ARB,
		0,
		WGL_CONTEXT_PROFILE_MASK_ARB,
		WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		WGL_CONTEXT_RELEASE_BEHAVIOR_ARB,
		WGL_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB,
		WGL_CONTEXT_FLAGS_ARB,
		WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | contextFlags,
		0
	};

	i32 piFormats = 0;
	UINT nNumFormats = 0;

	renderWindow->SetDrawContext( GetDC( renderWindow->GetWindow() ) );

	if( !wglChoosePixelFormatARB( renderWindow->GetDrawContext(), pFormat, NULL, 1, &piFormats, &nNumFormats ) ) {
		return false;
	}

	if( !SetPixelFormat( renderWindow->GetDrawContext(), piFormats, &pixelFormatDescriptor ) ) {
		return false;
	}

	renderWindow->SetRenderingContext( wglCreateContextAttribsARB( renderWindow->GetDrawContext(), 0, context ) );

	if( !renderWindow->GetRenderingContext() ) {
		return false;
	}

	wglMakeCurrent( renderWindow->GetDrawContext(), renderWindow->GetRenderingContext() );

	#ifdef FLAG_DEBUG
		glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );

		glDebugMessageCallback( DebugCallback, nullptr );
	#endif

	glEnable( GL_DEPTH_CLAMP );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_STENCIL_TEST );
	glEnable( GL_CULL_FACE );
	glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS );

	glDisable( GL_BLEND );

	glCullFace( GL_BACK );
	glDepthFunc( GL_LESS );
	glFrontFace( GL_CCW );

	glStencilMask( 0xFF );
	glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );

	glClearStencil( 0 );
	glClearDepth( 1.0f );
	glClearColor( 0.0f, 0.0f, 0.0f, 1.00f );

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glViewport( 0, 0, Instance.Configuration->Display.WindowWidth, Instance.Configuration->Display.WindowHeight );

	#ifdef FLAG_WINDOWS
		( Instance.Configuration->Display.EnableVSync ) ? wglSwapIntervalEXT( 1 ) : wglSwapIntervalEXT( 0 );
	#endif

	i32 texUnitCount = -1;
	glGetIntegerv( GL_MAJOR_VERSION, &Instance.Configuration->Display.ApiVersionMajor );
	glGetIntegerv( GL_MINOR_VERSION, &Instance.Configuration->Display.ApiVersionMinor );
	glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &texUnitCount );
		
	if( texUnitCount < 16 || ( Instance.Configuration->Display.ApiVersionMajor < 4 && Instance.Configuration->Display.ApiVersionMinor < 3 ) ) {
		return false;
	}

	m_ShaderManager = new ShaderManager();
	Instance.ShaderManager   = m_ShaderManager;
	instances->ShaderManager = m_ShaderManager;

	SAFE_INITIALIZE( m_ShaderManager )

	m_TextureManager = new TextureManager();
	instances->TextureManager = m_TextureManager;
	SAFE_INITIALIZE( m_TextureManager )

	return true;
}

void OpenGL::DeleteIndiceBuffer( IIndiceBuffer* &buffer ) {
	SAFE_DELETE( buffer )
}

void OpenGL::DeleteVertexBuffer( IVertexBuffer* &buffer ) {
	SAFE_DELETE( buffer )
}

void OpenGL::EnableBackBufferDepthWrite( const i32 state ) const  {
	glDepthMask( state );
}

void OpenGL::EnableDepthTest( const i32 state ) const  {
	( state == GENERIC_RENDER_TRUE ) ? glEnable( GL_DEPTH_TEST ) : glDisable( GL_DEPTH_TEST );
}

void OpenGL::CreateUBO( void* data, const i32 &size, void* &index ) {
	index = new GLuint();

	glGenBuffers( 1, &*(GLuint*)index );
	glBindBuffer( GL_UNIFORM_BUFFER, *(GLuint*)index );
	glBufferData( GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_COPY );
	glBindBuffer( GL_UNIFORM_BUFFER, 0 );
}

void OpenGL::UploadUBO( void* data, const i32 &size, void* &index ) {
	glBindBuffer( GL_UNIFORM_BUFFER, *(GLuint*)index );
	GLvoid* p = glMapBuffer( GL_UNIFORM_BUFFER, GENERIC_RENDER_WRITE_ONLY );

	if( !p ) {
		CONSOLE_PRINT_ERROR( "UniformBuffer::Upload => Failed to upload uniform buffer %i\n", *(GLuint*)index );
		return;
	}

	memcpy( p, data, size );

	glUnmapBuffer( GL_UNIFORM_BUFFER );
	glBindBuffer( GL_UNIFORM_BUFFER, 0 );
}

void OpenGL::BindUBO( void* &index, const u32 &bindPoint ) {
	glBindBufferBase( GL_UNIFORM_BUFFER, bindPoint, *(GLuint*)index );
}

void OpenGL::FreeUBO( void* &index ) {
	glDeleteBuffers( 1, &*(GLuint*)index );
	index = 0;
}

Itexture_t* OpenGL::CreateTexture() {
	return RENDER_ALLOCATE( texture_t );
}

IIndiceBuffer* OpenGL::CreateIndiceBuffer() {
	return RENDER_ALLOCATE( IndiceBuffer );
}

IVertexBuffer* OpenGL::CreateVertexBuffer() {
	return RENDER_ALLOCATE( VertexBuffer );
}

IFrameBuffer* OpenGL::CreateFrameBuffer( const i32 width, const i32 height ) {
	return RENDER_ALLOCATE_EX( FrameBuffer, ( width, height ) );
}

void OpenGL::BindBackBuffer() const  {
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
}

void OpenGL::ClearBuffer( const i32 clearFlags ) const  {
	glClear( clearFlags );
}

void OpenGL::UpdateViewport( const i32 x, const i32 y, const i32 width, const i32 height ) const  {
	glViewport( x, y, width, height );
}

void OpenGL::SetRasterizationMode( const i32 flag ) const  {
	glPolygonMode( GL_FRONT_AND_BACK, flag );
}

void OpenGL::SetFaceCulling( const i32 state ) const  {
	glCullFace( ( ( state == GENERIC_RENDER_FRONT ) ? GL_FRONT : ( state == GENERIC_RENDER_BACK ) ? GL_BACK : GL_FRONT_AND_BACK )  );
}

void OpenGL::EnableBlending( const i32 state ) const  {
	( state == GENERIC_RENDER_TRUE ) ? glEnable( GL_BLEND ) : glDisable( GL_BLEND );
}

void OpenGL::SetFrontFace( const i32 state ) const  {
	( state == GENERIC_RENDER_CLOCKWISE ) ? glFrontFace( GL_CW ) : glFrontFace( GL_CCW );
}

void OpenGL::SetStencilFunction( const i32 func, const i32 ref, const i32 mask ) const  {
	glStencilFunc( func, ref, mask );
}

void OpenGL::SetStencilOperation( const i32 stencilFail, const i32 depthFail, const i32 pass ) const  {
	glStencilOp( stencilFail, depthFail, pass );
}

void OpenGL::SetBlending( const i32 source, const i32 dest ) const  {
	glBlendFunc( source, dest );
}

void OpenGL::DrawIndicelessBuffer( const i32 primitive, const i32 first, const i32 count ) const  {
	glDrawArrays( primitive, first, count );
}

void OpenGL::SetMemoryBarrier( const i32 flags ) const  {
	glMemoryBarrier( flags );
}

void OpenGL::BindImageTexture( const u32 unit, const u32 texture, const i32 level, const bool layered, const i32 layer,
								                       const u32 access, const u32 format ) const  {
	glBindImageTexture( unit, texture, level, layered, layer, access, format );
}

void OpenGL::DispatchCompute( const u32 groupX, const u32 groupY, const u32 groupZ ) const  {
	glDispatchCompute( groupX, groupY, groupZ );
}

void OpenGL::GenerateMipMap( const u32 target ) const  {
	glGenerateMipmap( target );
}
