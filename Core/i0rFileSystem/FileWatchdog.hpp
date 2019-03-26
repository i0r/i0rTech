#pragma once

#if defined( FLAG_DEBUG )
	#include <i0rDebug/IFileWatchdog.hpp>

	class FileWatchdog : public IFileWatchdog {
		public:
			explicit FileWatchdog() ;
			~FileWatchdog() ;
		
			bool Initialize() ;
			void Pump() ;

		private:
			void Execute() ;

			char   m_WorkingDirectory[FILENAME_MAX];
	};
#endif
