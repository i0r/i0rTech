#pragma once

class FrameCounter {
	public:
		static void Update() ;
		static u32  m_FrameCount;
		static u32  m_LastFrameCount;
		static u64  m_LastFrameTime;
		static bool m_Display;
};
