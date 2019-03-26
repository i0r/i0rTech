#pragma once

enum task_priority_t {
	TASK_PRIORITY_DONT_CARE,
	TASK_PRIORITY_LOW,
	TASK_PRIORITY_NORMAL,
	TASK_PRIORITY_HIGH,
};


struct task_t {
	explicit task_t()  : IsDone( false ), 
								                      IsAsync( false ), 
								                      InProgress( false ), 
								                      Priority( TASK_PRIORITY_DONT_CARE ), 
								                      Function( nullptr ) {

	};

	explicit task_t( const task_priority_t priority )  : IsDone( false ), 
								                                                      IsAsync( false ), 
								                                                      InProgress( false ),
								                                                      Priority( priority ), 
								                                                      Function( nullptr ) {

	};

	~task_t() {
		IsDone     = false;
		IsAsync    = false;
		InProgress = false;
		Priority   = TASK_PRIORITY_DONT_CARE;
		Function   = nullptr;
	}

	bool            IsDone;
	bool            IsAsync;
	bool            InProgress;
	task_priority_t Priority;
	function_t      Function;
};


static INLINE bool TaskPriorityCompare( task_t* task1, task_t* task2 ) {
	return task1->Priority < task2->Priority;
}
