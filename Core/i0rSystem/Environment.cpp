#include "../Common.hpp"

#if defined( FLAG_WINDOWS )
	#include <intrin.h>
	#define CPU_ID( x, y ) __cpuid( x, y )
#elif defined( FLAG_LINUX )
	#include <cpuid.h>
	#define CPU_ID( x, y ) __get_cpuid( y, (u32*)&x[0], (u32*)&x[1], (u32*)&x[2], (u32*)&x[3] )
#endif

#include "Environment.hpp"

const char* Environment::GetCpuName() {
	char* cpuName = new char[48]();
	i32 cpuInfos[4] = {};
	i32 cpuLetter = -1;

	CPU_ID( cpuInfos, 0x80000000 );

	if( cpuInfos[0] >= 0x80000004 ) {
		CPU_ID( cpuInfos, 0x80000002 );
		for( const i32 &info : cpuInfos ) {
			cpuName[++cpuLetter] = (char)( info >> ( 8 * 0 ) ) & 0xFF;
			cpuName[++cpuLetter] = (char)( info >> ( 8 * 1 ) ) & 0xFF;
			cpuName[++cpuLetter] = (char)( info >> ( 8 * 2 ) ) & 0xFF;
			cpuName[++cpuLetter] = (char)( info >> ( 8 * 3 ) ) & 0xFF;
		}

		CPU_ID( cpuInfos, 0x80000003 );
		for( const i32 &info : cpuInfos ) {
			cpuName[++cpuLetter] = (char)( info >> ( 8 * 0 ) ) & 0xFF;
			cpuName[++cpuLetter] = (char)( info >> ( 8 * 1 ) ) & 0xFF;
			cpuName[++cpuLetter] = (char)( info >> ( 8 * 2 ) ) & 0xFF;
			cpuName[++cpuLetter] = (char)( info >> ( 8 * 3 ) ) & 0xFF;
		}

		CPU_ID( cpuInfos, 0x80000004 );
		for( const i32 &info : cpuInfos ) {
			cpuName[++cpuLetter] = (char)( info >> ( 8 * 0 ) ) & 0xFF;
			cpuName[++cpuLetter] = (char)( info >> ( 8 * 1 ) ) & 0xFF;
			cpuName[++cpuLetter] = (char)( info >> ( 8 * 2 ) ) & 0xFF;
			cpuName[++cpuLetter] = (char)( info >> ( 8 * 3 ) ) & 0xFF;
		}
	}

	return cpuName;
}

i32 Environment::GetCpuLogicCoreCount() {
	#ifdef FLAG_WINDOWS
		SYSTEM_INFO systemInfo = {};
		GetSystemInfo( &systemInfo );
		return systemInfo.dwNumberOfProcessors;
	#elif defined( FLAG_LINUX )
		return sysconf( _SC_NPROCESSORS_ONLN ); 
	#endif
}

u32 Environment::GetCpuCapacities() {
	u32 capacities = ~0;
	i32 cpuInfos[4] = {};
	CPU_ID( cpuInfos, 0x00000001 );

	if( cpuInfos[3] & 0x00000001 ) {
		capacities ^= 1ll << CPU_CAPACITY_FPU;
	}

	if( cpuInfos[3] & 0x00000017 ) {
		capacities ^= 1ll << CPU_CAPACITY_MMX;
	}

	if( cpuInfos[3] & 0x00000019 ) {
		capacities ^= 1ll << CPU_CAPACITY_SSE;
	}

	if( cpuInfos[3] & 0x0000001A ) {
		capacities ^= 1ll << CPU_CAPACITY_SSE2;
	}

	if( cpuInfos[2] & 0x00000001 ) {
		capacities ^= 1ll << CPU_CAPACITY_SSE3;
	}

	if( cpuInfos[2] & 0x00000009 ) {
		capacities ^= 1ll << CPU_CAPACITY_SSSE3;
	}

	if( cpuInfos[2] & 0x00000013 ) {
		capacities ^= 1ll << CPU_CAPACITY_SSE4_1;
	}

	if( cpuInfos[2] & 0x00000014 ) {
		capacities ^= 1ll << CPU_CAPACITY_SSE4_2;
	}

	if( cpuInfos[2] & 0x00000019 ) {
		capacities ^= 1ll << CPU_CAPACITY_AES;
	}

	if( cpuInfos[2] & 0x0000001C ) {
		capacities ^= 1ll << CPU_CAPACITY_AVX;
	}

	CPU_ID( cpuInfos, 0x80000001 );

	if( cpuInfos[2] & 0x00000006 ) {
		capacities ^= 1ll << CPU_CAPACITY_SSE4A;
	}

	return capacities;
}

u64 Environment::GetRamAvailable() {
	#if defined( FLAG_WINDOWS )
		MEMORYSTATUSEX memoryStatusEx = {};
		memoryStatusEx.dwLength = sizeof( memoryStatusEx );
		GlobalMemoryStatusEx( &memoryStatusEx );
		return memoryStatusEx.ullTotalPhys;
	#elif defined( FLAG_LINUX )
		u64 ramCapacity = 0;
		std::string meminfoToken = "";
		std::ifstream meminfo( "/proc/meminfo" );
		while( meminfo >> meminfoToken ) {
			if( meminfoToken == "MemTotal:" ) {
				meminfo >> ramCapacity;
				break;
			}

			meminfo.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
		}
			
		meminfo.close();
		
		return ramCapacity;
	#endif
}

#pragma warning( push )
#pragma warning( disable : 4996 ) //  C4996 - 'GetVersionExW': is deprecated

const char* Environment::GetOsName() {
	#ifdef FLAG_WINDOWS
		// NOTE: the function is not completly accurate on Windows, but this should be enough
		// for most usages
		char* name = new char[128]();

		OSVERSIONINFOEXW vi = {};
		vi.dwOSVersionInfoSize = sizeof( vi );
		GetVersionExW( (LPOSVERSIONINFOW)&vi );

		if( vi.dwMajorVersion == 10 && vi.dwMinorVersion == 0 ) {
			name = ( vi.wProductType == VER_NT_WORKSTATION ) ? "Windows 10" : "Windows Server 2016 Technical Preview";
		} else if( vi.dwMajorVersion == 6 && vi.dwMinorVersion == 3 ) {
			name = ( vi.wProductType == VER_NT_WORKSTATION ) ? "Windows 8.1" : "Windows Server 2012 R2";
		} else if( vi.dwMajorVersion == 6 && vi.dwMinorVersion == 2 ) {
			name = ( vi.wProductType == VER_NT_WORKSTATION ) ? "Windows 8" : "Windows Server 2012";
		} else if( vi.dwMajorVersion == 6 && vi.dwMinorVersion == 1 ) {
			name = ( vi.wProductType == VER_NT_WORKSTATION ) ? "Windows 7" : "Windows Server 2008 R2";
		} else if( vi.dwMajorVersion == 6 && vi.dwMinorVersion == 0 ) {
			name = ( vi.wProductType == VER_NT_WORKSTATION ) ? "Windows Vista" : "Windows Server 2008";
		} else if( vi.dwMajorVersion == 5 && vi.dwMinorVersion == 2 ) {
			if( vi.wSuiteMask & VER_SUITE_WH_SERVER ) {
				name = "Windows Home Server";
			} else if( GetSystemMetrics( SM_SERVERR2 ) != 0 ) {
				name = "Windows Server 2003 R2";
			} else if( !GetSystemMetrics( SM_SERVERR2 ) ) {
				name = "Windows Server 2003";
			}
		} else if( vi.dwMajorVersion == 5 && vi.dwMinorVersion == 1 ) {
			name = "Windows XP";
		} else if( vi.dwMajorVersion == 5 && vi.dwMinorVersion == 0 ) {
			name = "Windows 2000";
		} else {
			name = "Windows Unknown";
		}

		return name;
	#elif defined( FLAG_UNIX )
		std::string distro = "";
		std::ifstream distroInfo( "/proc/version" );
		distroInfo >> distro;
		distroInfo.close();
		return distro.c_str();
	#endif
}

#pragma warning( pop )
