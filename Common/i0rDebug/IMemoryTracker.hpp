#pragma once

class IMemoryTracker {
	DECLARE_MEMBER( protected, u64, MemoryAllocated )
	DECLARE_MEMBER( protected, u64, AllocationCount )

	public:
		INLINE void TrackAlloc( const size_t memoryAllocated, u32* ptr ) {
			m_MemoryAllocated += memoryAllocated;
			m_AllocationCount++;
		}

		INLINE void TrackRelease( size_t size ) {
			m_MemoryAllocated -= size;
			m_AllocationCount--;
		}
};


#ifdef FLAG_DEBUG
	#define TRACK_MEMORY void* operator new( size_t size ) {\
								               void* p = malloc( size );\
								              /* if( Instance.MemoryTracker && p ) Instance.MemoryTracker->TrackAlloc( size, (u32*)p );*/\
								               return p;\
								              }\
								              void operator delete( void* p, size_t size ) {\
								              /* if( Instance.MemoryTracker ) Instance.MemoryTracker->TrackRelease( size );*/\
								               free( p );\
								              }\
								              void* operator new[]( size_t size ) {\
								               void* p = malloc( size ); \
								              /* if( Instance.MemoryTracker && p ) Instance.MemoryTracker->TrackAlloc( size, (u32*)p );*/ \
								               return p; \
								              }\
								              void operator delete[]( void* p, size_t size ) {\
								              /* if( Instance.MemoryTracker ) Instance.MemoryTracker->TrackRelease( size );*/\
								               free( p );\
								              }
#else
	#define TRACK_MEMORY 
#endif
