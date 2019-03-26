#pragma once

/*===========================
GetClientTimeAsMis
	Return client time as microseconds
===========================*/
static u64 GetClientTimeAsMis() {
		const HANDLE currentThread = GetCurrentThread();
		const DWORD_PTR previousMask = SetThreadAffinityMask( currentThread, 1 );

		static LARGE_INTEGER frequency = {};
		QueryPerformanceFrequency( &frequency );

		LARGE_INTEGER time = {};
		QueryPerformanceCounter( &time );

		SetThreadAffinityMask( currentThread, previousMask );

		return ( u64 )( 1000000 * time.QuadPart / frequency.QuadPart );
}

/*===========================
GetClientTimeAsMs
	Return client time as miliseconds
===========================*/
static u64 GetClientTimeAsMs() {
	return GetClientTimeAsMis() / 1000;
}

/*===========================
GetTimestamp
Returns a timestamp as string
===========================*/
static char* GetTimestamp() {
	const time_t t = time( NULL );
	tm* curTime = new tm();

	localtime_s( curTime, &t );
	char* timestamp = new char[32]();
	asctime_s( timestamp, 32, curTime );

	delete curTime;
	return timestamp;
}
