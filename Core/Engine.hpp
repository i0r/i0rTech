#pragma once

#include <i0rCore/IEngine.hpp>

class Engine : public IEngine {
	public:
		Engine() ;

		void Shutdown() ;
		bool Initialize( IGameLogic* gameLogic ) ;
		void Run() ;
};
