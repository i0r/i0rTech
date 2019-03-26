#pragma once

	template<typename T> struct vector4_t;
	template<typename T> struct vector2_t;

template<typename T> struct ALIGN( 16 ) vector3_t {
	ALIGNED_OBJECT( vector3_t, 16 )

	union {
		struct {
			T x;
			T y;
			T z;
		};

		__m128 s;
	};

//=====================================================================================================================

	INLINE T &operator[] ( const i32 i ) {
		return ( !i ) ? x : ( i == 1 ) ? y : z;
	}

//=====================================================================================================================

	explicit vector3_t()  : s( _mm_setzero_ps() ) {

	}

	explicit vector3_t( const T &f )  : s( _mm_set_ps( 0.0f, f, f, f ) ) {

	}

	explicit vector3_t( const T f[3] )  : s( _mm_set_ps( 0.0f, f[2], f[1], f[0] ) ) {

	}

	explicit vector3_t( const T &nx, const T &ny, const T &nz )  
		: s( _mm_set_ps( 0.0f, (f32)nz, (f32)ny, (f32)nx ) ) {

	}

	explicit vector3_t( const vector4_t<T> &nv )  : s( _mm_set_ps( 0.0f, nv.z, nv.y, nv.x ) ) {

	}

	explicit vector3_t( const vector2_t<T> &nv )  : s( _mm_set_ps( 0.0f, 0.0f, nv.y, nv.x ) ) {

	}

	explicit vector3_t( const __m128 &ns )  : s( ns ) {

	}

	~vector3_t() {
		s = _mm_set_ps1( 0.0f );
	}

//=====================================================================================================================

	INLINE vector3_t<T> &operator = ( const vector4_t<T> &v ) {
		s = _mm_set_ps( 0.0f, v.z, v.y, v.x );

		return *this;
	}

	INLINE vector3_t<T> &operator = ( const vector3_t<T> &v ) {
		s = v.s;

		return *this;
	}

	INLINE vector3_t<T> &operator = ( const vector2_t<T> &v ) {
		s = v.s;

		return *this;
	}

	INLINE vector3_t<T> &operator = ( const T &f ) {
		s = _mm_set_ps( f, f, f, f );

		return *this;
	}

	INLINE vector3_t<T> &operator = ( const T fa[3] ) {
		s = _mm_set_ps( 0.0f, fa[2], fa[1], fa[0] );

		return *this;
	}

//=====================================================================================================================

	INLINE vector3_t<T> operator + ( vector3_t<T> v ) {
		return vector3_t<T>( x + v.x, y + v.y, z + v.z );
	}

	INLINE vector3_t<T> &operator + ( const T &f ) {
		return vector3_t<T>( _mm_set_ps( 0.0f, z + f, y + f, x + f ) );
	}

	INLINE vector3_t<T> operator - ( vector3_t<T> v ) {
		return vector3_t<T>( _mm_sub_ps( s, v.s ) );
	}

	INLINE vector3_t<T> &operator - ( const T &f ) {
		return vector3_t<T>( _mm_set_ps( 0.0f, z - f, y - f, x - f ) );
	}

	INLINE vector3_t<T> operator * ( vector3_t<T> v ) {
		return vector3_t<T>( _mm_mul_ps( s, v.s ) );
	}

	INLINE vector3_t<T> operator * ( const T &f ) {
		return vector3_t<T>( _mm_set_ps( 0.0f, z * f, y * f, x * f ) );
	}

	INLINE vector3_t<T> &operator / ( const vector3_t<T> &v ) {
		return vector3_t<T>( _mm_div_ps( s, v.s ) );
	}

	INLINE vector3_t<T> operator / ( const T &f ) {
		T fi = ( (T)1 / f );
		return vector3_t<T>( _mm_set_ps( 0.0f, z * fi, y * fi, x * fi ) );
	}

//=====================================================================================================================

	INLINE vector3_t<T> operator - () const  {
		return vector3_t<T>( _mm_set_ps( 0.0f, -z, -y, -x ) );
	}

	INLINE vector3_t<T> operator ! () const  {
		return -*this;
	}

//=====================================================================================================================

	INLINE vector3_t<T> &operator += ( const T &f ) {
		return *( this + f );
	}

	INLINE vector3_t<T> operator -= ( vector3_t<T> v ) {
		return *this - v;
	}

	INLINE vector3_t<T> &operator -= ( const T &f ) {
		return *( this - f );
	}

	INLINE vector3_t<T> &operator *= ( const vector3_t<T> &v ) {
		return *( this * v );
	}

	INLINE vector3_t<T> &operator *= ( const T &f ) {
	z *= f;
	y *= f;
	x *= f;

	return *this;
	}

	INLINE vector3_t<T> &operator /= ( const vector3_t<T> &v ) {
		return *( this / v );
	}

	INLINE vector3_t<T> &operator /= ( const T &f ) {
		z /= f;
		y /= f;
		x /= f;

		return *this;
	}

//=====================================================================================================================

	/*====================
	vector3_t::Dot()
	Returns the dot product of two 3D vector
	====================*/
	INLINE T Dot( const vector3_t<T> &v ) {
		return (T)x*v.x + y*v.y + z*v.z; 
	}

	/*====================
	vector3_t::Normalize()
	Normalizes this 3D vector
	====================*/
	INLINE void Normalize() {
		s =  _mm_mul_ps( this->s, _mm_rsqrt_ps( _mm_dp_ps( this->s, this->s, 0x7F ) ) );
	}
};

	template<typename T>
	INLINE vector3_t<T> Exp( const vector3_t<T> &u ) {
		return vector3_t<T>( _mm_set_ps( 0.0f, exp( u.z ), exp( u.y ), exp( u.x ) ) );
	}

	template<typename T>
	INLINE vector3_t<T> Pow( vector3_t<T> u, vector3_t<T> v ) {
		return vector3_t<T>( _mm_set_ps( 0.0f, pow( u.z, v.z ), pow( u.y, v.y ), 
			pow( u.x, v.x ) ) );
	}

	template<typename T>
	INLINE vector3_t<T> operator / ( const T &f, const vector3_t<T> &u ) {
		T fi = ( ( T )1 / f );
		return vector3_t<T>( _mm_set_ps( 0.0f, u.z * fi, u.y * fi, u.x * fi ) );
	}

	template<typename T>
	INLINE vector3_t<T> operator * ( const T &f, const vector3_t<T> &u ) {
		return vector3_t<T>( _mm_set_ps( 0.0f, u.z * f, u.y * f, u.x * f ) );
	}

	template<typename T>
	INLINE vector3_t<T> operator * ( const vector3_t<T> &u, const T &f ) {
		return vector3_t<T>( _mm_set_ps( 0.0f, u.z * f, u.y * f, u.x * f ) );
	}

	template<typename T>
	INLINE vector3_t<T> operator + ( const T &f, vector3_t<T> u ) {
		return vector3_t<T>( _mm_set_ps( 0.0f, u.z + f, u.y + f, u.x + f ) );
	}

	template<typename T>
	INLINE vector3_t<T> operator + ( const vector3_t<T> &u, const T &f ) {
		return vector3_t<T>( _mm_set_ps( 0.0f, u.z + f, u.y + f, u.x + f ) );
	}

	template<typename T> 
	INLINE vector3_t<T> operator - ( const vector3_t<T> &u, const vector3_t<T> &v ) {
		return vector3_t<T>( _mm_sub_ps( u.s, v.s ) );
	}

//=====================================================================================================================

template<typename T> 
INLINE vector3_t<T> operator += ( vector3_t<T> &u, vector3_t<T> &v ) {
	u = u + v;
	return u;
}

/*====================
::Dot()
Returns the dot product of two 3D vector
====================*/
template<typename T> 
INLINE T Dot( const vector3_t<T> &u, const vector3_t<T> &v ) {
	return (T)u.x * v.x + u.y * v.y + u.z * v.z;
}

/*====================
::Normalize()
Normalizes a 3D vector
====================*/
template<typename T>
INLINE vector3_t<T> Normalize( const vector3_t<T> &v ) {
	return vector3_t<T>( _mm_mul_ps( v.s, _mm_rsqrt_ps( _mm_dp_ps( v.s, v.s, 0x7F ) ) ) );
}

/*====================
::Cross()
Returns the cross product of two 3D vector
====================*/
template<typename T>
INLINE vector3_t<T> Cross( const vector3_t<T> &u, const vector3_t<T> &v ) {
	return vector3_t<T>( _mm_set_ps( 0.0f, u.x * v.y - u.y * v.x, u.z * v.x - u.x * v.z, u.y * v.z - u.z * v.y ) );
}

template<typename T>
INLINE vector3_t<T> Min( const vector3_t<T> &u, const vector3_t<T> &v ) {
	union {
		struct {
			T x;
			T y;
			T z;
			T w;
		};

		__m128 s;
	} cmpResult;

	cmpResult.s = _mm_cmpge_ps( u.s, v.s );

	i32 test = 0;
	
	if( cmpResult.x != 0x0 ) test++;
	if( cmpResult.y != 0x0 ) test++;
	if( cmpResult.z != 0x0 ) test++;
	if( cmpResult.w != 0x0 ) test++;
	
	if( test <= 2 ) return u;
	else return v;
}

template<typename T>
INLINE vector3_t<T> Max( const vector3_t<T> &u, const vector3_t<T> &v ) {
	union {
		struct {
			T x;
			T y;
			T z;
			T w;
		};

		__m128 s;
	} cmpResult;

	cmpResult.s = _mm_cmpge_ps( u.s, v.s );

	i32 test = 0;

	if( !cmpResult.x ) test++;
	if( !cmpResult.y ) test++;
	if( !cmpResult.z ) test++;
	if( !cmpResult.w ) test++;

	if( test <= 2 ) return u;
	else return v;
}

template<typename T>
INLINE T Length( const vector3_t<T> &v ) {
	return _mm_cvtss_f32( _mm_sqrt_ss( _mm_dp_ps( v.s, v.s, 0x71 ) ) );
}
