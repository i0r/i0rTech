#pragma once

#include <GL/gl.h>

// to avoid compiler warning
#define OPENGL_BUFFER_OFFSET( x ) ( char* )NULL + x

#define GL_GENERATE_MIPMAP 0x8191
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB   0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define WGL_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB   0x0000
#define WGL_CONTEXT_DEBUG_BIT_ARB   0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB  0x0002
#define WGL_DRAW_TO_WINDOW_ARB    0x2001
#define WGL_ACCELERATION_ARB    0x2003
#define WGL_SWAP_METHOD_ARB     0x2007
#define WGL_SUPPORT_OPENGL_ARB    0x2010
#define WGL_DOUBLE_BUFFER_ARB     0x2011
#define WGL_PIXEL_TYPE_ARB   0x2013
#define WGL_COLOR_BITS_ARB   0x2014
#define WGL_DEPTH_BITS_ARB   0x2022
#define WGL_STENCIL_BITS_ARB    0x2023
#define WGL_FULL_ACCELERATION_ARB   0x2027
#define WGL_SWAP_EXCHANGE_ARB     0x2028
#define WGL_CONTEXT_FLAGS_ARB     0x2094
#define WGL_TYPE_RGBA_ARB    0x202B
#define WGL_CONTEXT_RELEASE_BEHAVIOR_ARB 0x2097
#define WGL_CONTEXT_MAJOR_VERSION_ARB    0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB    0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB   0x9126
#define GL_ARRAY_BUFFER    0x8892
#define GL_STATIC_DRAW     0x88E4
#define GL_FRAGMENT_SHADER   0x8B30
#define GL_VERTEX_SHADER   0x8B31
#define GL_COMPILE_STATUS    0x8B81
#define GL_LINK_STATUS     0x8B82
#define GL_INFO_LOG_LENGTH   0x8B84
#define GL_TEXTURE0      0x84C0
#define GL_BGRA     0x80E1
#define GL_ELEMENT_ARRAY_BUFFER   0x8893
#define GL_MAJOR_VERSION   0x821B
#define GL_MINOR_VERSION   0x821C
#define GL_DEPTH_CLAMP     0x864F
#define GL_TEXTURE_CUBE_MAP_SEAMLESS   0x884F
#define GL_TEXTURE_2D      0x0DE1
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT  0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#define GL_GEOMETRY_SHADER   0x8DD9
#define GL_TESS_EVALUATION_SHADER   0x8E87
#define GL_TESS_CONTROL_SHADER    0x8E88
#define GL_COLOR_ATTACHMENT0    0x8CE0
#define GL_FRAMEBUFFER_COMPLETE   0x8CD5
#define GL_FRAMEBUFFER     0x8D40
#define GL_DEPTH_ATTACHMENT     0x8D00 
#define GL_CLAMP_TO_BORDER   0x812D
#define GL_RGBA32F    0x8814
#define GL_RGB32F     0x8815
#define GL_RGBA16F    0x881A
#define GL_RGB16F     0x881B
#define GL_DYNAMIC_DRAW    0x88E8
#define GL_R8      0x8229
#define GL_SHADER_STORAGE_BUFFER  0x90D2
#define GL_DYNAMIC_READ     0x88E9 
#define GL_DYNAMIC_COPY     0x88EA
#define GL_WRITE_ONLY     0x88B9
#define GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS  0x90DB
#define GL_MAX_UNIFORM_BUFFER_BINDINGS   0x8A2F
#define GL_MAX_UNIFORM_BUFFER_BINDINGS   0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE  0x8A30
#define GL_UNIFORM_BUFFER   0x8A11
#define GL_CLAMP_TO_EDGE   0x812F
#define GL_DEPTH_COMPONENT32F    0x8CAC
#define GL_READ_FRAMEBUFFER   0x8CA8
#define GL_DRAW_FRAMEBUFFER   0x8CA9
#define GL_RG32F    0x8230
#define GL_MAX_TEXTURE_IMAGE_UNITS 0x8872
#define GL_RG32UI    0x823C
#define GL_PIXEL_PACK_BUFFER    0x88EB
#define GL_RG       0x8227
#define GL_R32I     0x8235
#define GL_R32UI    0x8236
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_MAX_TEXTURE_BUFFER_SIZE  0x8C2B
#define GL_TEXTURE_BUFFER   0x8C2A
#define GL_TEXTURE_UPDATE_BARRIER_BIT_EXT 0x00000100
#define GL_BUFFER_UPDATE_BARRIER_BIT_EXT 0x00000200
#define GL_READ_ONLY     0x88B8
#define GL_TEXTURE_MAX_LEVEL    0x813D
#define GL_READ_WRITE     0x88BA
#define GL_RED_INTEGER     0x8D94
#define GL_DEPTH_TEXTURE_MODE    0x884B
#define GL_TEXTURE_COMPARE_MODE   0x884C
#define GL_COMPARE_R_TO_TEXTURE    0x884E
#define GL_TEXTURE_COMPARE_FUNC    0x884D
#define GL_MAP_PERSISTENT_BIT     0x0040
#define GL_MAP_COHERENT_BIT    0x0080
#define GL_MAP_READ_BIT      0x0001
#define GL_MAP_WRITE_BIT     0x0002
#define GL_COMPUTE_SHADER   0x91B9
#define GL_SHADER_STORAGE_BLOCK    0x92E6
#define GL_READ_FRAMEBUFFER   0x8CA8
#define GL_DRAW_FRAMEBUFFER   0x8CA9
#define GL_MAX_COMPUTE_WORK_GROUP_SIZE   0x91BF
#define GL_TEXTURE1    0x84C1
#define GL_TEXTURE2    0x84C2
#define GL_TEXTURE3    0x84C3
#define GL_TEXTURE4    0x84C4
#define GL_TEXTURE5    0x84C5
#define GL_TEXTURE6    0x84C6
#define GL_TEXTURE7    0x84C7
#define GL_TEXTURE8    0x84C8
#define GL_TEXTURE9    0x84C9
#define GL_TEXTURE10     0x84CA
#define GL_TEXTURE11     0x84CB
#define GL_TEXTURE12     0x84CC
#define GL_TEXTURE13     0x84CD
#define GL_TEXTURE14     0x84CE
#define GL_TEXTURE15     0x84CF
#define GL_TEXTURE16     0x84D0
#define GL_TEXTURE17     0x84D1
#define GL_TEXTURE18     0x84D2
#define GL_TEXTURE19     0x84D3
#define GL_TEXTURE20     0x84D4
#define GL_TEXTURE21     0x84D5
#define GL_TEXTURE22     0x84D6
#define GL_TEXTURE23     0x84D7
#define GL_TEXTURE24     0x84D8
#define GL_TEXTURE25     0x84D9
#define GL_TEXTURE26     0x84DA
#define GL_TEXTURE27     0x84DB
#define GL_TEXTURE28     0x84DC
#define GL_TEXTURE29     0x84DD
#define GL_TEXTURE30     0x84DE
#define GL_TEXTURE31     0x84DF
#define GL_ALL_BARRIER_BITS   0xFFFFFFFF
#define GL_FUNC_ADD    0x8006
#define GL_FUNC_SUBTRACT   0x800A
#define GL_FUNC_REVERSE_SUBTRACT  0x800B
#define GL_DEPTH24_STENCIL8   0x88F0
#define GL_DEPTH_STENCIL   0x84F9
#define GL_DEPTH_STENCIL_ATTACHMENT  0x821A
#define GL_UNSIGNED_INT_24_8    0x84FA
#define GL_FRAMEBUFFER_SRGB   0x8DB9
#define GL_TEXTURE_3D     0x806F
#define GL_TEXTURE_WRAP_R   0x8072
#define GL_RG16F    0x822F
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP       0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE      0x851C
#define GL_FRAMEBUFFER_DEFAULT_WIDTH 0x9310
#define GL_FRAMEBUFFER_DEFAULT_HEIGHT 0x9311
#define GL_FRAMEBUFFER_DEFAULT_LAYERS 0x9312
#define GL_FRAMEBUFFER_DEFAULT_SAMPLES 0x9313
#define GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS 0x9314
#define GL_MAX_FRAMEBUFFER_WIDTH 0x9315
#define GL_MAX_FRAMEBUFFER_HEIGHT 0x9316
#define GL_MAX_FRAMEBUFFER_LAYERS 0x9317
#define GL_MAX_FRAMEBUFFER_SAMPLES 0x9318
#define GL_VERTEX_PROGRAM_POINT_SIZE 0x8642
#define GL_TEXTURE_FETCH_BARRIER_BIT 0x00000008
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT 0x00000020
#define GL_MAP_INVALIDATE_BUFFER_BIT 0x0008
#define GL_SAMPLE_SHADING                 0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE       0x8C37
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5F
#define GL_TEXTURE_CUBE_MAP_ARRAY         0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY 0x900A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY   0x900B
#define GL_SAMPLER_CUBE_MAP_ARRAY         0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW  0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY     0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY 0x900F
#define GL_BGR 0x80E0
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_COMPARE_REF_TO_TEXTURE 0x884E
#define GL_DEBUG_CALLBACK_FUNCTION        0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM      0x8245
#define GL_DEBUG_SOURCE_API               0x8246
#define GL_DEBUG_OUTPUT_SYNCHRONOUS       0x8242
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM     0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER   0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY       0x8249
#define GL_DEBUG_SOURCE_APPLICATION       0x824A
#define GL_DEBUG_SOURCE_OTHER             0x824B
#define GL_DEBUG_TYPE_ERROR               0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR  0x824E
#define GL_DEBUG_TYPE_PORTABILITY         0x824F
#define GL_DEBUG_TYPE_PERFORMANCE         0x8250
#define GL_DEBUG_TYPE_OTHER               0x8251
#define GL_MAX_DEBUG_MESSAGE_LENGTH       0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES      0x9144
#define GL_DEBUG_LOGGED_MESSAGES          0x9145
#define GL_DEBUG_SEVERITY_HIGH            0x9146
#define GL_DEBUG_SEVERITY_MEDIUM          0x9147
#define GL_DEBUG_SEVERITY_LOW             0x9148
#define GL_DEBUG_TYPE_MARKER              0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP          0x8269
#define GL_DEBUG_TYPE_POP_GROUP           0x826A
#define GL_DEBUG_SEVERITY_NOTIFICATION    0x826B
#define GL_MAX_DEBUG_GROUP_STACK_DEPTH    0x826C
#define GL_DEBUG_GROUP_STACK_DEPTH        0x826D

typedef char GLchar;
typedef ptrdiff_t GLsizeiptr;
typedef int GLsizei;
typedef void GLvoid;

typedef void ( APIENTRY * GLDEBUGPROC )( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam );
typedef void ( APIENTRY * PFNGLDEBUGMESSAGECALLBACKPROC ) ( GLDEBUGPROC callback, const void *userParam );

typedef void ( APIENTRY * PFNGLATTACHSHADERPROC )( GLuint program, GLuint shader );
typedef void ( APIENTRY * PFNGLBINDBUFFERPROC )( GLenum target, GLuint buffer );
typedef void ( APIENTRY * PFNGLBINDVERTEXARRAYPROC )( GLuint array );
typedef void ( APIENTRY * PFNGLBUFFERDATAPROC )( GLenum target, ptrdiff_t size, const GLvoid *data, GLenum usage );
typedef void ( APIENTRY * PFNGLCOMPILESHADERPROC )( GLuint shader );
typedef GLuint( APIENTRY * PFNGLCREATEPROGRAMPROC )( void );
typedef GLuint( APIENTRY * PFNGLCREATESHADERPROC )( GLenum type );
typedef void ( APIENTRY * PFNGLDELETEBUFFERSPROC )( GLsizei n, const GLuint *buffers );
typedef void ( APIENTRY * PFNGLDELETEPROGRAMPROC )( GLuint program );
typedef void ( APIENTRY * PFNGLDELETESHADERPROC )( GLuint shader );
typedef void ( APIENTRY * PFNGLDELETEVERTEXARRAYSPROC )( GLsizei n, const GLuint *arrays );
typedef void ( APIENTRY * PFNGLDETACHSHADERPROC )( GLuint program, GLuint shader );
typedef void ( APIENTRY * PFNGLENABLEVERTEXATTRIBARRAYPROC )( GLuint index );
typedef void ( APIENTRY * PFNGLGENBUFFERSPROC )( GLsizei n, GLuint *buffers );
typedef void ( APIENTRY * PFNGLGENVERTEXARRAYSPROC )( GLsizei n, GLuint *arrays );
typedef GLint( APIENTRY * PFNGLGETATTRIBLOCATIONPROC )( GLuint program, const char *name );
typedef void ( APIENTRY * PFNGLGETPROGRAMINFOLOGPROC )( GLuint program, GLsizei bufSize, GLsizei *length, 
								                                                char *infoLog );
typedef void ( APIENTRY * PFNGLGETPROGRAMIVPROC )( GLuint program, GLenum pname, GLint *params );
typedef void ( APIENTRY * PFNGLGETSHADERINFOLOGPROC )( GLuint shader, GLsizei bufSize, GLsizei *length, 
								                                               char *infoLog );
typedef void ( APIENTRY * PFNGLGETSHADERIVPROC )( GLuint shader, GLenum pname, GLint *params );
typedef void ( APIENTRY * PFNGLLINKPROGRAMPROC )( GLuint program );
typedef void ( APIENTRY * PFNGLUSEPROGRAMPROC )( GLuint program );
typedef void ( APIENTRY * PFNGLVERTEXATTRIBPOINTERPROC )( GLuint index, GLint size, GLenum type, GLboolean normalized,
								                                                  GLsizei stride, const GLvoid *pointer );
typedef void ( APIENTRY * PFNGLBINDATTRIBLOCATIONPROC )( GLuint program, GLuint index, const char *name );
typedef GLint( APIENTRY * PFNGLGETUNIFORMLOCATIONPROC )( GLuint program, const char *name );
typedef void ( APIENTRY * PFNGLUNIFORMMATRIX4FVPROC )( GLint location, GLsizei count, GLboolean transpose,
								                                               const GLfloat *value );
typedef void ( APIENTRY * PFNGLACTIVETEXTUREPROC )( GLenum texture );
typedef void ( APIENTRY * PFNGLUNIFORM1IPROC )( GLint location, GLint v0 );
typedef void ( APIENTRY * PFNGLUNIFORM1FPROC )( GLint location, GLfloat x );
typedef void ( APIENTRY * PFNGLUNIFORM1FVPROC )( GLint location, GLsizei count, const GLfloat* v );
typedef void ( APIENTRY * PFNGLUNIFORM1IVPROC )( GLint location, GLsizei count, const GLint* v );
typedef void ( APIENTRY * PFNGLGENERATEMIPMAPPROC )( GLenum target );
typedef void ( APIENTRY * PFNGLDISABLEVERTEXATTRIBARRAYPROC )( GLuint index );
typedef void ( APIENTRY * PFNGLUNIFORM2FPROC )( GLint location, GLfloat x, GLfloat y );
typedef void ( APIENTRY * PFNGLUNIFORM2FVPROC )( GLint location, GLsizei count, const GLfloat* v );
typedef void ( APIENTRY * PFNGLUNIFORM2IPROC )( GLint location, GLint x, GLint y );
typedef void ( APIENTRY * PFNGLUNIFORM2IVPROC )( GLint location, GLsizei count, const GLint* v );
typedef void ( APIENTRY * PFNGLUNIFORM3FVPROC )( GLint location, GLsizei count, const GLfloat *value );
typedef void ( APIENTRY * PFNGLUNIFORM3FPROC )( GLint location, GLfloat x, GLfloat y, GLfloat z );
typedef void ( APIENTRY * PFNGLUNIFORM3IPROC )( GLint location, GLint x, GLint y, GLint z );
typedef void ( APIENTRY * PFNGLUNIFORM3IVPROC )( GLint location, GLsizei count, const GLint* v );
typedef void ( APIENTRY * PFNGLUNIFORM4FPROC )( GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w );
typedef void ( APIENTRY * PFNGLUNIFORM4IPROC )( GLint location, GLint x, GLint y, GLint z, GLint w );
typedef void ( APIENTRY * PFNGLUNIFORM4IVPROC )( GLint location, GLsizei count, const GLint* v );
typedef void ( APIENTRY * PFNGLUNIFORM4FVPROC )( GLint location, GLsizei count, const GLfloat *value );
typedef void ( APIENTRY * PFNGLUNIFORMMATRIX2FVPROC )( GLint location, GLsizei count, GLboolean transpose,
								                                               const GLfloat* value );
typedef void ( APIENTRY * PFNGLUNIFORMMATRIX3FVPROC )( GLint location, GLsizei count, GLboolean transpose,
								                                               const GLfloat* value );
typedef void ( APIENTRY * PFNGLCOMPRESSEDTEXIMAGE2DPROC )( GLenum target, GLint level, GLenum internalformat, 
								                                                   GLsizei width, GLsizei height, GLint border, 
								                                                   GLsizei imageSize, const void *data );
typedef void ( APIENTRY * PFNGLGENFRAMEBUFFERSPROC )( GLsizei n, GLuint *framebuffers );
typedef void ( APIENTRY * PFNGLFRAMEBUFFERTEXTURE2DPROC )( GLenum target, GLenum attachment, GLenum textarget,
								                                                   GLuint texture, GLint level );
typedef void ( APIENTRY * PFNGLDELETEFRAMEBUFFERSPROC )( GLsizei n, const GLuint *framebuffers );
typedef GLenum( APIENTRY * PFNGLCHECKFRAMEBUFFERSTATUSPROC )( GLenum target );
typedef void ( APIENTRY * PFNGLDRAWBUFFERSPROC )( GLsizei n, const GLenum *bufs );
typedef void ( APIENTRY * PFNGLBINDFRAMEBUFFERPROC )( GLenum target, GLuint framebuffer );
typedef GLvoid * ( APIENTRY * PFNGLMAPBUFFERARBPROC )( GLenum target, GLenum access );
typedef GLboolean( APIENTRY * PFNGLUNMAPBUFFERARBPROC )( GLenum target );
typedef void ( APIENTRY * PFNGLBINDBUFFERBASE )( GLenum target, GLuint index, GLuint buffer );
typedef GLuint( APIENTRY * PFNGLGETUNIFORMBLOCKINDEXPROC )( GLuint program, const GLchar *uniformBlockName );
typedef void ( APIENTRY * PFNGLUNIFORMBLOCKBINDINGPROC )( GLuint program, GLuint uniformBlockIndex, 
								                                                  GLuint uniformBlockBinding );
typedef void ( APIENTRY * PFNGLBINDIMAGETEXTUREEXTPROC )( GLuint index, GLuint texture, GLint level, GLboolean layered,
								                                                  GLint layer, GLenum access, GLint format );
typedef void ( APIENTRY * PFNGLTEXBUFFERPROC )( GLenum target, GLenum internalformat, GLuint buffer );
typedef void ( APIENTRY * PFNGLMEMORYBARRIERPROC )( GLbitfield barriers );
typedef void ( APIENTRY * PFNGLGETBUFFERSUBDATA )( GLenum target, GLint* offset, GLsizei* size, const GLvoid* data );
typedef void ( APIENTRY * PFNGLBLITFRAMEBUFFERPROC )( GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
								                                              GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1,
								                                              GLbitfield mask, GLenum filter );
typedef void ( APIENTRY * PFNGLDISPATCHCOMPUTEPROC )( GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z );
typedef GLuint( APIENTRY * PFNGLGETPROGRAMRESOURCEINDEXPROC )( GLuint program, GLenum programInterface, 
								                                                       const GLchar *name );
typedef void ( APIENTRY * PFNGLSHADERSTORAGEBLOCKBINDINGPROC )( GLuint program, GLuint storageBlockIndex, 
								                                                        GLuint storageBlockBinding );
typedef void ( APIENTRY * PFNGLFRAMEBUFFERTEXTURELAYERPROC )( GLenum target, GLenum attachment, GLuint texture,
								                                                      GLint level, GLint layer );
typedef void ( APIENTRY * PFNGLBLENDEQUATIONPROC )( GLenum mode );
typedef void ( APIENTRY * PFNGLFRAMEBUFFERTEXTURE3D )( GLenum, GLenum, GLenum, GLuint, GLint, GLint );
typedef void ( APIENTRY * PFNGLTEXIMAGE3DPROC )( GLenum target, GLint level, GLint internalformat, GLsizei width,
								                                         GLsizei height, GLsizei depth, GLint border, GLenum format,
								                                         GLenum type, const void *pixels );
typedef void ( APIENTRY * PFNGLFRAMEBUFFERTEXTUREPROC )( GLenum target, GLenum attachment, GLuint texture, 
								                                                 GLint level );
typedef void ( APIENTRY * PFNGLBLENDEQUATIONSEPARATEPROC )( GLenum modeRGB, GLenum modeAlpha );
typedef void ( APIENTRY * PFNGLBLENDFUNCSEPARATEPROC )( GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha,
								                                                GLenum dfactorAlpha );
typedef void ( APIENTRY * PFNGLFRAMEBUFFERPARAMETERIPROC )( GLenum target, GLenum pname, GLint param );
typedef void ( APIENTRY * PFNGLTEXSTORAGE3DPROC )( GLenum target, GLsizei levels, GLenum internalformat, GLsizei width,
								                                           GLsizei height, GLsizei depth );
	typedef BOOL( WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC )( HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList,
								                                                  UINT nMaxFormats, int *piFormats, UINT *nNumFormats );
	typedef HGLRC( WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC )( HDC hDC, HGLRC hShareContext, const int *attribList );
	typedef BOOL( WINAPI * PFNWGLSWAPINTERVALEXTPROC )( int interval );
	typedef void ( APIENTRY * PFNGLSHADERSOURCEPROC )( GLuint shader, GLsizei count, const char* *string, 
								                                            const GLint *length );
	typedef void ( APIENTRY * PFNGLBUFFERSTORAGEPROC )( GLenum target, GLsizei* size, const void *data, GLbitfield flags );
	typedef void *( APIENTRY * PFNGLMAPBUFFERRANGEPROC )( GLenum target, GLint* offset, GLsizeiptr length, 
								                                               GLbitfield access );


extern PFNGLATTACHSHADERPROC pglAttachShader;
extern PFNGLBINDBUFFERPROC pglBindBuffer;
extern PFNGLBINDVERTEXARRAYPROC pglBindVertexArray;
extern PFNGLBUFFERDATAPROC pglBufferData;
extern PFNGLCOMPILESHADERPROC pglCompileShader;
extern PFNGLCREATEPROGRAMPROC pglCreateProgram;
extern PFNGLCREATESHADERPROC pglCreateShader;
extern PFNGLDELETEBUFFERSPROC pglDeleteBuffers;
extern PFNGLDELETEPROGRAMPROC pglDeleteProgram;
extern PFNGLDELETESHADERPROC pglDeleteShader;
extern PFNGLDELETEVERTEXARRAYSPROC pglDeleteVertexArrays;
extern PFNGLDETACHSHADERPROC pglDetachShader;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC pglEnableVertexAttribArray;
extern PFNGLGENBUFFERSPROC pglGenBuffers;
extern PFNGLGENVERTEXARRAYSPROC pglGenVertexArrays;
extern PFNGLGETATTRIBLOCATIONPROC pglGetAttribLocation;
extern PFNGLGETPROGRAMINFOLOGPROC pglGetProgramInfoLog;
extern PFNGLGETPROGRAMIVPROC pglGetProgramiv;
extern PFNGLGETSHADERINFOLOGPROC pglGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC pglGetShaderiv;
extern PFNGLLINKPROGRAMPROC pglLinkProgram;
extern PFNGLSHADERSOURCEPROC pglShaderSource;
extern PFNGLUSEPROGRAMPROC pglUseProgram;
extern PFNGLVERTEXATTRIBPOINTERPROC pglVertexAttribPointer;
extern PFNGLBINDATTRIBLOCATIONPROC pglBindAttribLocation;
extern PFNGLGETUNIFORMLOCATIONPROC pglGetUniformLocation;
extern PFNGLUNIFORMMATRIX2FVPROC pglUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVPROC pglUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC pglUniformMatrix4fv;
extern PFNGLUNIFORM1IPROC pglUniform1i;
extern PFNGLUNIFORM1FPROC pglUniform1f;
extern PFNGLUNIFORM1IVPROC pglUniform1iv;
extern PFNGLUNIFORM1FVPROC pglUniform1fv;
extern PFNGLGENERATEMIPMAPPROC pglGenerateMipmap;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC pglDisableVertexAttribArray;
extern PFNGLUNIFORM2IPROC pglUniform2i;
extern PFNGLUNIFORM2FPROC pglUniform2f;
extern PFNGLUNIFORM2IVPROC pglUniform2iv;
extern PFNGLUNIFORM2FVPROC pglUniform2fv;
extern PFNGLUNIFORM3IPROC pglUniform3i;
extern PFNGLUNIFORM3FPROC pglUniform3f;
extern PFNGLUNIFORM3IVPROC pglUniform3iv;
extern PFNGLUNIFORM3FVPROC pglUniform3fv;
extern PFNGLUNIFORM4FPROC pglUniform4f;
extern PFNGLUNIFORM4IPROC pglUniform4i;
extern PFNGLUNIFORM4IVPROC pglUniform4iv;
extern PFNGLUNIFORM4FVPROC pglUniform4fv;
extern PFNGLGENFRAMEBUFFERSPROC pglGenFramebuffers;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC pglFramebufferTexture2D;
extern PFNGLDELETEFRAMEBUFFERSPROC pglDeleteFramebuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC pglCheckFramebufferStatus;
extern PFNGLDRAWBUFFERSPROC pglDrawBuffers;
extern PFNGLBINDFRAMEBUFFERPROC pglBindFramebuffer;
extern PFNGLMAPBUFFERARBPROC pglMapBuffer;
extern PFNGLUNMAPBUFFERARBPROC pglUnmapBuffer;
extern PFNGLBINDBUFFERBASE pglBindBufferBase;
extern PFNGLGETUNIFORMBLOCKINDEXPROC pglGetUniformBlockIndex;
extern PFNGLUNIFORMBLOCKBINDINGPROC pglUniformBlockBinding;
extern PFNGLBINDIMAGETEXTUREEXTPROC pglBindImageTexture;
extern PFNGLTEXBUFFERPROC pglTexBuffer;
extern PFNGLMEMORYBARRIERPROC pglMemoryBarrier;
extern PFNGLGETBUFFERSUBDATA pglGetBufferSubData;
extern PFNGLBLITFRAMEBUFFERPROC pglBlitFramebuffer;
extern PFNGLDISPATCHCOMPUTEPROC pglDispatchCompute;
extern PFNGLGETPROGRAMRESOURCEINDEXPROC pglGetProgramResourceIndex;
extern PFNGLSHADERSTORAGEBLOCKBINDINGPROC pglShaderStorageBlockBinding;
extern PFNGLFRAMEBUFFERTEXTURELAYERPROC pglFramebufferTextureLayer;
extern PFNGLBLENDEQUATIONPROC pglBlendEquation;
extern PFNGLFRAMEBUFFERTEXTURE3D pglFrameBufferTexture3D;
extern PFNGLTEXIMAGE3DPROC pglTexImage3D;
extern PFNGLFRAMEBUFFERTEXTUREPROC pglFramebufferTexture;
extern PFNGLBLENDEQUATIONSEPARATEPROC pglBlendEquationSeparate;
extern PFNGLBLENDFUNCSEPARATEPROC pglBlendFuncSeparate;
extern PFNGLFRAMEBUFFERPARAMETERIPROC pglFramebufferParameteri;
extern PFNGLTEXSTORAGE3DPROC pglTexStorage3D;
extern PFNGLACTIVETEXTUREPROC pglActiveTexture;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC pglCompressedTexImage2D;
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
extern PFNGLBUFFERSTORAGEPROC pglBufferStorage;
extern PFNGLMAPBUFFERRANGEPROC pglMapBufferRange;
extern PFNGLDEBUGMESSAGECALLBACKPROC pglDebugMessageCallback;

#define glAttachShader pglAttachShader
#define glBindBuffer pglBindBuffer
#define glBindVertexArray pglBindVertexArray
#define glBufferData pglBufferData
#define glCompileShader pglCompileShader
#define glCreateProgram pglCreateProgram
#define glCreateShader pglCreateShader
#define glDeleteBuffers pglDeleteBuffers
#define glDeleteProgram pglDeleteProgram
#define glDeleteShader pglDeleteShader
#define glDeleteVertexArrays pglDeleteVertexArrays
#define glDetachShader pglDetachShader
#define glEnableVertexAttribArray pglEnableVertexAttribArray
#define glGenBuffers pglGenBuffers
#define glGenVertexArrays pglGenVertexArrays
#define glGetAttribLocation pglGetAttribLocation
#define glGetProgramInfoLog pglGetProgramInfoLog
#define glGetProgramiv pglGetProgramiv
#define glGetShaderInfoLog pglGetShaderInfoLog
#define glGetShaderiv pglGetShaderiv
#define glLinkProgram pglLinkProgram
#define glShaderSource pglShaderSource
#define glUseProgram pglUseProgram
#define glVertexAttribPointer pglVertexAttribPointer
#define glBindAttribLocation pglBindAttribLocation
#define glGetUniformLocation pglGetUniformLocation
#define glUniformMatrix2fv pglUniformMatrix2fv
#define glUniformMatrix3fv pglUniformMatrix3fv
#define glUniformMatrix4fv pglUniformMatrix4fv
#define glUniform1i pglUniform1i
#define glUniform1f pglUniform1f
#define glUniform1iv pglUniform1iv
#define glUniform1fv pglUniform1fv
#define glGenerateMipmap pglGenerateMipmap
#define glDisableVertexAttribArray pglDisableVertexAttribArray
#define glUniform2i pglUniform2i
#define glUniform2f pglUniform2f
#define glUniform2iv pglUniform2iv
#define glUniform2fv pglUniform2fv
#define glUniform3i pglUniform3i
#define glUniform3f pglUniform3f
#define glUniform3iv pglUniform3iv
#define glUniform3fv pglUniform3fv
#define glUniform4f pglUniform4f
#define glUniform4i pglUniform4i
#define glUniform4iv pglUniform4iv
#define glUniform4fv pglUniform4fv
#define glGenFramebuffers pglGenFramebuffers
#define glFramebufferTexture2D pglFramebufferTexture2D
#define glDeleteFramebuffers pglDeleteFramebuffers
#define glCheckFramebufferStatus pglCheckFramebufferStatus
#define glDrawBuffers pglDrawBuffers
#define glBindFramebuffer pglBindFramebuffer
#define glMapBuffer pglMapBuffer
#define glUnmapBuffer pglUnmapBuffer
#define glBindBufferBase pglBindBufferBase
#define glGetUniformBlockIndex pglGetUniformBlockIndex
#define glUniformBlockBinding pglUniformBlockBinding
#define glBindImageTexture pglBindImageTexture
#define glTexBuffer pglTexBuffer
#define glMemoryBarrier pglMemoryBarrier
#define glGetBufferSubData pglGetBufferSubData
#define glBlitFramebuffer pglBlitFramebuffer
#define glDispatchCompute pglDispatchCompute
#define glGetProgramResourceIndex pglGetProgramResourceIndex
#define glShaderStorageBlockBinding pglShaderStorageBlockBinding
#define glFramebufferTextureLayer pglFramebufferTextureLayer
#define glBlendEquation pglBlendEquation
#define glFramebufferTexture3D pglFrameBufferTexture3D
#define glTexImage3D pglTexImage3D
#define glFramebufferTexture pglFramebufferTexture
#define glBlendEquationSeparate pglBlendEquationSeparate
#define glBlendFuncSeparate pglBlendFuncSeparate
#define glFramebufferParameteri pglFramebufferParameteri
#define glTexStorage3D pglTexStorage3D
#define glActiveTexture pglActiveTexture
#define glCompressedTexImage2D pglCompressedTexImage2D
#define glBufferStorage pglBufferStorage
#define glMapBufferRange pglMapBufferRange
#define glDebugMessageCallback pglDebugMessageCallback

class OpenGLAddresses {
	public:
		static bool LoadExtensionList() ;
};
