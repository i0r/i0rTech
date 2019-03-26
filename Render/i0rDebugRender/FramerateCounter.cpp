#include "../Common.hpp"
#include "FramerateCounter.hpp"

u32 FrameCounter::m_FrameCount       = 0;
u32 FrameCounter::m_LastFrameCount   = 0;
u64 FrameCounter::m_LastFrameTime    = 0;
bool FrameCounter::m_Display         = true;

void FrameCounter::Update() {
	m_FrameCount++;

	if( GetClientTimeAsMs() - m_LastFrameTime >= 1000 ) {
		m_LastFrameCount = m_FrameCount;
		m_FrameCount     = 0;
		m_LastFrameTime  = GetClientTimeAsMs();
	}
}
