#pragma once

#define MEMBER_GET( x, y )        INLINE x Get##y() const  { return m_##y; }
#define MEMBER_SET( x, y )        INLINE void Set##y( x newVal ) { m_##y = newVal; }
#define MEMBER_GET_SET( x, y )    MEMBER_GET( x, y ) MEMBER_SET( x, y )
#define DECLARE_MEMBER( z, x, y ) z: x m_##y; public: MEMBER_GET_SET( x, y )
