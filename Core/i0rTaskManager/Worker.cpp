#include "../Common.hpp"
#include "Worker.hpp"

#include "TaskManager.hpp"

Worker::Worker()  : m_Thread( &Worker::Work, this ),
								                    m_CurrentTask( nullptr ) {

}

Worker::~Worker() {
	if( m_Thread.joinable() ) {
		m_Thread.join();
	}

	m_CurrentTask = nullptr;
}

void Worker::Work() {
	while( 1 ) {
		{
			std::unique_lock<std::mutex> workerLock = std::unique_lock<std::mutex>( Instance.TaskManager->m_WorkersMutex );

			while( !Instance.TaskManager->m_StopSignal && Instance.TaskManager->m_TaskList.empty() ) {
				Instance.TaskManager->m_HasWorkTodo.wait( workerLock );
			}

			if( Instance.TaskManager->m_StopSignal ) return;

			m_CurrentTask = Instance.TaskManager->m_TaskList.back();
			Instance.TaskManager->m_TaskList.pop_back();
		}

		m_CurrentTask->Function();
		m_CurrentTask->IsDone = true;

		// we don't control async tasks, so it can be destroyed once the job is done
		if( m_CurrentTask->IsAsync ) {
			m_CurrentTask->~task_t();
			CORE_ALLOCATED_FREE( m_CurrentTask );
		}

		m_CurrentTask = nullptr;
	}
}