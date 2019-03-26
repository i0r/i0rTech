#pragma once

#undef ASSERT

#ifdef FLAG_DEBUG
	#define ASSERT( x, y ) if( !( x ) ) {\
		DumpRegistersToConsole();\
		const std::string abortStr = "Assertion '" + std::string( #x ) + "' failed! (file: " + std::string( __FILE__ )\
								                     + " at line: " + std::to_string( __LINE__ ) + ") : " + y;\
		ON_FATAL_ERROR( __FUNCTION__, abortStr.c_str() );\
	}
#elif defined FLAG_RELEASE
	#include <i0rCore/Dialogs.hpp>
	#define ASSERT( x, y ) if( !( x ) ) {\
		const std::string abortStr = "Assertion '" + std::string( #x ) + "' failed! (file: " + std::string( __FILE__ )\
								                     + " at line: " + std::to_string( __LINE__ ) + ") : " + y;\
		ON_FATAL_ERROR( __FUNCTION__, abortStr.c_str() );\
	}
#endif
