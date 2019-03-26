#pragma once

enum cpu_capacity_t {
	CPU_CAPACITY_FPU,
	CPU_CAPACITY_MMX,
	CPU_CAPACITY_SSE,
	CPU_CAPACITY_SSE2,
	CPU_CAPACITY_SSE3,
	CPU_CAPACITY_SSSE3,
	CPU_CAPACITY_SSE4_1,
	CPU_CAPACITY_SSE4_2,
	CPU_CAPACITY_SSE4A,
	CPU_CAPACITY_AES,
	CPU_CAPACITY_AVX,

	CPU_CAPACITY_COUNT,
};

class Environment {
	public:
		static const char* GetCpuName() ;
		static i32 GetCpuLogicCoreCount() ;
		static u32 GetCpuCapacities() ;
		static u64 GetRamAvailable() ;
		static const char* GetOsName() ;
};