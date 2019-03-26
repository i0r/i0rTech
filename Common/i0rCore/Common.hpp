#pragma once

#include "Flags.hpp"

#include <condition_variable>
#include <algorithm>
#include <atomic>
#include <mutex>
#include <unordered_map>
#include <map>
#include <vector>
#include <thread>
#include <fstream>
#include <random>
#include <string>
#include <sstream>
#include <queue>
#include <unordered_map>
#include <sys/stat.h>
#include <time.h>

#if defined( FLAG_WINDOWS )
	#include <windows.h>
	#include <Commctrl.h>
	#pragma comment( lib, "comctl32.lib" ) 
	#include <comdef.h>
	#include <direct.h>
#endif

#include "Types.hpp"
#include "Macros.hpp"
#include "Task.hpp"
#include "Instances.hpp"

#include "../i0rMaths/Common.hpp"

#include "../i0rRender/Renderables/Generic.hpp"
#include "../i0rRender/Renderables/TextString.hpp"
#include "../i0rRender/Renderables/UITexture.hpp"

#include "Helpers/Class.hpp"
#include "Helpers/File.hpp"

#include "../i0rGame/ISceneManager.hpp"

#include "Time.hpp"

#include "MemoryPage.hpp"
#include "IMemoryManager.hpp"

#include "IConsole.hpp"

#include "../i0rDebug/Assert.hpp"
#include "../i0rDebug/CrashTools.hpp"
#include "../i0rDebug/IMemoryTracker.hpp"
#include "../i0rDebug/IFileWatchdog.hpp"
