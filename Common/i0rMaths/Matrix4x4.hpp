#pragma once

template<typename T> struct ALIGN( 16 ) matrix4x4_t {
	ALIGNED_OBJECT( matrix4x4_t, 16 )

	vector4_t<T> r0;
	vector4_t<T> r1;
	vector4_t<T> r2;
	vector4_t<T> r3;

//=====================================================================================================================

	INLINE constexpr T operator[] ( const i32 i ) const  {
		if( i < 4 )       return r0[i];
		else if( i < 8 )  return r1[i];
		else if( i < 12 ) return r2[i];
		else if( i < 16 ) return r3[i];
		else              return (T)0;
	}

	INLINE vector4_t<T> GetRow( const i32 i ) const  {
		return ( !i ) ? r0 : ( i == 1 ) ? r1 : ( i == 2 ) ? r2 : r3;
	}

//=====================================================================================================================

	explicit matrix4x4_t()  : r0( vector4_t<T>() ), 
								                           r1( vector4_t<T>() ), 
								                           r2( vector4_t<T>() ),
								                           r3( vector4_t<T>() ) {

	}

	explicit matrix4x4_t( const T &v )  : r0( vector4_t<T>( v, v, v, v ) ),
								                                       r1( vector4_t<T>( v, v, v, v ) ),
								                                       r2( vector4_t<T>( v, v, v, v ) ),
								                                       r3( vector4_t<T>( v, v, v, v ) ) {

	}

	explicit matrix4x4_t( const vector4_t<T> &nr0, const vector4_t<T> &nr1, const vector4_t<T> &nr2, 
								               const vector4_t<T> &nr3 )  : r0( nr0 ), 
								                                                    r1( nr1 ), 
								                                                    r2( nr2 ), 
								                                                    r3( nr3 ) {

	}

	explicit matrix4x4_t( const T &nr0c0, const T &nr0c1, const T &nr0c2, const T &nr0c3,
								               const T &nr1c0, const T &nr1c1, const T &nr1c2, const T &nr1c3,
								               const T &nr2c0, const T &nr2c1, const T &nr2c2, const T &nr2c3,
								               const T &nr3c0, const T &nr3c1, const T &nr3c2, const T &nr3c3 )  
	: r0( vector4_t<T>( nr0c0, nr0c1, nr0c2, nr0c3 ) ),
			r1( vector4_t<T>( nr1c0, nr1c1, nr1c2, nr1c3 ) ),
			r2( vector4_t<T>( nr2c0, nr2c1, nr2c2, nr2c3 ) ),
			r3( vector4_t<T>( nr3c0, nr3c1, nr3c2, nr3c3 ) ) {
		
	}

	~matrix4x4_t() {
		r0 = vector4_t<T>();
		r1 = vector4_t<T>();
		r2 = vector4_t<T>();
		r3 = vector4_t<T>();
	}


//=====================================================================================================================

	matrix4x4_t<T> &operator = ( const matrix4x4_t<T>& m ) {
		r0 = m.r0;
		r1 = m.r1;
		r2 = m.r2;
		r3 = m.r3;

		return *this;
	}

	matrix4x4_t<T> &operator *= ( const T& f ) {
		this->r0.x *= f; this->r0.y *= f; this->r0.z *= f; this->r0.w *= f;
		this->r1.x *= f; this->r1.y *= f; this->r1.z *= f; this->r1.w *= f;
		this->r2.x *= f; this->r2.y *= f; this->r2.z *= f; this->r2.w *= f;
		this->r3.x *= f; this->r3.y *= f; this->r3.z *= f; this->r3.w *= f;

		return *this;
	}

//=====================================================================================================================

	void Identity() {
		r0 = vector4_t<T>( (T)1, (T)0, (T)0, (T)0 );
		r1 = vector4_t<T>( (T)0, (T)1, (T)0, (T)0 );
		r2 = vector4_t<T>( (T)0, (T)0, (T)1, (T)0 );
		r3 = vector4_t<T>( (T)0, (T)0, (T)0, (T)1 );
	}

	void Translate( const vector3_t<T> translationVector ) {
		r3.x = translationVector.x;
		r3.y = translationVector.y;
		r3.z = translationVector.z;
	}

	void Transpose() {
		const matrix4x4_t<T> tempMat = *this;

		r0 = vector4_t<T>( tempMat.r0.x, tempMat.r1.x, tempMat.r2.x, tempMat.r3.x );
		r1 = vector4_t<T>( tempMat.r0.y, tempMat.r1.y, tempMat.r2.y, tempMat.r3.y );
		r2 = vector4_t<T>( tempMat.r0.z, tempMat.r1.z, tempMat.r2.z, tempMat.r3.z );
		r2 = vector4_t<T>( tempMat.r0.w, tempMat.r1.w, tempMat.r2.w, tempMat.r3.w );
	}

	matrix4x4_t<T> &Scale( const T &f ) {
		r0 *= f;
		r1 *= f;
		r2 *= f;

		return *this;
	}

	void Scale( const vector3_t<T> &v ) {
		r0 *= v;
		r1 *= v;
		r2 *= v;
	}

	INLINE void Rotate( quaternion_t<T> q ) {
		matrix4x4_t<T> rotMatrix = q.ToMatrix();
		*this = *this * rotMatrix;
	}

	void Clear() {
		r0 = vector4_t<T>( (T)0 );
		r1 = vector4_t<T>( (T)0 );
		r2 = vector4_t<T>( (T)0 );
		r3 = vector4_t<T>( (T)0 );
	}
};

template<typename T>
constexpr vector4_t<T> operator * ( const matrix4x4_t<T> &m1, const vector4_t<T> &v1 ) {
	return vector4_t<T>( m1.r0.x * v1.x + m1.r0.y * v1.y + m1.r0.z * v1.z + m1.r0.w * v1.w,
								              m1.r1.x * v1.x + m1.r1.y * v1.y + m1.r1.z * v1.z + m1.r1.w * v1.w,
								              m1.r2.x * v1.x + m1.r2.y * v1.y + m1.r2.z * v1.z + m1.r2.w * v1.w,
								              m1.r3.x * v1.x + m1.r3.y * v1.y + m1.r3.z * v1.z + m1.r3.w * v1.w );
}

template<typename T>
matrix4x4_t<T> operator * ( vector4_t<T>& v1 ) {
	matrix4x4_t<T> m3 = matrix4x4_t<T>();

	m3.r0 = vector4_t<T>( Dot( m3.r0, m1c0 ), Dot( m2.r0, m1c1 ), Dot( m2.r0, m1c2 ), Dot( m2.r0, m1c3 ) );
	m3.r1 = vector4_t<T>( Dot( m2.r1, m1c0 ), Dot( m2.r1, m1c1 ), Dot( m2.r1, m1c2 ), Dot( m2.r1, m1c3 ) );
	m3.r2 = vector4_t<T>( Dot( m2.r2, m1c0 ), Dot( m2.r2, m1c1 ), Dot( m2.r2, m1c2 ), Dot( m2.r2, m1c3 ) );
	m3.r3 = vector4_t<T>( Dot( m2.r3, m1c0 ), Dot( m2.r3, m1c1 ), Dot( m2.r3, m1c2 ), Dot( m2.r3, m1c3 ) );

	return m3;
}

template<typename T>
matrix4x4_t<T> operator * ( const matrix4x4_t<T>& m1, const matrix4x4_t<T>& m2 ) {
	matrix4x4_t<T> m3 = matrix4x4_t<T>();

	const vector4_t<T> m1c0 = vector4_t<T>( m1.r0.x, m1.r1.x, m1.r2.x, m1.r3.x );
	const vector4_t<T> m1c1 = vector4_t<T>( m1.r0.y, m1.r1.y, m1.r2.y, m1.r3.y );
	const vector4_t<T> m1c2 = vector4_t<T>( m1.r0.z, m1.r1.z, m1.r2.z, m1.r3.z );
	const vector4_t<T> m1c3 = vector4_t<T>( m1.r0.w, m1.r1.w, m1.r2.w, m1.r3.w );

	m3.r0 = vector4_t<T>( Dot( m2.r0, m1c0 ), Dot( m2.r0, m1c1 ), Dot( m2.r0, m1c2 ), Dot( m2.r0, m1c3 ) );
	m3.r1 = vector4_t<T>( Dot( m2.r1, m1c0 ), Dot( m2.r1, m1c1 ), Dot( m2.r1, m1c2 ), Dot( m2.r1, m1c3 ) );
	m3.r2 = vector4_t<T>( Dot( m2.r2, m1c0 ), Dot( m2.r2, m1c1 ), Dot( m2.r2, m1c2 ), Dot( m2.r2, m1c3 ) );
	m3.r3 = vector4_t<T>( Dot( m2.r3, m1c0 ), Dot( m2.r3, m1c1 ), Dot( m2.r3, m1c2 ), Dot( m2.r3, m1c3 ) );

	return m3;
}

template<typename T>
INLINE matrix4x4_t<T> Translate( const vector4_t<T> &v ) {
	return Translate( v.x, v.y, v.z );
}

template<typename T>
INLINE matrix4x4_t<T> Translate( const vector3_t<T> &v ) {
	return Translate( v.x, v.y, v.z );
}

/*====================
::Translate()
Translates a 4x4 matrix according to 3 points
====================*/
template<typename T>
matrix4x4_t<T> Translate( const T &x, const T &y, const T &z ) {
	matrix4x4_t<T> m = matrix4x4_t<T>();

	m.r0.w = x;
	m.r1.w = y;
	m.r2.w = z;

	return m;
}

template<typename T>
matrix4x4_t<T> Inverse( const matrix4x4_t<T> matrix ) {
	matrix4x4_t<T> inversedMatrix = matrix4x4_t<T>( (T)0 );

	T coFactors[12] = {
		matrix.r2.z * matrix.r3.w,
		matrix.r2.w * matrix.r3.z,

		matrix.r2.y * matrix.r3.z,
		matrix.r2.z * matrix.r3.y,

		matrix.r2.y * matrix.r3.z,
		matrix.r2.z * matrix.r3.y,

		matrix.r2.x * matrix.r3.w,
		matrix.r2.w * matrix.r3.x,

		matrix.r2.x * matrix.r3.z,
		matrix.r2.z * matrix.r3.x,

		matrix.r2.x * matrix.r3.y,
		matrix.r2.y * matrix.r3.x,
	};

	inversedMatrix.r0.x  = coFactors[0] * matrix.r1.y + coFactors[3] * matrix.r1.z + coFactors[4] * matrix.r1.w;
	inversedMatrix.r0.x -= coFactors[1] * matrix.r1.y + coFactors[2] * matrix.r1.z + coFactors[5] * matrix.r1.w;

	inversedMatrix.r1.x  = coFactors[1] * matrix.r1.x + coFactors[6] * matrix.r1.z + coFactors[9] * matrix.r1.w;
	inversedMatrix.r1.x -= coFactors[0] * matrix.r1.x + coFactors[7] * matrix.r1.z + coFactors[8] * matrix.r1.w;

	inversedMatrix.r2.x  = coFactors[2] * matrix.r1.x + coFactors[7] * matrix.r1.y + coFactors[10] * matrix.r1.w;
	inversedMatrix.r2.x -= coFactors[3] * matrix.r1.x + coFactors[6] * matrix.r1.y + coFactors[11] * matrix.r1.w;

	inversedMatrix.r3.x  = coFactors[5] * matrix.r1.x + coFactors[8] * matrix.r1.y + coFactors[11] * matrix.r1.w;
	inversedMatrix.r3.x -= coFactors[4] * matrix.r1.x + coFactors[9] * matrix.r1.y + coFactors[10] * matrix.r1.w;

	inversedMatrix.r0.y  = coFactors[1] * matrix.r0.y + coFactors[2] * matrix.r0.z + coFactors[5] * matrix.r0.w;
	inversedMatrix.r0.y -= coFactors[0] * matrix.r0.y + coFactors[3] * matrix.r0.z + coFactors[4] * matrix.r0.w;

	inversedMatrix.r1.y  = coFactors[0] * matrix.r0.x + coFactors[7] * matrix.r0.z + coFactors[8] * matrix.r0.w;
	inversedMatrix.r1.y -= coFactors[1] * matrix.r0.x + coFactors[6] * matrix.r0.z + coFactors[9] * matrix.r0.w;

	inversedMatrix.r2.y  = coFactors[3] * matrix.r0.x + coFactors[6] * matrix.r0.y + coFactors[11] * matrix.r0.w;
	inversedMatrix.r2.y -= coFactors[2] * matrix.r0.x + coFactors[7] * matrix.r0.y + coFactors[10] * matrix.r0.w;
	
	inversedMatrix.r3.y  = coFactors[4] * matrix.r0.x + coFactors[9] * matrix.r0.y + coFactors[10] * matrix.r0.z;
	inversedMatrix.r3.y -= coFactors[5] * matrix.r0.x + coFactors[8] * matrix.r0.y + coFactors[11] * matrix.r0.z;

	coFactors[0] = matrix.r0.z * matrix.r1.w;
	coFactors[1] = matrix.r0.w * matrix.r1.z;

	coFactors[2] = matrix.r0.y * matrix.r1.w;
	coFactors[3] = matrix.r0.w * matrix.r1.y;

	coFactors[4] = matrix.r0.y * matrix.r1.z;
	coFactors[5] = matrix.r0.z * matrix.r1.y;

	coFactors[6] = matrix.r0.x * matrix.r1.w;
	coFactors[7] = matrix.r0.w * matrix.r1.x;

	coFactors[8] = matrix.r0.x * matrix.r1.z;
	coFactors[9] = matrix.r0.z * matrix.r1.x;

	coFactors[10] = matrix.r0.x * matrix.r1.y;
	coFactors[11] = matrix.r0.y * matrix.r1.x;

	inversedMatrix.r0.z  = coFactors[0] * matrix.r3.y + coFactors[3] * matrix.r3.z + coFactors[4] * matrix.r3.w;
	inversedMatrix.r0.z -= coFactors[1] * matrix.r3.y + coFactors[2] * matrix.r3.z + coFactors[5] * matrix.r3.w;

	inversedMatrix.r1.z  = coFactors[1] * matrix.r3.x + coFactors[6] * matrix.r3.z + coFactors[9] * matrix.r3.w;
	inversedMatrix.r1.z -= coFactors[0] * matrix.r3.x + coFactors[7] * matrix.r3.z + coFactors[8] * matrix.r3.w;
	
	inversedMatrix.r2.z  = coFactors[2] * matrix.r3.x + coFactors[7] * matrix.r3.y + coFactors[10] * matrix.r3.w;
	inversedMatrix.r2.z -= coFactors[3] * matrix.r3.x + coFactors[6] * matrix.r3.y + coFactors[11] * matrix.r3.w;
	
	inversedMatrix.r2.w  = coFactors[5] * matrix.r3.x + coFactors[8] * matrix.r3.y + coFactors[11] * matrix.r3.z;
	inversedMatrix.r2.w -= coFactors[4] * matrix.r3.x + coFactors[9] * matrix.r3.y + coFactors[10] * matrix.r3.z;

	inversedMatrix.r3.z  = coFactors[5] * matrix.r3.x + coFactors[8] * matrix.r3.y + coFactors[11] * matrix.r3.z;
	inversedMatrix.r2.z -= coFactors[4] * matrix.r3.x + coFactors[9] * matrix.r3.y + coFactors[10] * matrix.r3.z;

	inversedMatrix.r0.w  = coFactors[2] * matrix.r2.z + coFactors[5] * matrix.r2.w + coFactors[1] * matrix.r2.y;
	inversedMatrix.r0.w -= coFactors[4] * matrix.r2.w + coFactors[0] * matrix.r2.y + coFactors[3] * matrix.r2.z;
	
	inversedMatrix.r1.w  = coFactors[8] * matrix.r2.w + coFactors[0] * matrix.r2.x + coFactors[7] * matrix.r2.z;
	inversedMatrix.r1.w -= coFactors[6] * matrix.r2.z + coFactors[9] * matrix.r2.w + coFactors[1] * matrix.r2.x;

	inversedMatrix.r2.w  = coFactors[6] * matrix.r2.y + coFactors[11] * matrix.r2.w + coFactors[3] * matrix.r2.x;
	inversedMatrix.r2.w -= coFactors[10] * matrix.r2.w + coFactors[2] * matrix.r2.x + coFactors[7] * matrix.r2.y;

	inversedMatrix.r3.w  = coFactors[10] * matrix.r2.z + coFactors[4] * matrix.r2.x + coFactors[9] * matrix.r2.y;
	inversedMatrix.r3.w -= coFactors[8] * matrix.r2.y + coFactors[11] * matrix.r2.z + coFactors[5] * matrix.r2.x;

	T inversedDeterminant = (T)1.0 / ( matrix.r0.x * inversedMatrix.r0.x +
								                            matrix.r0.y * inversedMatrix.r0.y +
								                            matrix.r0.z * inversedMatrix.r0.z +
								                            matrix.r0.w * inversedMatrix.r0.w );
			
	inversedMatrix *= inversedDeterminant;

	return inversedMatrix;
}

template<typename T>
matrix4x4_t<T> Ortho( T left, T right, T bottom, T top, T nearo, T faro ) {
matrix4x4_t<T> orthoMatrix = matrix4x4_t<T>( 1.0f );

const f32 xO = 2.0f / ( right - left );
const f32 yO = 2.0f / ( top - bottom );
const f32 zO = -2.0f / ( faro - nearo );

const f32 tx = -( right + left ) / ( right - left );
const f32 ty = -( top + bottom ) / ( top - bottom );
const f32 tz = -( faro + nearo ) / ( faro - nearo );

orthoMatrix.r0 = vector4_t<T>( xO,   0.0f, 0.0f, 0.0f );
orthoMatrix.r1 = vector4_t<T>( 0.0f, yO,   0.0f, 0.0f );
orthoMatrix.r2 = vector4_t<T>( 0.0f, 0.0f, zO,   0.0f );
orthoMatrix.r3 = vector4_t<T>( tx,   ty,   tz,   1.0f );

return orthoMatrix;
}

/*====================
::Perspective()
Creates and returns a 4x4 perspective matrix
====================*/
template<typename T>
matrix4x4_t<T> Perspective( const T &fovy, const T &aspect, const T &zNear, const T &zFar ) {
	const T a = (T)( 1 / tan( fovy / 2 ) );

	matrix4x4_t<T> perspectiveMatrix = matrix4x4_t<T>();
	perspectiveMatrix.r0 = vector4_t<T>( a / aspect, 0, 0, 0 );
	perspectiveMatrix.r1 = vector4_t<T>( 0, a, 0, 0 );
	perspectiveMatrix.r2 = vector4_t<T>( 0, 0, -( ( zFar + zNear ) / ( zFar - zNear ) ), -1 );
	perspectiveMatrix.r3 = vector4_t<T>( 0, 0, -( ( 2 * zFar * zNear ) / ( zFar - zNear ) ), 0 );

	return perspectiveMatrix;
}

/*====================
::LookAt()
Creates and returns a 4x4 look at matrix
====================*/
template<typename T>
matrix4x4_t<T> LookAt( const vector3_t<T> eye, const vector3_t<T> center, const vector3_t<T> up ) {
	const vector3_t<T> f = Normalize( center - eye );
	const vector3_t<T> s = Normalize( Cross( f, up ) );
	const vector3_t<T> t = Cross( s, f );

	matrix4x4_t<T> lookMatrix = matrix4x4_t<T>();

	lookMatrix.r0 = vector4_t<T>( s.x,    t.x,   f.x,    0 );
	lookMatrix.r1 = vector4_t<T>( s.y,    t.y,   f.y,    0 );
	lookMatrix.r2 = vector4_t<T>( s.z,    t.z,   f.z,    0 );
	lookMatrix.r3 = vector4_t<T>( -Dot( s, eye ), -Dot( t, eye ), -Dot( f, eye ), 1 );

	return lookMatrix;
}
