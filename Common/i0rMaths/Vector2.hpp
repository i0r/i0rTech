#pragma once 
template<typename T> struct vector3_t;
template<typename T> struct vector4_t;

template<typename T> struct ALIGN( 16 ) vector2_t {
	ALIGNED_OBJECT( vector2_t, 16 )

	union {
		struct {
			T x;
			T y;
		};

		__m128 s;
	};

//=====================================================================================================================

	INLINE constexpr T operator[] ( const i32 i ) const  {
		return ( !i ) ? x : y;
	}

//=====================================================================================================================

	explicit vector2_t()  : s( _mm_setzero_ps() ) {

	}

	explicit vector2_t( const T f )  : s( _mm_set_ps( ( T )0, ( T )0, f, f ) ) {

	}

	explicit vector2_t( const T f[2] )  : s( _mm_set_ps( ( T )0, ( T )0, f[1], f[0] ) ) {

	}

	explicit vector2_t( const T &nx, const T &ny )  : s( _mm_set_ps( ( T )0, ( T )0, (T)ny, (T)nx ) ) {

	}

	explicit vector2_t( const vector4_t<T> &nv )  : s( _mm_set_ps( ( T )0.0f, ( T )0.0f, nv.y, nv.x ) ) {

	}

	explicit vector2_t( const vector3_t<T> &nv )  : s( _mm_set_ps( ( T )0, ( T )0, nv.y, nv.x ) ) {

	}

	explicit vector2_t( const __m128 &ns )  : s( ns ) {

	}
	
	~vector2_t() {
		s = _mm_set_ps1( 0.0f ); 
	}

//=====================================================================================================================

	INLINE vector2_t<T> &operator = ( const vector4_t<T> &v ) {
		s = _mm_set_ps( (T)0, (T)0, v.y, v.x );

		return *this;
	}

	INLINE vector2_t<T> &operator = ( const vector3_t<T> &v ) {
		s = _mm_set_ps( (T)0, (T)0, v.y, v.x );

		return *this;
	}
	
	INLINE vector2_t<T> &operator = ( const vector2_t<T> &v ) {
		s = v.s;

		return *this;
	}

	INLINE vector2_t<T> &operator = ( const T &f ) {
		s = _mm_set_ps( (T)0, (T)0, f, f );

		return *this;
	}

	INLINE vector2_t<T> &operator = ( const T fa[2] ) {
		s = _mm_set_ps( (T)0, (T)0, fa[1], fa[0] );

		return *this;
	}

//=====================================================================================================================

	INLINE vector2_t<T> &operator + ( const vector2_t<T> &v ) {
		return vector2_t<T>( _mm_add_ps( s, v.s ) );
	}

	INLINE vector2_t<T> &operator + ( const T &f ) {
		return vector2_t<T>( _mm_set_ps( 0.0f, 0.0f, y + f, x + f ) );
	}

	INLINE vector2_t<T> &operator - ( const vector2_t<T> &v ) {
		return vector2_t<T>( _mm_sub_ps( s, v.s ) );
	}

	INLINE vector2_t<T> &operator - ( const T &f ) {
		return vector2_t<T>( _mm_set_ps( 0.0f, 0.0f, y - f, x - f ) );
	}

	INLINE vector2_t<T> &operator * ( const vector2_t<T> &v ) {
		return vector2_t<T>( _mm_mul_ps( s, v.s ) );
	}

	INLINE vector2_t<T> &operator * ( const T &f ) {
		vector2_t<T>* v = new vector2_t<T>( _mm_set_ps( 0.0f, 0.0f, y * f, x * f ) );

		return *v;
	}

	INLINE vector2_t<T> &operator / ( const vector2_t<T> &v ) {
		return vector2_t<T>( _mm_div_ps( s, v.s ) );
	}

	INLINE vector2_t<T> &operator / ( const T &f ) {
		return *( this * ( 1.0 / f ) );
	}

//=====================================================================================================================

	INLINE vector2_t<T> &operator - () {
		return vector2_t<T>( _mm_set_ps( 0.0f, 0.0f, -y, -x ) );
	}

	INLINE vector2_t<T> &operator ! () {
		return -*this;
	}

//=====================================================================================================================

	INLINE vector2_t<T> &operator += ( const vector2_t<T> &v ) {
		return *( this + v );
	}

	INLINE vector2_t<T> &operator += ( const T &f ) {
		return *( this + f );
	}

	INLINE vector2_t<T> &operator -= ( const vector2_t<T> &v ) {
		return *( this - v );
	}

	INLINE vector2_t<T> &operator -= ( const T &f ) {
		return *( this - f );
	}

	INLINE vector2_t<T> &operator *= ( const vector2_t<T> &v ) {
		return *( this * v );
	}

	INLINE vector2_t<T> &operator *= ( const T &f ) {
		return *( this * f );
	}

	INLINE vector2_t<T> &operator /= ( const vector2_t<T> &v ) {
		return *( this / v );
	}

	INLINE vector2_t<T> &operator /= ( const T &f ) {
		return *( this / f );
	}

//=====================================================================================================================

	/*====================
	vector2_t::Dot()
	Returns the dot product of two 2D vector
	====================*/
	INLINE T Dot( const vector2_t<T> &v ) {
		return (T)x*v.x + y*v.y;
	}

	/*====================
	vector2_t::Normalize()
	Normalizes this 2D vector
	====================*/
	INLINE void Normalize() {
		s = _mm_mul_ps( this.s, _mm_rsqrt_ps( _mm_dp_ps( this.s, this.s, 0x7F ) ) );
	}
};

/*====================
::Dot()
Returns the dot product of two 2D vector
====================*/
template<typename T>
INLINE T Dot( const vector2_t<T> &u, const vector2_t<T> &v ) {
	return (T)u.x * v.x + u.y * v.y;
}

/*====================
::Normalize()
Normalizes a 2D vector
====================*/
template<typename T>
INLINE vector2_t<T> Normalize( const vector2_t<T> &v ) {
	return _mm_mul_ps( v.s, _mm_rsqrt_ps( _mm_dp_ps( v.s, v.s, 0x7F ) ) );
}

/*====================
::Cross()
Returns the cross product of two 2D vector
====================*/
template<typename T>
INLINE T Cross( const vector2_t<T> &a, const vector2_t<T> &b ) {
	return (T)a.x * b.y - b.y * a.x;
}
