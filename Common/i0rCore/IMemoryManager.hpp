#pragma once

struct memory_page_t;
class IMemoryManager {
	DECLARE_MEMBER( protected, std::vector<memory_page_t*>, MemoryPages )
	DECLARE_MEMBER( protected, memory_page_t*,              CorePage )
	DECLARE_MEMBER( protected, memory_page_t*,              RenderPage )
	DECLARE_MEMBER( protected, memory_page_t*,              RenderApiPage )
	DECLARE_MEMBER( protected, memory_page_t*,              DebugPage )
	DECLARE_MEMBER( protected, u64,                         TotalMemoryUsage )

	public:
		virtual ~IMemoryManager() {};

		virtual bool             Initialize()                               = 0;
		virtual memory_page_t*   AllocatePage( const char* debugName, const size_t pageSize )      = 0;
		virtual bool             ReleasePage( memory_page_t* pagePointer )  = 0;
};
