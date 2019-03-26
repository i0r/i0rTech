#pragma once

template<typename T> struct vector2_t;
template<typename T> struct vector3_t;

template<typename T> struct ALIGN( 16 ) vector4_t {
	ALIGNED_OBJECT( vector4_t, 16 )

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

	INLINE T &operator[] ( const i32 &i ) {
			return ( i == 0 ) ? x : ( i == 1 ) ? y : ( i == 2 ) ? z : w;
	}

//=====================================================================================================================

	explicit vector4_t() : s( _mm_setzero_ps() ) {

	}

	explicit vector4_t( const T &f )  : s( _mm_set_ps( f, f, f, f ) ) {

	}

	explicit vector4_t( const T f[4] )  : s( _mm_set_ps( (f32)f[3], (f32)f[2], (f32)f[1], (f32)f[0] ) ) {

	}

	explicit vector4_t( const T &nx, const T &ny, const T &nz, const T &nw )  
		: s( _mm_set_ps( (f32)nw, (f32)nz, (f32)ny, (f32)nx ) ) {

	}

	explicit vector4_t( const vector3_t<T> &nv )  
		: s( _mm_set_ps( 0.0f, (f32)nv.z, (f32)nv.y, (f32)nv.x ) ) {

	}

	explicit vector4_t( const vector3_t<T> &nv, const T &nw )  
		: s( _mm_set_ps( (f32)nw, (f32)nv.z, (f32)nv.y, (f32)nv.x ) ) {

	}

	explicit vector4_t( const vector2_t<T> &nv )  
		: s( _mm_set_ps( 0.0f, 0.0f, (f32)nv.y, (f32)nv.x ) ) {

	}

	explicit vector4_t( const vector2_t<T> &nv, const vector2_t<T> &nv2 )  
		: s( _mm_set_ps( (f32)nv2.y, (f32)nv2.x, (f32)nv.y, (f32)nv.x ) ) {

	}

	explicit vector4_t( const __m128 &ns )  : s( ns ) {

	}

	~vector4_t() {
		s = _mm_set_ps1( 0.0f );
	}

//=====================================================================================================================

	INLINE vector4_t<T> &operator = ( const vector4_t<T> &v ) {
		s = v.s;

		return *this;
	}

	INLINE vector4_t<T> &operator = ( const vector3_t<T> &v ) {
		s = v.s;

		return *this;
	}

	INLINE vector4_t<T> &operator = ( const vector2_t<T> &v ) {
		s = v.s;

		return *this;
	}

	INLINE vector4_t<T> &operator = ( const T &f ) {
		s = _mm_set_ps( f, f, f, f );

		return *this;
	}

	INLINE vector4_t<T> &operator = ( const T fa[4] ) {
		s = _mm_set_ps( (f32)fa[3], (f32)fa[2], (f32)fa[1], (f32)fa[0] );

		return *this;
	}

//=====================================================================================================================

	INLINE vector4_t<T> &operator + ( const vector4_t<T> &v ) {
		return vector4_t<T>( _mm_add_ps( s, v.s ) );
	}

	INLINE vector4_t<T> &operator + ( const T &f ) {
		return vector4_t<T>( _mm_set_ps( w + f, z + f, y + f, x + f ) );
	}

	INLINE vector4_t<T> operator - ( const vector4_t<T> &v ) {
		return vector4_t<T>( _mm_sub_ps( s, v.s ) );
	}

	INLINE vector4_t<T> operator - ( const T &f ) {
		return vector4_t<T>( _mm_set_ps( w - f, z - f, y - f, x - f ) );
	}

	INLINE vector4_t<T> operator * ( const vector3_t<T> &v ) {
	return vector4_t<T>( _mm_mul_ps( s, v.s ) );
	}

	INLINE vector4_t<T> operator * ( const vector4_t<T> &v ) {
		return vector4_t<T>( _mm_mul_ps( s, v.s ) );
	}

	INLINE vector4_t<T> operator * ( const T &f ) {
		return vector4_t<T>( _mm_set_ps( w * f, z * f, y * f, x * f ) );
	}

	INLINE vector4_t<T> &operator / ( const vector4_t<T> &v ) {
		return vector4_t<T>( _mm_div_ps( s, v.s ) );
	}

	INLINE vector4_t<T> operator / ( const T &f ) {
		return ( *this * ( (T)1.0 / f ) );
	}

//=====================================================================================================================

	INLINE vector4_t<T> operator - () {
		x = -x;
		y = -y;
		z = -z;
		w = -w;

		return *this;
	}

	INLINE vector4_t<T> &operator ! () {
		return -*this;
	}

//=====================================================================================================================

	INLINE vector4_t<T> &operator += ( const vector4_t<T> &v ) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;

		return *this;
	}

	INLINE vector4_t<T> &operator += ( const T &f ) {
		x += f;
		y += f;
		z += f;
		w += f;

		return *this;
	}

	INLINE vector4_t<T> &operator -= ( const vector4_t<T> &v ) {
		return *this - v;
	}

	INLINE vector4_t<T> &operator -= ( const T &f ) {
		return *this - f;
	}

	INLINE vector4_t<T> operator *= ( const vector4_t<T> &v ) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
	}

	INLINE vector4_t<T> operator *= ( const vector3_t<T> &v ) {
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
	}

	INLINE vector4_t<T> operator *= ( const T &f ) {
		x *= f;
		y *= f;
		z *= f;
		w *= f;

		return *this;
	}

	INLINE vector4_t<T> &operator /= ( const vector4_t<T> &v ) {
		return *this / v;
	}

	INLINE vector4_t<T> operator /= ( const T &f ) {
		return *this / f;
	}

//=====================================================================================================================

	/*====================
	vector4_t::Dot()
		Returns the dot product of two 4D vector
	====================*/
	INLINE T Dot( const vector4_t<T> &v ) {
		return (T)x * v.x + y * v.y + z * v.z + w * v.w;
	}

	/*====================
	vector4_t::Cross()
		Returns the cross product of two 4D vector
	====================*/
	INLINE vector4_t<T> Cross( const vector4_t<T> &v ) {
		return vector4_t<T>( _mm_set_ps( 0.0f, 
						x * v.y - y * v.x, 
						z * v.x - x * v.z, 
						y * v.z - z * v.y ) );
	}
};

//=====================================================================================================================

/*====================
::Dot()
	Returns the dot product of two 4D vector
====================*/
template<typename T> 
INLINE T Dot( const vector4_t<T> &u, const vector4_t<T> &v ) {
	return (T)u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
}

/*====================
::Cross()
	Returns the cross product of two 4D vector
====================*/
template<typename T> 
INLINE vector4_t<T> Cross( const vector4_t<T> &u, const vector4_t<T> &v ) {
	return vector4_t<T>( _mm_set_ps( 0.0f, 
								                          u.x * v.y - u.y * v.x, 
								                          u.z * v.x - u.x * v.z, 
								                          u.y * v.z - u.z * v.y ) );
}

template<typename T>
vector4_t<T> Normalize( vector4_t<T> v ) {
	vector4_t<T> sqrt = vec4f( _mm_mul_ps( v.s, v.s ) );
	vector4_t<T> t = sqrt;

	sqrt.s = _mm_shuffle_ps( sqrt.s, sqrt.s, 0x1B );
	t.s = _mm_add_ps( t.s, sqrt.s );

	sqrt.s = _mm_shuffle_ps( sqrt.s, sqrt.s, 0x4E );
	t.s = _mm_add_ps( t.s, sqrt.s );

	sqrt.s = _mm_shuffle_ps( sqrt.s, sqrt.s, 0x1B );
	t.s = _mm_add_ps( t.s, sqrt.s );

	t.s = _mm_sqrt_ps( t.s );

	return vector4_t<T>( _mm_mul_ps( v.s, t.s ) );
}


template<typename T>
vector4_t<T> Round( vector4_t<T> v ) {
	vector4_t<T> roundedVector = vector4_t<T>();
	roundedVector.x = round( v.x );
	roundedVector.y = round( v.y );
	roundedVector.z = round( v.z );
	roundedVector.w = round( v.w );

	return roundedVector;
}

template<typename T>
INLINE vector4_t<T> Min( const vector4_t<T> &u, const vector4_t<T> &v ) {
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
INLINE vector4_t<T> Max( const vector4_t<T> &u, const vector4_t<T> &v ) {
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
