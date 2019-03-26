#pragma once

#include <i0rGame/IGameLogic.hpp>

class GameLogic : public IGameLogic {
	DECLARE_MEMBER( private, renderable_t*, MouseCursor )
	DECLARE_MEMBER( private, renderable_t*, MouseDecal )
	DECLARE_MEMBER( private, Mesh*,         ActiveAsset ) // selected asset

	public:
		explicit GameLogic() ;
		~GameLogic() ;

		bool Initialize( instances_t* instances ) ;
		void OnTick() ;
		void OncePerTick() ;

		void ChangeMode( const game_logic_mode_t newMode ) ;
		void ChangeScene( const char* sceneName, const game_logic_mode_t nextGameMode ) ;

	private:
		void EditorMouseUpdate() ;
};
