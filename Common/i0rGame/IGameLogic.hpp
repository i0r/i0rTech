#pragma once

enum game_logic_mode_t {
	GAME_LOGIC_MODE_NONE,

	GAME_LOGIC_MODE_TEST,

	GAME_LOGIC_MODE_PLAYING, // can be used for debug/sandbox too
	GAME_LOGIC_MODE_LOADING,
	GAME_LOGIC_MODE_EDITOR,
	GAME_LOGIC_MODE_MENU,
	GAME_LOGIC_MODE_TRANSITION,
	GAME_LOGIC_MODE_DEBUG_FRAME_PAUSE,

	GAME_LOGIC_MODE_UNKNOWN,
};

class IGameLogic {
	DECLARE_MEMBER( protected, const char*,       ProjectName )
	DECLARE_MEMBER( protected, game_logic_mode_t, ActiveGameMode )
	DECLARE_MEMBER( protected, game_logic_mode_t, NextGameMode )

	public:
		virtual ~IGameLogic() {}
		virtual bool Initialize( instances_t* instances )                                        = 0;
		virtual void OnTick()                                                                    = 0;
		virtual void OncePerTick()  = 0;
		virtual void ChangeMode( const game_logic_mode_t newMode )                               = 0;
		virtual void ChangeScene( const char* sceneName, const game_logic_mode_t nextGameMode )  = 0;
};
