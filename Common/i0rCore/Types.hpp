#pragma once

//=====================================================================================================================

template<typename T> struct vector2_t;
template<typename T> struct vector3_t;
template<typename T> struct vector4_t;

template<typename T> struct matrix3x3_t;
template<typename T> struct matrix4x4_t;

template<typename T> struct quaternion_t;

template<typename T> struct color_rgba_t;
template<typename T> struct color_rgb_t;

//=====================================================================================================================

typedef signed char                                                 i8;
typedef signed short                                               i16;
typedef signed int                                                 i32;
typedef signed long long                                           i64;

typedef unsigned char                                               u8;
typedef unsigned short                                             u16;
typedef unsigned int                                               u32;
typedef unsigned long long                                         u64;

typedef u16                                                        f16;
typedef float                                                      f32;
typedef double                                                     f64;

static_assert( sizeof( i8 )  == 1, "sizeof( i8 )  == 1" );
static_assert( sizeof( i16 ) == 2, "sizeof( i16 ) == 2" );
static_assert( sizeof( i32 ) == 4, "sizeof( i32 ) == 4" );
static_assert( sizeof( i64 ) == 8, "sizeof( i64 ) == 8" );

static_assert( sizeof( u8 )  == 1, "sizeof( u8 )  == 1" );
static_assert( sizeof( u16 ) == 2, "sizeof( u16 ) == 2" );
static_assert( sizeof( u32 ) == 4, "sizeof( u32 ) == 4" );
static_assert( sizeof( u64 ) == 8, "sizeof( u64 ) == 8" );

static_assert( sizeof( f16 ) == 2, "sizeof( f16 ) == 2" );
static_assert( sizeof( f32 ) == 4, "sizeof( f32 ) == 4" );
static_assert( sizeof( f64 ) == 8, "sizeof( f64 ) == 8" );

#define I8_MIN                                           ( -127i8 - 1 )
#define I16_MIN                                       ( -32767i16 - 1 )
#define I32_MIN                                  ( -2147483647i32 - 1 )
#define I64_MIN                         ( -9223372036854775807i64 - 1 )

#define I8_MAX                                                    127i8
#define I16_MAX                                                32767i16
#define I32_MAX                                           2147483647i32
#define I64_MAX                                  9223372036854775807i64

#define U8_MIN                                                   0x0ui8
#define U16_MIN                                                 0x0ui16
#define U32_MIN                                                 0x0ui32
#define U64_MIN                                                 0x0ui64

#define U8_MAX                                                  0xffui8
#define U16_MAX                                              0xffffui16
#define U32_MAX                                          0xffffffffui32
#define U64_MAX                                  0xffffffffffffffffui64

#define F16_MIN                                            2.047999e-3F
#define F32_MIN                                        1.175494351e-38F
#define F64_MIN                                        1.175494351e-38F

#define F16_MAX                                            2.047999e+3F
#define F32_MAX                                        3.402823466e+38F
#define F64_MAX                                 1.7976931348623158e+308

//=====================================================================================================================

typedef vector2_t<i32>                                           vec2i;
typedef vector2_t<u32>                                           vec2u;
typedef vector2_t<f32>                                           vec2f;
typedef vector2_t<f64>                                           vec2d;
typedef vector2_t<bool>                                          vec2b;

typedef vector3_t<i32>                                           vec3i;
typedef vector3_t<u32>                                           vec3u;
typedef vector3_t<f32>                                           vec3f;
typedef vector3_t<f64>                                           vec3d;
typedef vector3_t<bool>                                          vec3b;

typedef vector4_t<i32>                                           vec4i;
typedef vector4_t<u32>                                           vec4u;
typedef vector4_t<f32>                                           vec4f;
typedef vector4_t<f64>                                           vec4d;
typedef vector4_t<bool>                                          vec4b;

typedef matrix3x3_t<i32>                                       mat3x3i;
typedef matrix3x3_t<f32>                                       mat3x3f;
typedef matrix3x3_t<f64>                                       mat3x3d;
typedef matrix3x3_t<bool>                                      mat3x3b;

typedef matrix4x4_t<i32>                                       mat4x4i;
typedef matrix4x4_t<f32>                                       mat4x4f;
typedef matrix4x4_t<f64>                                       mat4x4d;
typedef matrix4x4_t<bool>                                      mat4x4b;

typedef quaternion_t<i32>                                        quati;
typedef quaternion_t<f32>                                        quatf;
typedef quaternion_t<f64>                                        quatd;
typedef quaternion_t<bool>                                       quatb;

typedef color_rgb_t<int8_t>                                  colorrgbb;
typedef color_rgba_t<int8_t>                                colorrgbab;

typedef color_rgb_t<f32>                                     colorrgbf;
typedef color_rgba_t<f32>                                   colorrgbaf;

#ifdef FLAG_WINDOWS
	typedef HANDLE             application_handle_t, file_system_handle_t;
	typedef LPCTSTR                                 file_system_pointer_t;
	typedef HINSTANCE                                    library_handle_t;
	typedef FARPROC                             library_function_handle_t;
#endif

typedef std::function<void( void )>             function_t, callback_t;
