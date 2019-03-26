#pragma once

class ITaskManager {
	DECLARE_MEMBER( protected, bool,                 IsInitialized )
	DECLARE_MEMBER( protected, std::vector<task_t*>, TaskList )

	public:
		virtual ~ITaskManager() {}
		virtual bool Initialize()                                                         = 0;

		virtual void AddTask( task_t* task )                                              = 0;
		virtual void AddAsyncTask( function_t taskFunction, 
								                     task_priority_t taskPriority = TASK_PRIORITY_NORMAL )  = 0;

	protected:
		friend class Worker;

		std::atomic<bool>        m_StopSignal;
		std::condition_variable  m_HasWorkTodo;
		std::mutex               m_WorkersMutex;
};
