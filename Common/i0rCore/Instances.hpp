#pragma once

#include "Flags.hpp" // must be included, otherwise the header won't use the correct flag set

class IEngine;
class ITaskManager;
class IGameLogic;
class IFileSystem;
class IInputHandler;
class IRenderer;
class IRenderWindow;
class ITextureManager;
class IShaderManager;
class ISceneManager;
class IGraphicsApiContext;
class IMemoryManager;

#ifdef FLAG_DEBUG
	class IConsole;
	class IDevMenu;
	class IFileWatchdog;
#endif

struct configuration_t;

struct instances_t {
	IGameLogic*          GameLogic;
	IEngine*             Engine;
	ITaskManager*        TaskManager;
	IFileSystem*         FileSystem;
	IInputHandler*       InputHandler;
	IRenderer*           Renderer;
	IRenderWindow*       MainRenderWindow;
	configuration_t*     Configuration;
	ITextureManager*     TextureManager;
	IShaderManager*      ShaderManager;
	ISceneManager*       SceneManager;
	IGraphicsApiContext* GraphicsApiContext;
	IMemoryManager*      MemoryManager;

	#ifdef FLAG_DEBUG
		IConsole*           Console;
		IDevMenu*           DebugMenu;
		IFileWatchdog*      FileWatchdog;
	#endif
};

extern instances_t Instance;
