#pragma once

/*====================================
pool_t
A pool of object
====================================*/
template<typename T> struct pool_t {
	T*     Pool;
	size_t Size;

	union pool_element_t {
		T* Next;
		T  Current;
	};

	struct pool_data_t {
		T*             Free;
		pool_element_t Items[1];
	};

	explicit pool_t<T>( const size_t &poolSize )  : m_Pool( nullptr ),
								                                                 m_Size( poolSize ) {
		ASSERT( poolSize > 0, "Invalid pool size (size must be a positive integer)" )
	}
			
	~pool_t() {
		SAFE_ARRAY_DELETE( Pool );
		Size = 0;
	}
			
	/*====================================
	Create
		Create the pool
	====================================*/
	bool Create() {
		Pool = ( T* )( new char[sizeof( pool_element_t ) * Size + sizeof( T* )] );

		if( !Pool ) {
			CONSOLE_PRINT_INFO( "pool_t::Create => Failed to allocated pool (pool element count: %i; obj size: %i)\n",
								               Size, sizeof( T ) );
			return false;
		}

		pool_data_t* data = ( pool_data_t* )Pool;

		data->Free = &data->Items[0].Current;

		for( i32 i = 0; i < Size - 1; i++ ) {
			data->Items[i].Next = &data->Items[i + 1].Current;
		}

		data->Items[Size - 1].Next = nullptr;

		return true;
	}

	/*====================================
	GetHead
		Returns the head of the linked list
	====================================*/
	INLINE pool_element_t GetHead() const  {
		return ( pool_data_t* )( Pool )->Items[0];
	}

	/*====================================
	GetSlot
		Get a slot from the pool
	====================================*/
	T* GetSlot() {
		pool_data_t* data = ( pool_data_t* )Pool;

		const T* newObj = data->Free;
		if( newObj ) {
			data->Free = ( ( pool_element_t* )newObj )->Next;
		}

		return newObj;
	}

	/*====================================
	FreeSlot
		Free a specific slot
			T* slotPointer: pointer to the slot of the pool
	====================================*/
	void FreeSlot( const T* slotPointer ) {
		pool_data_t* data = ( pool_data_t* )Pool;

		( ( pool_element_t* )slotPointer )->Next = data->Free;
		data->Free = slotPointer;
	}
};
