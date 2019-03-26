#pragma once

/*====================================
stack_t
	Generic stack
====================================*/
struct stack_t {
	void*  BaseAddress;
	void*  CurrentAddress;
	void*  PreviousAddress;
	size_t Size;

	explicit stack_t( const size_t stackSize )  : BaseAddress( nullptr ), 
								                                               CurrentAddress( nullptr ), 
								                                               PreviousAddress( nullptr ), 
								                                               Size( stackSize ) {
		ASSERT( stackSize > 0, "Invalid stack size (size must be a positive integer)" )
	}
			
	~stack_t() {
		free( BaseAddress );
		CONSOLE_PRINT_INFO( "stack_t::~stack_t => Released block at 0x%X\n", BaseAddress );
		
		BaseAddress     = nullptr;
		CurrentAddress  = nullptr;
		PreviousAddress = nullptr;  
		Size            = 0;
	}

	/*====================================
	Create
		Create a stack
	====================================*/
	bool Create() {
		BaseAddress = malloc( Size );

		if( !BaseAddress ) {
			CONSOLE_PRINT_ERROR( "stack_t::Create => Failed to allocate block (size: %i) : BaseAddress is nullptr\n", Size );
			return false;
		}

		CONSOLE_PRINT_INFO( "stack_t::Create => Allocated block at 0x%X (size: %i)\n", BaseAddress, Size );

		CurrentAddress  = BaseAddress;
		PreviousAddress = BaseAddress;
		return true;
	}

	/*====================================
	Allocate
		Allocate a chunk from top of the stack
			const size_t size: size to allocate on the stack
	====================================*/
	void* Allocate( const size_t size ) {
		if( size > Size || ( size_t )BaseAddress + Size - ( size_t )CurrentAddress < size ) {
			return nullptr;
		}

		PreviousAddress = CurrentAddress;
		CurrentAddress  = ( void* )( ( size_t )CurrentAddress + size );
		
		return PreviousAddress;
	}

	/*====================================
	Clear
		Clear the stack
	====================================*/
	void Clear() {
		CurrentAddress  = BaseAddress;
		PreviousAddress = BaseAddress;
		
		memset( BaseAddress, 0, Size );
	}

	/*====================================
	Pop
		Pop the last chunk pushed to the stack
			void* previousAllocation: pointer to the last element pushed
	====================================*/
	void Pop( void* previousAllocation ) {
		const size_t sizePopd = ( size_t )CurrentAddress - ( size_t )previousAllocation;
		memset( previousAllocation, 0, sizePopd );

		CurrentAddress  = previousAllocation;
		PreviousAddress = ( !previousAllocation ) ? CurrentAddress : previousAllocation;
	}
};
