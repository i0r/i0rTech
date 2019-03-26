#pragma once

#include "Flags.hpp"

#ifdef FLAG_WINDOWS
	#define ALIGN( x )               __declspec( align( x ) )
	#define ALIGNED_ALLOCATE( x, y ) _aligned_malloc( x, y )
	#define ALIGNED_FREE( x )        _aligned_free( x )
#endif

#define ALIGNED_OBJECT( z, y )\
							public:\
							INLINE static void* operator new( const size_t s ) \
							{ return ALIGNED_ALLOCATE( s, y ); }\
							INLINE static void* operator new[] ( const size_t s ) \
							{ return ALIGNED_ALLOCATE( s, y ); }\
							INLINE static void operator delete ( void* x ) \
							{ if( x ) { ALIGNED_FREE( x ); } }\
							INLINE static void operator delete[] ( void* x ) \
							{ if( x ) { ALIGNED_FREE( x ); } }
