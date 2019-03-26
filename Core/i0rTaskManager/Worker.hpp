#pragma once

#include <i0rCore/Task.hpp>

class Worker {
	DECLARE_MEMBER( private, task_t*, CurrentTask )

	public:
		explicit Worker() ;
		~Worker() ;

	private:
		void            Work() ;

		std::thread     m_Thread;
};
