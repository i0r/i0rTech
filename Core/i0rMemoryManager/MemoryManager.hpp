#pragma once

#include <i0rCore/IMemoryManager.hpp>

struct memory_page_t;

class MemoryManager : public IMemoryManager {
	public:
		explicit MemoryManager() ;
		~MemoryManager() ;

		bool             Initialize() ;
		memory_page_t*   AllocatePage( const char* debugName, const size_t pageSize ) ;
		bool             ReleasePage( memory_page_t* pagePointer ) ;
};
