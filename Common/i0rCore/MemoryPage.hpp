#pragma once

#include "IConsole.hpp"

struct memory_unit_t {
	explicit memory_unit_t()  : Previous( nullptr ), 
								                             Address( nullptr ), 
								                             Size( 0 ), 
								                             Next( nullptr ) {

	}

	~memory_unit_t() {
		Previous = nullptr;
		Address  = nullptr;
		Size     = 0;
		Next     = nullptr;
	}

	void* operator new( size_t size ) {
		return malloc( size );
	}

	void operator delete( void* p ) {
		free( p );
	}

	memory_unit_t*  Previous;
	void*           Address;
	size_t          Size;
	memory_unit_t*  Next;
};

struct memory_page_t {
	explicit memory_page_t( const char* debugChunkName = "UNKNOWN CHUNK" )  : BaseAddress( nullptr ), 
								                                                                           Unit( nullptr ),
								                                                                           FreeUnit( nullptr ), 
								                                                                           MemoryUsed( 0 ), 
								                                                                           Size( 0 ),
								                                                                           DebugName( debugChunkName ) {

	}

	~memory_page_t() {
		free( BaseAddress );
		BaseAddress     = nullptr;
		Size            = 0;
		MemoryUsed      = 0;

		memory_unit_t* curUnit = Unit, *nextUnit = nullptr;

		do {
			nextUnit = curUnit->Previous;
			delete curUnit;
			curUnit = nextUnit;
		} while( nextUnit );

		curUnit = FreeUnit;
		nextUnit = nullptr;

	/* do {
			nextUnit = curUnit->Previous;
			
			if( !curUnit->Previous ) break;

			delete curUnit;
			curUnit = nextUnit;
		} while( nextUnit );
*/

		curUnit = nullptr;
		nextUnit = nullptr;
	}

	INLINE bool operator == ( memory_page_t* c ) const  {
		return this->BaseAddress == c->BaseAddress;
	}

	void* Allocate( const size_t size ) {
		if( MemoryUsed + size > Size ) {
			CONSOLE_PRINT_WARNING( "memory_page_t::Allocate => Page %s is full; allocation will be done on the global heap\n", 
								                  DebugName );
			return malloc( size );
		}

		if( FreeUnit ) {
			if( FreeUnit->Size >= size ) {
				void* availableFreeAddress = FreeUnit->Address;

				FreeUnit = FreeUnit->Next;

				if( FreeUnit ) FreeUnit->Previous = nullptr;

				return availableFreeAddress;
			}
		}

		// DEBUG VAR Update
		MemoryUsed += size;

		// Update the unit that'll be used
		Unit->Size = size;
		Unit->Next = new memory_unit_t();

		// prepare the next unit
		Unit->Next->Previous = Unit;

		// set the next free unit as the new unit
		Unit          = Unit->Next;
		Unit->Address = ( void* )( ( size_t )Unit->Previous->Address + Unit->Previous->Size );

		return Unit->Previous->Address;
	}

	void Free( void* address ) {
		memory_unit_t* it = Unit;

		while( it ) {
			if( it->Address == address ) {
				if( !FreeUnit ) {
					FreeUnit                 = it;
					FreeUnit->Previous->Next = FreeUnit->Next;
					FreeUnit->Next->Previous = FreeUnit->Previous;
					FreeUnit->Next           = nullptr;
					FreeUnit->Previous       = nullptr;
				} else {
					memory_unit_t* curUnit = it;

					if( curUnit->Previous )
						curUnit->Previous->Next = curUnit->Next;
					if( curUnit->Next )
						curUnit->Next->Previous = curUnit->Previous;

					curUnit->Next = nullptr;
					curUnit->Previous = nullptr;

					memory_unit_t* nextFreeUnit   = FreeUnit; 
					memory_unit_t* nextFreeUnitOk = nextFreeUnit;

					while( nextFreeUnit ) {
						nextFreeUnitOk = nextFreeUnit;
						nextFreeUnit   = nextFreeUnit->Next;
					}

					nextFreeUnitOk->Next      = curUnit;
					curUnit->Previous         = nextFreeUnitOk;
				}

				MemoryUsed -= it->Size;
				return;
			}

			it = it->Previous;
		}
}
		
	void*          BaseAddress;
	memory_unit_t* Unit;
	memory_unit_t* FreeUnit;
	size_t         Size;

	// DEBUG VARS.
	size_t         MemoryUsed;
	const char*    DebugName;
};