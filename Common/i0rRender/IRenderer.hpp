#pragma once

class Text;
class Skydome;
class IFrameBuffer;
class DeferredTiled;
class Mesh;
class IDevMenu;
class EnvironmentProbeRendering;
class Shadow;
class IGraphicsApiContext;

struct Itexture_t;
struct renderable_t;
struct environment_probe_t;
struct decal_t;

#include <i0rDebug/IDevMenu.hpp>
#include "IRenderWindow.hpp"

class IRenderer {
	DECLARE_MEMBER( protected, library_handle_t,           ApiLibrary )
	DECLARE_MEMBER( protected, IGraphicsApiContext*,       ApiContext )

	DECLARE_MEMBER( protected, Text*,                      TextProgram )
	DECLARE_MEMBER( protected, DeferredTiled*,             DeferredTiledProgram )
	DECLARE_MEMBER( protected, Skydome*,                   SkydomeProgram )
	DECLARE_MEMBER( protected, EnvironmentProbeRendering*, EnvironmentProbeRenderingProgram )
	DECLARE_MEMBER( protected, Shadow*,                    ShadowProgram )

	DECLARE_MEMBER( protected, IRenderWindow*,             RenderWindow )
	DECLARE_MEMBER( protected, f64,                        InterpolatedFrametime )

	DECLARE_MEMBER( protected, Itexture_t*,                Stencil )
	DECLARE_MEMBER( protected, IFrameBuffer*,              FrameBuffer )
	DECLARE_MEMBER( protected, environment_probe_t*,       EnvMap )

	DECLARE_MEMBER( protected, std::deque<renderable_t*>,  RenderGeometryQueue )
	DECLARE_MEMBER( protected, std::deque<renderable_t*>,  RenderInterfaceQueue )
	
	#ifdef FLAG_DEBUG
		DECLARE_MEMBER( protected, i32,       DrawCall )
		DECLARE_MEMBER( protected, bool,      DisplayDebugMenu )
		DECLARE_MEMBER( protected, bool,      DisplayDrawCallCount )
		DECLARE_MEMBER( protected, bool,      DisplayCamPosition )
		DECLARE_MEMBER( protected, bool,      ToggleWireframe )
		DECLARE_MEMBER( protected, bool,      DisplaySunSlices )
		DECLARE_MEMBER( protected, bool,      DisplayMeshAABB )
		DECLARE_MEMBER( protected, bool,      DisplayMemoryPages )
		DECLARE_MEMBER( protected, IDevMenu*, DebugMenu )
	#endif

	public:
		virtual void  Shutdown()                                                                       = 0;
		
		virtual bool  Initialize( instances_t* &instances )                                            = 0;
		virtual void  Draw()                                                                           = 0;
		virtual void  Swap()                                                                           = 0;

		virtual void  PushRenderableGeometry( renderable_t* renderable )                               = 0;
		virtual void  PushRenderableInterface( renderable_t* renderable )                              = 0;
		
		virtual void  OnTick()                                                                         = 0;
		
		virtual void  CacheSceneRessources()                                                           = 0;
		virtual u32   AllocateMaterialSlot( material_data_t* &pointer )                                = 0;
		virtual void  AllocateMeshData( char* &ptr, u32 &i, u32 lodCount, u32 meshFlags, Mesh* mesh )  = 0;
		virtual void  UpdateProjectionMatrix()                                                         = 0;

		virtual void MousePicking( const vec2f mousePos, vec3f &rayStart, vec3f &rayDirection )        = 0;
		
		#if defined( FLAG_WINDOWS )
			virtual void OnResize( HWND* hwnd )                                                           = 0;
		#endif

		#ifdef FLAG_DEBUG
			INLINE void SetDebugMenuState() {
				m_DisplayDebugMenu = !m_DisplayDebugMenu;
				m_DebugMenu->UpdateFocus();
			}
		#endif
};
	
