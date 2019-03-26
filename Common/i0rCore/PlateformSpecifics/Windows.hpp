#pragma once

#ifdef FLAG_WINDOWS
	/*===========================
	GetBaseAdress
	Retrieves a module handle for the main module
	===========================*/
	static HMODULE GetBaseAddress() {
		HMODULE hModule = NULL;
		GetModuleHandleEx( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, ( LPCTSTR )GetBaseAddress, &hModule );
		return hModule;
	}
	
	static INLINE void WindowsPathToEnginePath( std::string &x ) {
		ReplaceWord( x, "\\", "/" );
	}
#endif
