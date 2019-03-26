#include "../Common.hpp"
#include "../i0rSystem/Environment.hpp"
#include "Worker.hpp"
#include "TaskManager.hpp"

TaskManager::TaskManager()  : m_WorkerCount( 0 ) {
	memset( &m_Workers, 0, sizeof( Worker* ) * TASKMANAGER_MAX_WORKER_COUNT );
	m_IsInitialized = false;
	m_StopSignal    = false;
}

TaskManager::~TaskManager() {
	m_StopSignal = true;

	if( m_IsInitialized ) {
		m_HasWorkTodo.notify_all();

		for( Worker* worker : m_Workers ) {
			if( worker ) {
				worker->~Worker();
				CORE_ALLOCATED_FREE( worker );
				worker = nullptr;
			}
		}
	}

	for( task_t* t : m_TaskList ) {
		SAFE_DELETE( t )
	}

	m_TaskList.clear();

	memset( &m_Workers, 0, sizeof( Worker* ) * TASKMANAGER_MAX_WORKER_COUNT );
	m_IsInitialized = false;
	m_StopSignal    = false;
}

bool TaskManager::Initialize() {
	// we should at least leave one core for the main loop
	m_WorkerCount = Environment::GetCpuLogicCoreCount() - 1;

	if( !m_WorkerCount ) {
		// at least one worker will be added (but this will be unoptimized)
		m_WorkerCount = 1;
	}

	for( i32 i = 0; i < m_WorkerCount; ++i ) {
		m_Workers[i] = CORE_ALLOCATE( Worker );
	}

	for( i32 i = m_WorkerCount; i < TASKMANAGER_MAX_WORKER_COUNT; ++i ) {
		m_Workers[i] = nullptr;
	}

	m_IsInitialized = true;

	return m_IsInitialized;
}

void TaskManager::UpdateTaskList( task_t* task ) {
	{
		std::unique_lock<std::mutex> workerLock( m_WorkersMutex );
		m_TaskList.push_back( task );
	}

	m_HasWorkTodo.notify_one();
}

/*====================
TaskManager::AddTask()
Adds a synchronous task to the task manager
note: it's up to the user to synchronise the task
====================*/
void TaskManager::AddTask( task_t* task ) {
	if( !m_IsInitialized ) {
		return;
	}

	UpdateTaskList( task );
}

/*====================
TaskManager::AddAsyncTask()
Adds an asynchronous task to the task manager
====================*/
void TaskManager::AddAsyncTask( function_t taskFunction, task_priority_t taskPriority ) {
	if( !m_IsInitialized ) {
		return;
	}

	task_t* task   = CORE_ALLOCATE( task_t );
	task->Function = taskFunction;
	task->Priority = taskPriority;
	task->IsAsync  = true;

	UpdateTaskList( task );
}
