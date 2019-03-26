#pragma once

template<typename T> struct vector4_t;
template<typename T> struct vector3_t;
template<typename T> struct vector2_t;

template<typename T> struct ALIGN( 16 ) quaternion_t {
	ALIGNED_OBJECT( quaternion_t, 16 )

	union {
		struct {
			T x;
			T y;
			T z;
			T w;
		};

		__m128 s;
	};

//=====================================================================================================================

	INLINE constexpr T operator[] ( const i32 i ) const  {
		return ( !i ) ? x : ( i == 1 ) ? y : ( i == 2 ) ? z : w;
	}

//=====================================================================================================================

	explicit quaternion_t()  : s( _mm_set_ps( 1, 0, 0, 0 ) ) {

	}

	explicit quaternion_t( const T &nx, const T &ny, const T &nz, const T &nw = 1 )  
		: s( _mm_set_ps( nw, nz, ny, nx ) ) {

	}

	explicit quaternion_t( const T f )  : s( _mm_set_ps( 1.0f, f, f, f ) ) {

	}

	explicit quaternion_t( const T f[4] )  : s( _mm_set_ps( f[3], f[2], f[1], f[0] ) ) {

	}

	explicit quaternion_t( const vector4_t<T> &v )  : s( _mm_set_ps( v.w, v.z, v.y, v.x ) ) {

	}

	explicit quaternion_t( const vector3_t<T> &v )  : s( _mm_set_ps( 1.0f, v.z, v.y, v.x ) ) {

	}

	explicit quaternion_t( const vector2_t<T> &v )  : s( _mm_set_ps( 1.0f, 0.0f, v.y, v.x ) ) {

	}

	explicit quaternion_t( const __m128 &ns )  : s( ns ) {

	}

	~quaternion_t() {
		s = _mm_set_ps1( 0.0f );
	}

//=====================================================================================================================

	INLINE quaternion_t<T> &operator = ( const quaternion_t<T>& q ) {
		s = _mm_set_ps( q.w, q.z, q.y, q.x );

		return *this;
	}

	INLINE quaternion_t<T> &operator = ( const vector4_t<T>& q ) {
		s = q.s;

		return *this;
	}

	INLINE quaternion_t<T> &operator = ( const vector3_t<T>& q ) {
		s = _mm_set_ps( (T)1, q.z, q.y, q.x ); 

		return *this;
	}

	INLINE quaternion_t<T> &operator = ( const vector2_t<T>& q ) {
		s = _mm_set_ps( (T)1, (T)0, q.y, q.x );

		return *this;
	}

	INLINE quaternion_t<T> &operator = ( const T fa[4] ) {
		s = _mm_set_ps( fa[3], fa[2], fa[1], fa[0] );

		return *this;
	}

//=====================================================================================================================

	INLINE quaternion_t<T> &operator * ( const T& f ) {
		return quaternion_t<T>( _mm_set_ps( w * f, z * f, y * f, x * f ) );
	}
	
//=====================================================================================================================

	INLINE void Clear() {
		s = _mm_set_ps( (T)1, (T)0, (T)0, (T)0 );
	}

	INLINE void Invert() {
		s = _mm_set_ps( w, -z, -y, -x );
	}

	INLINE void Flip() {
		s = _mm_set_ps( -w, -z, -y, -x );
	}

	matrix4x4_t<T> ToMatrix() {
		vector4_t<T> quatVec = vector4_t<T>( x, y, z, w );
		quatVec = Normalize( quatVec );

		const T xx = x * x,
								  xy = x * y,
								  xz = x * z,
								  xw = x * w,

								  yy = y * y,
								  yz = y * z,
								  yw = y * w,

								  zz = z * z,
								  zw = z * w;

		const matrix4x4_t<T> mat = matrix4x4_t<T>( 
			(T)( 1 - 2 * ( zz + yy ) ), (T)( 2 * ( xz - yw ) ),     (T)( 2 *( xy + zw ) ),      (T)0,
			(T)( 2 * ( xz + yw ) ),     (T)( 1 - 2 * ( xx + yy ) ), (T)( 2 * ( yz - xw ) ),     (T)0,
			(T)( 2 * ( xy - zw ) ),     (T)( 2 * ( yz + xw ) ),     (T)( 1 - 2 * ( xx + zz ) ), (T)0,
			(T)0,                       (T)0,                       (T)0,                       (T)1   
		);

		return mat;
	}
};
