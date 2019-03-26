#pragma once
								                                                                                                               
template<typename T> struct ALIGN( 16 ) matrix3x3_t {
	ALIGNED_OBJECT( matrix3x3_t, 16 )

	vector3_t<T> r0;
	vector3_t<T> r1;
	vector3_t<T> r2;

//=====================================================================================================================

	INLINE constexpr vector3_t<T> &operator[] ( const i32 i ) const {
		return ( i >= 0 && i <= 3 ) ? ( i == 0 ) ? r0 : ( i == 1 ) ? r1 : r2  : r2;
	}

//=====================================================================================================================

	explicit matrix3x3_t()  : r0( vector3_t<T>() ),
								                           r1( vector3_t<T>() ), 
								                           r2( vector3_t<T>() ) {

	}

	explicit matrix3x3_t( const vector3_t<T> &nr0, const vector3_t<T> &nr1, const vector3_t<T> &nr2 )  
		: r0( nr0 ), 
				r1( nr1 ), 
				r2( nr2 ) {

	}

	explicit matrix3x3_t( const T &nr0c0, const T &nr0c1, const T &nr0c2,
								               const T &nr1c0, const T &nr1c1, const T &nr1c2,
								               const T &nr2c0, const T &nr2c1, const T &nr2c2 )  
		: r0( vector3_t<T>( nr0c0, nr0c1, nr0c2 ) ), 
				r1( vector3_t<T>( nr1c0, nr1c1, nr1c2 ) ), 
				r2( vector3_t<T>( nr2c0, nr2c1, nr2c2 ) ) {

	}

	~matrix3x3_t() {
		r0 = vector3_t<T>();
		r1 = vector3_t<T>();
		r2 = vector3_t<T>();
	}

//=====================================================================================================================

	INLINE matrix3x3_t<T> &operator = ( const matrix3x3_t<T>& m ) {
		r0 = m.r0;
		r1 = m.r1;
		r2 = m.r2;

		return *this;
	}

//=====================================================================================================================
		
	/*====================
	matrix3x3_t::Identity()
		Sets a 3x3 matrix identity
	====================*/
	INLINE void Identity() {
		r0 = vector3_t<T>( (T)1, (T)0, (T)0 );
		r1 = vector3_t<T>( (T)0, (T)1, (T)0 );
		r2 = vector3_t<T>( (T)0, (T)0, (T)1 );
	}

	/*====================
	matrix3x3_t::Transpose()
		Transposes a 3x3 matrix
	====================*/
	INLINE void Transpose() {
		const matrix3x3_t<T> tempMat = this;

		r0 = vector3_t<T>( tempMat.r0.x, tempMat.r1.x, tempMat.r2.x );
		r1 = vector3_t<T>( tempMat.r0.y, tempMat.r1.y, tempMat.r2.y );
		r2 = vector3_t<T>( tempMat.r0.z, tempMat.r1.z, tempMat.r2.z );
	}
};

template<typename T>
INLINE matrix3x3_t<T> &operator * ( const matrix3x3_t<T>& m1, const matrix3x3_t<T>& m2 ) {
	static matrix3x3_t<T> m3 =  matrix3x3_t<T>();

	vector3_t<T> m1c0 = vector3_t<T>( m1.r0.x, m1.r1.x, m1.r2.x );
	vector3_t<T> m1c1 = vector3_t<T>( m1.r0.y, m1.r1.y, m1.r2.y );
	vector3_t<T> m1c2 = vector3_t<T>( m1.r0.z, m1.r1.z, m1.r2.z );

	m3.r0 = vector3_t<T>( Dot( m2.r0, m1c0 ), Dot( m2.r0, m1c1 ), Dot( m2.r0, m1c2 ) );
	m3.r1 = vector3_t<T>( Dot( m2.r1, m1c0 ), Dot( m2.r1, m1c1 ), Dot( m2.r1, m1c2 ) );
	m3.r2 = vector3_t<T>( Dot( m2.r2, m1c0 ), Dot( m2.r2, m1c1 ), Dot( m2.r2, m1c2 ) );

	return m3;
}
