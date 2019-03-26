#pragma once

static constexpr f64 ENGINE_UPDATE_DELTA = 1.0 / 30.0; // update per seconds (gamelogic, input handling, ...)
static constexpr f64 ENGINE_SOD          = 0.25; // 'spiral of death' : preventing update overhead

struct memory_page_t;

class IEngine {
	DECLARE_MEMBER( protected, f64,              DeltaFrameTime )
	DECLARE_MEMBER( protected, library_handle_t, RenderLibrary )

	public:
		virtual void Shutdown()                           = 0;
		virtual bool Initialize( IGameLogic* gameLogic )  = 0;
		virtual void Run()                                = 0;

		INLINE void SendShutdownSignal() { m_IsRunning = false; }
		INLINE bool IsRunning() const     { return m_IsRunning;  }

	protected:
		std::atomic_bool  m_IsRunning;
};
