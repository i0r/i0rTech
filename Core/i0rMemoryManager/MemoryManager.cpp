#include "../Common.hpp"
#include "MemoryManager.hpp"

MemoryManager::MemoryManager() {
	m_CorePage         = nullptr;
	m_RenderPage       = nullptr;
	m_RenderApiPage    = nullptr;
	m_DebugPage        = nullptr;

	m_TotalMemoryUsage = 0;
}

MemoryManager::~MemoryManager() {
	for( i32 i = 0; i < m_MemoryPages.size(); ++i ) {
		if( !ReleasePage( m_MemoryPages[i] ) ) {
			CONSOLE_PRINT_ERROR( "MemoryManager::~MemoryManager => Failed to release page %s\n", m_MemoryPages[i]->DebugName );
		}
	}

	m_MemoryPages.clear();

	m_CorePage         = nullptr;
	m_RenderPage       = nullptr;
	m_RenderApiPage    = nullptr;

	#ifdef FLAG_DEBUG
		m_DebugPage       = nullptr;
	#endif

	m_TotalMemoryUsage = 0;
}

bool MemoryManager::Initialize() {
	m_CorePage      = AllocatePage( "Core",   524288 );  // Core module      : 512Mb
	m_RenderPage    = AllocatePage( "Render", 786432 );  // Render module    : 768Mb

	#ifdef FLAG_DEBUG
		m_DebugPage     = AllocatePage( "Debug", 262144 );  // Render module   : 256Mb
	#endif

	return true;
}

memory_page_t* MemoryManager::AllocatePage( const char* debugName, const size_t pageSize ) {
	memory_page_t* page = new memory_page_t( debugName );
	page->Size            = pageSize;
	page->BaseAddress     = malloc( page->Size );

	if( !page->BaseAddress ) {
		CONSOLE_PRINT_ERROR( "MemoryManager::~MemoryManager => Failed to allocate memory for page %s\n", page->DebugName );
		delete page;
		return nullptr;
	}

	page->Unit = new memory_unit_t();
	page->Unit->Address = page->BaseAddress;

	m_MemoryPages.push_back( page );

	return page;
}

bool MemoryManager::ReleasePage( memory_page_t* pagePointer ) {
	m_MemoryPages.erase( std::find( m_MemoryPages.begin(), m_MemoryPages.end(), pagePointer ) );
	SAFE_DELETE( pagePointer )
	return !pagePointer;
}
