#pragma once

#include <i0rCore/ITaskManager.hpp>

#define TASKMANAGER_MAX_WORKER_COUNT 8

class Worker;

class TaskManager : public ITaskManager {
	public:
		explicit TaskManager() ;
		~TaskManager() ;

		bool Initialize() ;

		void AddTask( task_t* task ) ;
		void AddAsyncTask( function_t taskFunction, task_priority_t taskPriority = TASK_PRIORITY_NORMAL ) ;

	private:
		void                     UpdateTaskList( task_t* task ) ;
	
		Worker*                  m_Workers[TASKMANAGER_MAX_WORKER_COUNT];
		i32                      m_WorkerCount;
};
