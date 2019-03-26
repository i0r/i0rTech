#pragma once

#include <i0rRender/IRenderer.hpp>

class Renderer : public IRenderer {
	public:
		Renderer() ;

		bool   Initialize( instances_t* &instances ) ;
		void   Shutdown() ;
		void   Draw() ;

		void   Swap() ;

		void   PushRenderableGeometry( renderable_t* renderable ) ;
		void   PushRenderableInterface( renderable_t* renderable ) ;

		void   OnTick() ;
		
		void   CacheSceneRessources() ;

		u32    AllocateMaterialSlot( material_data_t* &pointer ) ;
		void   AllocateMeshData( char* &ptr, u32 &i, u32 lodCount, u32 meshFlags, Mesh* mesh ) ;

		#if defined( FLAG_WINDOWS )
			void  OnResize( HWND* hwnd ) ;
		#endif

		void  UpdateProjectionMatrix() ;
		void  MousePicking( const vec2f mousePos, vec3f &rayStart, vec3f &rayDirection ) ;

	private:
		void   RenderGeometry() ;
		void   RenderInterface() ;

		void   Resize( const u16 width, const u16 height ) ;
	
		std::mutex                m_RenderGeometryQueueMutex;
		std::mutex                m_RenderInterfaceQueueMutex;

		bool                      m_IsInitialized;
};
