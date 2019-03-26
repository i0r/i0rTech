#pragma once

#include <xmmintrin.h>

#include "../i0rDebug/Assert.hpp"
#include "../i0rCore/MemoryAlignement.hpp"

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

#include "Matrix3x3.hpp"
#include "Matrix4x4.hpp"

#include "Quaternion.hpp"

#define PI                  3.14159265358979323846  
#define PI_2                1.57079632679489661923  
#define PI_4                0.78539816339744830961
#define TWO_PI              PI * 2.0
#define SQRT_2              1.41421356237309504880  
#define DEGREES_TO_RADIANS  0.01745329238000000000

template<typename T>
INLINE T Lerp( const T x, const T y, const T z ) {
	return x + z * ( y - x );
}

template<typename T>
INLINE T DegreesToRadians( const T &degrees ) {
	return (T)( degrees * PI / 180.0f );
}

template<typename T>
INLINE T RadiansToDegrees( const T &radians ) {
	return radians * 180.0f / PI ;
}

template<typename T>
INLINE vector3_t<T> SphericalCoordinates( T t, T p ) {
	return vector3_t<T>( cos( p ) * sin( t ), sin( p ) * sin( t ), cos( t ) );
}

template<typename T>
T Distance2D( const vector2_t<T> p1, const vector2_t<T> p2 ) {
	const T xSqr = ( p2.x - p1.x ) * ( p2.x - p1.x );
	const T ySqr = ( p2.y - p1.y ) * ( p2.y - p1.y );

	return sqrt( xSqr + ySqr );
}

template<typename T>
T Distance3D( const vector3_t<T> p1, const vector3_t<T> p2 ) {
	const T xSqr = ( p2.x - p1.x ) * ( p2.x - p1.x );
	const T ySqr = ( p2.y - p1.y ) * ( p2.y - p1.y );
	const T zSqr = ( p2.z - p1.z ) * ( p2.z - p1.z );

	return sqrt( xSqr + ySqr + zSqr );
}

template<typename T>
INLINE void Clamp( T &var, const T min, const T max ) {
	var = (var > max) ? max : (var < min) ? min : var;
}

template<typename T>
INLINE T Mix( const T x, const T y, const T a ) {
	return x * ( 1.0 - a ) + y * a;
}

INLINE void Swap( void* x, void* y ) {
	i32* xPtr = (i32*)x;
	i32* yPtr = (i32*)y;

	if( xPtr == yPtr ) return;

	*xPtr ^= *yPtr;
	*yPtr ^= *xPtr;
	*xPtr ^= *yPtr;
}

template<typename T>
vector3_t<T> UnProject( vector3_t<T> winCoords, matrix4x4_t<T> viewProjInv, vector4_t<T> viewport ) {
	vector4_t<T> tmpVector = vector4_t<T>( winCoords, (T)1 );

	tmpVector.x = ( tmpVector.x - viewport.x ) / viewport.z * (T)2 - (T)1;
	tmpVector.y = ( tmpVector.y - viewport.y ) / viewport.w * (T)2 - (T)1;
	tmpVector.z = (T)2 * tmpVector.z - (T)1;

	const matrix4x4_t<T> vP = matrix4x4_t<T>( viewProjInv.r0.x, viewProjInv.r1.x, viewProjInv.r2.x, viewProjInv.r3.x,
								                                   viewProjInv.r0.y, viewProjInv.r1.y, viewProjInv.r2.y, viewProjInv.r3.y, 
								                                   viewProjInv.r0.z, viewProjInv.r1.z, viewProjInv.r2.z, viewProjInv.r3.z,
								                                   viewProjInv.r0.w, viewProjInv.r1.w, viewProjInv.r2.w, viewProjInv.r3.w ) ;

	vector4_t<T> retVector = vP * tmpVector;
	retVector.w = ( (T)1 / retVector.w );

	if( retVector.w  == (T)0 ) return vector3_t<T>( (T)0 );

	vector3_t<T> worldCoords = vector3_t<T>( (T)0 );
	worldCoords.x = retVector.x * retVector.w;
	worldCoords.y = retVector.y * retVector.w;
	worldCoords.z = retVector.z * retVector.w;

	return worldCoords;
}

template<typename T>
bool IsPointInBox( vector3_t<T> boxOrigin, vector3_t<T> boxDimensions, vector3_t<T> pointCoords ) {
	vector3_t<T> boxMinBounds = boxOrigin - boxDimensions / (T)2,
								      boxMaxBounds = boxOrigin + boxDimensions / (T)2;

	return ( boxMinBounds.x <= pointCoords.x && boxMaxBounds.x >= pointCoords.x 
							&& boxMinBounds.y <= pointCoords.y && boxMaxBounds.x >= pointCoords.y
							&& boxMinBounds.z <= pointCoords.z && boxMaxBounds.z >= pointCoords.z );
}
#define VALID_FLOAT_RANGE( x ) ( x != x ) ? 0.0f : x
#define SATURATE( x ) Clamp( x, 0.0f, 1.0f )
