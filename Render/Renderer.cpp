#include "Common.hpp"
#include <i0rCore/Module.hpp>
#include <i0rGame/IGameLogic.hpp>
#include <i0rCore/IEngine.hpp>
#include <i0rCore/ITaskManager.hpp>
#include <i0rCore/IInputHandler.hpp>
#include <i0rCore/Library.hpp>
#include <i0rCore/IMemoryManager.hpp>
#include <i0rCore/MemoryPage.hpp>

#include <i0rRender/IGraphicsApiContext.hpp>

#include <i0rRender/Camera.hpp>
#include <i0rRender/Material.hpp>
#include <i0rRender/Mesh.hpp>
#include <i0rRender/IIndiceBuffer.hpp>
#include <i0rRender/IVertexBuffer.hpp>

#include <i0rRender/Lod.hpp>
#include <i0rRender/Decal.hpp>

#include "RenderWindow.hpp"
#include "ShadowCascade.hpp"
#include "EnvironmentProbe.hpp"

#include "i0rPrograms/DeferredTiled.hpp"
#include "i0rPrograms/Text.hpp"
#include "i0rPrograms/Composition.hpp"
#include "i0rPrograms/Skydome.hpp"
#include "i0rPrograms/EnvironmentProbeRendering.hpp"
#include "i0rPrograms/Shadow.hpp"
#include "i0rPrograms/InterfaceTexture.hpp"
#include "i0rPrograms/DeferredDecal.hpp"
#include "i0rDebugRender/AABBWireframe.hpp"

#include "i0rPrimitives/PrimitiveEditorGrid.hpp"

#include "i0rDebugRender/FramerateCounter.hpp"
#include "i0rDebugRender/DebugMenu.hpp"

#include "Renderer.hpp"

MODULE( Renderer )

Renderer::Renderer() {
	m_TextProgram                      = nullptr;
	m_DeferredTiledProgram             = nullptr;
	m_SkydomeProgram                   = nullptr;
	m_ShadowProgram                    = nullptr;
	m_EnvironmentProbeRenderingProgram = nullptr;
	m_Stencil                          = nullptr;
	m_FrameBuffer                      = nullptr;
	m_EnvMap                           = nullptr;
	m_IsInitialized                    = false;

#ifdef FLAG_DEBUG
	m_DrawCall             = 0;
	m_DisplayDebugMenu     = false;
	m_DisplayDrawCallCount = false;
	m_DisplayCamPosition   = false;
	m_ToggleWireframe      = false;
	m_DisplaySunSlices     = false;
	m_DisplayMeshAABB      = false;
	m_DisplayMemoryPages   = false;

	m_DebugMenu            = nullptr;
#endif
}

void Renderer::Shutdown() {
	SAFE_DELETE( m_TextProgram )
	SAFE_DELETE( m_DeferredTiledProgram )
	SAFE_DELETE( m_SkydomeProgram )
	SAFE_DELETE( m_ShadowProgram )
	SAFE_DELETE( m_EnvironmentProbeRenderingProgram )

	InterfaceTexture::Shader_  = nullptr;
	AABBWireframe::m_Shader    = nullptr;
	DeferredDecal::Shader_     = nullptr;
	Composition::Shader_       = nullptr;
	ShadowCascade::shaderDebug = nullptr;

	SAFE_DELETE( m_EnvMap )

	m_Stencil->~Itexture_t();
	RENDER_ALLOCATED_FREE( m_Stencil );
	m_Stencil = nullptr;

	m_FrameBuffer->~IFrameBuffer();
	RENDER_ALLOCATED_FREE( m_FrameBuffer );
	m_FrameBuffer = nullptr;

	ShadowCascade::sun->~IFrameBuffer();
	RENDER_ALLOCATED_FREE( ShadowCascade::sun );
	ShadowCascade::sun = nullptr;

	Box::VAO->~IIndiceBuffer();
	RENDER_ALLOCATED_FREE( Box::VAO );
	Box::VAO = nullptr;

	Box::VBO->~IVertexBuffer();
	RENDER_ALLOCATED_FREE( Box::VBO );
	Box::VBO = nullptr;

	EditorGrid::VAO->~IIndiceBuffer();
	RENDER_ALLOCATED_FREE( EditorGrid::VAO );
	EditorGrid::VAO = nullptr;

	EditorGrid::VBO->~IVertexBuffer();
	RENDER_ALLOCATED_FREE( EditorGrid::VBO );
	EditorGrid::VBO = nullptr;

	Quad::VAO->~IIndiceBuffer();
	RENDER_ALLOCATED_FREE( Quad::VAO );
	Quad::VAO = nullptr;

	Quad::VBO->~IVertexBuffer();
	RENDER_ALLOCATED_FREE( Quad::VBO );
	Quad::VBO = nullptr;

	ScreenQuad::VAO->~IIndiceBuffer();
	RENDER_ALLOCATED_FREE( ScreenQuad::VAO );
	ScreenQuad::VAO = nullptr;

	ScreenQuad::VBO->~IVertexBuffer();
	RENDER_ALLOCATED_FREE( ScreenQuad::VBO );
	ScreenQuad::VBO = nullptr;

	m_RenderGeometryQueue.clear(); // no delete here, since the content is most of the time owned by a scene node
	m_RenderInterfaceQueue.clear();

	#ifdef FLAG_DEBUG
		m_DrawCall             = 0;
		m_DisplayDebugMenu     = false;
		m_DisplayDrawCallCount = false;
		m_DisplayCamPosition   = false;
		m_ToggleWireframe      = false;
		m_DisplaySunSlices     = false;
		m_DisplayMeshAABB      = false;
		m_DisplayMemoryPages   = false;

		m_DebugMenu            = nullptr;
	#endif

	for( Mesh* m : Instance.SceneManager->GetMeshes() ) {
		SAFE_DELETE( m )
	}

	Instance.SceneManager->GetMeshes().clear();
	Instance.SceneManager->m_Materials.clear();

	if( m_ApiContext ) {
		m_ApiContext->FreeUBO( uboCommon );
		memset( &Common, 0, sizeof( Common ) );

		m_ApiContext->FreeUBO( uboLights );
		memset( &Lights, 0, sizeof( Lights ) );

		m_ApiContext->FreeUBO( uboSky );
		memset( &Sky, 0, sizeof( Sky ) );

		m_ApiContext->FreeUBO( uboMaterials );
		memset( &Materials, 0, sizeof( Materials ) );

		m_ApiContext->FreeUBO( uboShadows );
		memset( &Shadows, 0, sizeof( Shadows ) );

		m_ApiContext->Shutdown( m_RenderWindow );
		m_ApiContext = nullptr;
	}

	m_RenderWindow->~IRenderWindow();
	RENDER_ALLOCATED_FREE( m_RenderWindow );
	m_RenderWindow = nullptr;

	LIBRARY_FREE_LIBRARY( m_ApiLibrary );

	m_InterpolatedFrametime = 0.0;
	m_IsInitialized         = false;
}

bool Renderer::Initialize( instances_t* &instances ) {
	Instance = *instances;

	switch( instances->Configuration->Display.GraphicsApi ) {
		case GRAPHICS_API_OPENGL:
			m_ApiLibrary = LIBRARY_GET_LIBRARY_HANDLE( BuildLibraryName( "i0rTechOpenGL" ) );
			if( !m_ApiLibrary ) {
				return false;
			}

			m_ApiContext = GetModuleInstance<IGraphicsApiContext>( m_ApiLibrary, "GetOpenGLInstance" );
			break;

		default:
			ASSERT( false, "API not implemented (yet)" )
			return false;
	}

	if( !m_ApiContext ) {
		return false;
	}

	instances->GraphicsApiContext = m_ApiContext;

	m_RenderWindow = RENDER_ALLOCATE_EX( RenderWindow, ( instances->Configuration->Display.WindowWidth,
								                                              instances->Configuration->Display.WindowHeight,
								                                              instances->GameLogic->GetProjectName(), 
								                                              instances->Configuration->Display.WindowDisplayMode ) );

	SAFE_INITIALIZE( m_RenderWindow )

	instances->MainRenderWindow = m_RenderWindow;
	
	if( !m_ApiContext->Initialize( m_RenderWindow, instances ) ) {
		return false;
	}

	//====================================================================

	#ifdef FLAG_DEBUG
		m_DebugMenu          = DebugMenu::Create();
		instances->DebugMenu = m_DebugMenu;

		m_DebugMenu->RegisterBoolean( &FrameCounter::m_Display, "Display Framerate Counter", "\\Rendering" );
		m_DebugMenu->RegisterBoolean( &m_DisplayDrawCallCount,  "Display DrawCall Count",    "\\Rendering" );
		m_DebugMenu->RegisterBoolean( &m_DisplayCamPosition,    "Display Camera Position",   "\\Rendering" );
		m_DebugMenu->RegisterBoolean( &m_ToggleWireframe,       "Toggle Wireframe",          "\\Rendering" );
		m_DebugMenu->RegisterBoolean( &m_DisplaySunSlices,      "Display Sun Shadow maps",   "\\Rendering" );
		m_DebugMenu->RegisterBoolean( &m_DisplayMeshAABB,       "Display Meshes AABB",       "\\Rendering" );

		m_DebugMenu->RegisterBoolean( &m_DisplayMemoryPages, "Display Memory Pages Infos", "\\System\\Memory Paging" );
	#endif

	Instance = *instances; // pass core instances table to this module

	//====================================================================
	// create primitives buffers
	Quad::Create();
	Box::Create();
	ScreenQuad::Create();
	EditorGrid::Create();

	//====================================================================
	// allocate buffer objects on the GPU

	// COMMON ( VIEW MATRICES, SCREEN SIZE, ... )
	m_ApiContext->CreateUBO( &Common, sizeof( Common ), uboCommon );
	m_ApiContext->BindUBO( uboCommon, UBO_COMMON );
	m_ApiContext->UploadUBO( &Common, sizeof( Common ), uboCommon );

	// SKY DOME ( BASED ON HOSEK MODEL )
	m_ApiContext->CreateUBO( &Sky, sizeof( Sky ), uboSky );
	m_ApiContext->BindUBO( uboSky, UBO_SKY );
	m_ApiContext->UploadUBO( &Sky, sizeof( Sky ), uboSky );

	// LIGHTS
	m_ApiContext->CreateUBO( &Lights, sizeof( Lights ), uboLights );
	m_ApiContext->BindUBO( uboLights, UBO_LIGHTS );
	m_ApiContext->UploadUBO( &Lights, sizeof( Lights ), uboLights );

	// MATERIALS
	m_ApiContext->CreateUBO( &Materials, sizeof( Materials ), uboMaterials );
	m_ApiContext->BindUBO( uboMaterials, UBO_MATERIALS );
	m_ApiContext->UploadUBO( &Materials, sizeof( Materials ), uboMaterials );

	// SHADOWS
	m_ApiContext->CreateUBO( &Shadows, sizeof( Shadows ), uboShadows );
	m_ApiContext->BindUBO( uboShadows, UBO_SHADOWS );
	m_ApiContext->UploadUBO( &Shadows, sizeof( Shadows ), uboShadows );

	//====================================================================

	m_EnvMap = new environment_probe_t();

	ShadowCascade::Create();

	m_IsInitialized = true;

	Resize( ( u16 )Instance.Configuration->Display.WindowWidth,
								 ( u16 )Instance.Configuration->Display.WindowHeight );

	//====================================================================
	// load programs
	// :TODO: defines a list to program that should be loaded 
	// (according to user's settings)

	// SKYDOME
	m_SkydomeProgram = new Skydome();
	SAFE_INITIALIZE( m_SkydomeProgram )

	// DEFERRED TILED
	m_DeferredTiledProgram = new DeferredTiled();
	SAFE_INITIALIZE( m_DeferredTiledProgram )

	// TEXT
	m_TextProgram = new Text();
	SAFE_INITIALIZE( m_TextProgram )

	// ENVMAP
	m_EnvironmentProbeRenderingProgram = new EnvironmentProbeRendering();
	SAFE_INITIALIZE( m_EnvironmentProbeRenderingProgram )

	// SHADOWS
	m_ShadowProgram = new Shadow();
	SAFE_INITIALIZE( m_ShadowProgram )

	// UI TEXTURES
	if( !InterfaceTexture::Initialize() ) {
		return false;
	}

	// AABB Wireframe display
	if( !AABBWireframe::Initialize() ) {
		return false;
	}

	//// DEFERRED DECAL
	if( !DeferredDecal::Initialize() ) {
		return false;
	}

	// COMPOSITION
	if( !Composition::Initialize() ) {
		return false;
	}

	//====================================================================

	m_EnvMap->RearHemisphere->Bind( TEXTURE_BIND_PARABOLOID_REAR );
	m_EnvMap->FrontHemisphere->Bind( TEXTURE_BIND_PARABOLOID_FRONT );
	m_FrameBuffer->BindAttachement( 0, TEXTURE_BIND_COMPOSITION );
	EditorGrid::Texture->Bind( TEXTURE_BIND_GRID );

	//====================================================================

	return true;
}

void Renderer::Draw() {
	// sort by draw priority
	std::sort( m_RenderGeometryQueue.begin(), m_RenderGeometryQueue.end() );
	std::sort( m_RenderInterfaceQueue.begin(), m_RenderInterfaceQueue.end() );

	m_EnvironmentProbeRenderingProgram->RenderProbe( m_EnvMap ); 

	ShadowCascade::ComputeMatrices();
	ShadowCascade::ShadowPass();

	RenderGeometry();

	m_SkydomeProgram->Render();

	#ifdef FLAG_DEBUG
		m_DrawCall++;
	#endif

	Instance.GraphicsApiContext->BindBackBuffer();
	Instance.GraphicsApiContext->ClearBuffer( GENERIC_RENDER_COLOR_BUFFER_BIT | GENERIC_RENDER_DEPTH_BUFFER_BIT );
	Composition::Render( m_FrameBuffer ); // post processing

	RenderInterface();

	#ifdef FLAG_DEBUG
		if( m_DisplaySunSlices ) ShadowCascade::Debug();
	#endif
}

void Renderer::RenderGeometry() {
	#ifdef FLAG_DEBUG
		if( m_ToggleWireframe ) Instance.GraphicsApiContext->SetRasterizationMode( GENERIC_RENDER_WIRE );
	#endif

	m_DeferredTiledProgram->StartGeometryPass();

	{
		std::unique_lock<std::mutex> RenderQueueLock( m_RenderGeometryQueueMutex );

		for( renderable_t* renderable : m_RenderGeometryQueue ) {
			switch( renderable->Type ) {
				case RENDERABLE_TYPE_EDITOR_GRID:
					EditorGrid::Draw();
					break;

				case RENDERABLE_TYPE_DECAL:
					DeferredDecal::Render( renderable );
					break;

				case RENDERABLE_TYPE_MESH:
					m_DeferredTiledProgram->DrawSingle( renderable );
					break;

				case RENDERABLE_TYPE_PRIMITIVE:
				case RENDERABLE_TYPE_UNKNOWN:
				default:
					break;
			}

		#ifdef FLAG_DEBUG
			m_DrawCall++;
		#endif
		}

		m_RenderGeometryQueue.clear();
	}

	m_DeferredTiledProgram->Render();

	#ifdef FLAG_DEBUG
		// restore the fill mode so that we can see the wireframe geometry rendered to the fbo earlier
		if( m_ToggleWireframe ) Instance.GraphicsApiContext->SetRasterizationMode( GENERIC_RENDER_FILL );
	#endif
}

void Renderer::RenderInterface() {
	{
		std::unique_lock<std::mutex> RenderQueueLock( m_RenderInterfaceQueueMutex );
		for( renderable_t* renderable : m_RenderInterfaceQueue ) {
			switch( renderable->Type ) {
				case RENDERABLE_TYPE_TEXT:
					m_TextProgram->Print( renderable );
					break;

				case RENDERABLE_TYPE_UI_TEXTURE:
					InterfaceTexture::Render( renderable );
					break;

				case RENDERABLE_TYPE_WIREFRAME:
					AABBWireframe::Render( renderable );
					break;

				case RENDERABLE_TYPE_UNKNOWN:
				default:
					break;
			}

		#ifdef FLAG_DEBUG
			m_DrawCall++;
		#endif

			if( renderable->Type == RENDERABLE_TYPE_TEXT ) delete renderable;
		}

		m_RenderInterfaceQueue.clear();
	}
}

void Renderer::PushRenderableGeometry( renderable_t* renderable ) {
	{
		std::unique_lock<std::mutex> RenderQueueLock( m_RenderGeometryQueueMutex );
		m_RenderGeometryQueue.push_back( renderable );
	}
}

void Renderer::PushRenderableInterface( renderable_t* renderable ) {
	{
		std::unique_lock<std::mutex> RenderQueueLock( m_RenderInterfaceQueueMutex );
		m_RenderInterfaceQueue.push_back( renderable );
	}
}

void Renderer::OnTick() {
#ifdef FLAG_DEBUG
	FrameCounter::Update();

	const f32 frameTime = 1000.0f / f32( FrameCounter::m_LastFrameCount );

	if( FrameCounter::m_Display ) {
		m_TextProgram->Compute( 
			m_TextProgram->GetFontDebug(),
			Instance.Configuration->Display.WindowWidth - 265, 
			5,
			colorrgbaf( COLOR_RGB_FIREBRICK, 1.0f ),
			colorrgbaf( 1.0f, 1.0f, 1.0f, 1.0f ), 
			1.0f,
			"frametime: %.4f ms ( %i FPS )", frameTime, FrameCounter::m_LastFrameCount );
	}

	if( m_DisplayDrawCallCount ) {
		m_TextProgram->Compute(
			m_TextProgram->GetFontDebug(),
			Instance.Configuration->Display.WindowWidth - 265,
			20,
			colorrgbaf( COLOR_RGB_STEELBLUE, 1.0f ),
			colorrgbaf(1.0f, 1.0f, 1.0f, 1.0f),
			1.0f,
			"draw calls: %i", m_DrawCall);
	}

	m_TextProgram->Compute(
		m_TextProgram->GetFontDebug(),
		Instance.Configuration->Display.WindowWidth - 265,
		35,
		colorrgbaf( COLOR_RGB_DARKGOLDENROD, 1.0f ),
		colorrgbaf( 1.0f, 1.0f, 1.0f, 1.0f ),
		1.0f,
		"game mode: %s", Instance.GameLogic->GetActiveGameMode() == GAME_LOGIC_MODE_EDITOR ? "editor" : "game" );

	m_DrawCall = 0;

	if( m_DisplayMemoryPages ) {
		i32 pageoffset = -1;
		i64 pageTotalFree = 0, pageTotalUsed = 0;
		for( memory_page_t* page : Instance.MemoryManager->GetMemoryPages() ) {
			m_TextProgram->Compute(
				m_TextProgram->GetFontDebug(),
				Instance.Configuration->Display.WindowWidth - 265,
				55 + 25 * ++pageoffset,
				colorrgbaf( COLOR_RGB_BISQUE, 1.0f ),
				colorrgbaf( 1.0f, 1.0f, 1.0f, 1.0f ),
				1.0f,
				"page %s: %i/%iMb", page->DebugName, page->MemoryUsed / 1024, page->Size / 1024 );

			pageTotalFree += page->Size;
			pageTotalUsed += page->MemoryUsed;
			}

		m_TextProgram->Compute(
			m_TextProgram->GetFontDebug(),
			Instance.Configuration->Display.WindowWidth - 265,
			55 + 25 * ++pageoffset,
			colorrgbaf( COLOR_RGB_BISQUE, 1.0f ),
			colorrgbaf( 1.0f, 1.0f, 1.0f, 1.0f ),
			1.0f,
			"total: %i/%iMb", pageTotalUsed / 1024, pageTotalFree / 1024 );
	}

	if( m_DisplayDebugMenu ) {
		m_DebugMenu->Compute();
	}
#endif

	//================================================================================================

	Camera* currentCamera = Instance.SceneManager->GetActiveCamera();
	if( currentCamera ) {
		Common.View                  = currentCamera->GetViewMatrix();
		Common.InverseView           = Inverse( Common.View );
		Common.ViewPosition          = *currentCamera->GetCenter();
		Common.ViewProjection        = Common.Projection * Common.View;
		Common.InverseViewProjection = Inverse( Common.ViewProjection );
		currentCamera->GetFrustum()->Update( Common.ViewProjection );

		m_EnvMap->WorldPosition = *currentCamera->GetCenter();

#ifdef FLAG_DEBUG
		const vec3f camPos    = *currentCamera->GetCenter(),
								      camLookAt = *currentCamera->GetEye();

		if( m_DisplayCamPosition ) {
			m_TextProgram->Compute(
				m_TextProgram->GetFontDebug(),
				5,
				5,
				colorrgbaf(1.0f, 1.0f, 1.0f, 1.0f),
				colorrgbaf(1.0f, 1.0f, 1.0f, 1.0f),
				1.0f,
				"virtual pos: %.4f %.4f %.4f", camPos.x, camPos.y, camPos.z );

			m_TextProgram->Compute(
				m_TextProgram->GetFontDebug(),
				5,
				20,
				colorrgbaf(1.0f, 1.0f, 1.0f, 1.0f),
				colorrgbaf(1.0f, 1.0f, 1.0f, 1.0f),
				1.0f,
				"world pos: %.4f %.4f %.4f", camPos.x, camPos.y, camPos.z);

			m_TextProgram->Compute(
				m_TextProgram->GetFontDebug(),
				5,
				35,
				colorrgbaf(1.0f, 1.0f, 1.0f, 1.0f),
				colorrgbaf(1.0f, 1.0f, 1.0f, 1.0f),
				1.0f,
				"eye: %.4f %.4f %.4f", camLookAt.x, camLookAt.y, camLookAt.z);
		}
#endif

		m_ApiContext->UploadUBO( &Common, sizeof( Common ), uboCommon );
	}
}

void Renderer::MousePicking( const vec2f mousePos, vec3f &rayStart, vec3f &rayDirection ) {
	const vec4f vPort = vec4f( 0.0f, 0.0f, (f32)Instance.Configuration->Display.WindowWidth,
								                                (f32)Instance.Configuration->Display.WindowHeight );

	vec3f wCoords = vec3f( mousePos.x, mousePos.y, 0.0f );

	rayStart = UnProject( wCoords, Common.InverseViewProjection, vPort );
	rayStart.z = Common.ViewPosition.z;

	wCoords.z = 1.0f;
	vec3f farPoint = UnProject( wCoords, Common.InverseViewProjection, vPort );

	rayDirection = Normalize( farPoint - rayStart );
}

// todo: move all the allocation and assets management somewhere else?
// it does not really belongs to the renderer
u32 Renderer::AllocateMaterialSlot( material_data_t* &pointer ) {
	const u32 matIndex = Materials.MaterialsCount;
	pointer = &Materials.Materials[Materials.MaterialsCount++];
	return matIndex;
} 

void Renderer::AllocateMeshData( char* &ptr, u32 &i, u32 lodCount, u32 meshFlags, Mesh* mesh ) {
	struct file_header_its_mesh_lod_t {
		f32  LOD;
		u32  VBOSize;
		u32  VAOSize;
		u32  __PADDING__;
	} meshLodHeader = {};

	lod_t* lod = nullptr;
	for( u32 j = 0; j < lodCount; ++j ) {
		meshLodHeader = FILE_READ_RECURSIVE( file_header_its_mesh_lod_t );
		lod = new lod_t();
		lod->LOD = meshLodHeader.LOD;

#ifdef FLAG_OPENGL
		// todo: use preallocated stack instead...
		f32* vboData = new f32[meshLodHeader.VBOSize];
		memcpy( vboData, ptr, meshLodHeader.VBOSize );

		FILE_MOVE_POINTER_RECURSIVE( meshLodHeader.VBOSize );

			std::vector<u32> indices( ( u32* )ptr, ( u32* )ptr + meshLodHeader.VAOSize / sizeof( u32 ) );

		FILE_MOVE_POINTER_RECURSIVE( meshLodHeader.VAOSize );

		std::vector<u32> strides = { 3 };

		if( meshFlags & MESH_FEATURE_UV_MAP ) {
			strides.push_back( 2 );
		}

		if( meshFlags & MESH_FEATURE_NORMAL_MAPPING ) {
			// tangents + bitangents
			strides.push_back( 3 );
			strides.push_back( 3 );
		}

		// a mesh will always have its normals exported
		strides.push_back( 3 );

		lod->VAO->Bind();
		lod->VBO->FastBufferElements( GENERIC_RENDER_STATIC_DRAW, vboData, meshLodHeader.VBOSize, strides );
		lod->VAO->BufferElements( indices, GENERIC_RENDER_STATIC_DRAW );
		lod->VAO->Unbind();
#endif
		
		mesh->AddLod( lod );

		while( i % 16 != 0 ) {
			ptr++;
			i++;
		}

		delete[] vboData; 
	}

	#ifdef FLAG_DEBUG
		mesh->m_BoundingBox.DebugRender         = new renderable_t();
		mesh->m_BoundingBox.DebugRender->Type   = RENDERABLE_TYPE_WIREFRAME;
		mesh->m_BoundingBox.DebugRender->Object = &mesh->m_BoundingBox;
		mesh->m_BoundingBox.DebugColor          = colorrgbaf( 1.0f, 0.0f, 0.0f, 1.0f );
	#endif

	std::vector<lod_t*> mLods = mesh->GetLods();
	if( mLods.size() > 1 ) {
		std::sort( mLods.begin(), mLods.end(), CompareLOD );
	}
}

void Renderer::CacheSceneRessources() {
	for( point_light_t* light : Instance.SceneManager->GetPointLights() ) {
		Lights.DynamicPointLights[Lights.PointLightsCount] = *light;
		Lights.PointLightsCount++;
	}

	m_ApiContext->UploadUBO( &Lights, sizeof( Lights ), uboLights );
	m_ApiContext->UploadUBO( &Materials, sizeof( Materials ), uboMaterials );
}

void Renderer::Resize( const u16 width, const u16 height ) {
	// Update Common Values
	Instance.Configuration->Display.WindowWidth  = width;
	Instance.Configuration->Display.WindowHeight = height;

	if( !m_IsInitialized ) return;

	Instance.GraphicsApiContext->UpdateViewport( 0, 0, ( i32 )width, ( i32 )height );
	CONSOLE_PRINT_INFO( "Renderer::OnResize => %i %i\n", width, height );

	UpdateProjectionMatrix();
	Common.NearPlane  = ( f32 )Instance.Configuration->Graphics.NearPlane;
	Common.FarPlane   = ( f32 )Instance.Configuration->Graphics.FarPlane;
	Common.ScreenSize = vec2f( ( f32 )Instance.Configuration->Display.WindowWidth,
								                    ( f32 )Instance.Configuration->Display.WindowHeight );

	// Rebuild FBOs & texture attachements
	if( m_Stencil ) {
		m_Stencil->~Itexture_t();
		RENDER_ALLOCATED_FREE( m_Stencil );
		m_Stencil = nullptr;
	}

	if( m_FrameBuffer ) {
		m_FrameBuffer->~IFrameBuffer();
		RENDER_ALLOCATED_FREE( m_FrameBuffer );
		m_FrameBuffer = nullptr;
	}

	m_Stencil = Instance.GraphicsApiContext->CreateTexture();
	m_Stencil->IsManaged = false;
	m_Stencil->CreateTexture2D( { GENERIC_RENDER_DEPTH24_STENCIL8, GENERIC_RENDER_DEPTH_STENCIL,
								                       GENERIC_RENDER_R8G8B8A8, GENERIC_RENDER_CLAMP_TO_EDGE, 0, 
								                       GENERIC_RENDER_LINEAR, GENERIC_RENDER_LINEAR },
								                     Instance.Configuration->Display.WindowWidth,
								                     Instance.Configuration->Display.WindowHeight );

	m_FrameBuffer = Instance.GraphicsApiContext->CreateFrameBuffer( Instance.Configuration->Display.WindowWidth,
								                                                         Instance.Configuration->Display.WindowHeight );
	m_FrameBuffer->Bind( GENERIC_RENDER_FRAMEBUFFER_RW );
	m_FrameBuffer->AddAttachement2D( GENERIC_RENDER_RGBA32F, GENERIC_RENDER_RGBA, GENERIC_RENDER_FLOAT, 
								                          GENERIC_RENDER_CLAMP_TO_EDGE );
	m_FrameBuffer->AttachDepth( m_Stencil->Index );
	m_FrameBuffer->Build();

	if( m_DeferredTiledProgram ) {
		m_DeferredTiledProgram->OnResize();
	}
	
	m_FrameBuffer->BindAttachement( 0, TEXTURE_BIND_COMPOSITION );
		
	if( m_EnvMap ) {
			m_EnvMap->FrontHemisphere->Bind( m_EnvMap->FrontHemisphere->FixedSlot );
			m_EnvMap->RearHemisphere->Bind( m_EnvMap->RearHemisphere->FixedSlot );
	}

	Instance.GraphicsApiContext->BindBackBuffer();
}

void Renderer::UpdateProjectionMatrix() {
	Common.Projection = Perspective( ( f32 )Instance.Configuration->Graphics.Fov,
								                          ( f32 )( ( f32 )Instance.Configuration->Display.WindowWidth 
								                                 / ( f32 )Instance.Configuration->Display.WindowHeight ),
								                          ( f32 )Instance.Configuration->Graphics.NearPlane,
								                          ( f32 )Instance.Configuration->Graphics.FarPlane );

	Common.InverseProjection = Inverse( Common.Projection );
}

#ifdef FLAG_WINDOWS
	void Renderer::Swap() {
		::SwapBuffers( m_RenderWindow->GetDrawContext() );
	}

	void Renderer::OnResize( HWND* hwnd ) {
		RECT rRect = {};
		GetClientRect( *hwnd, &rRect );

		Resize( ( u16 )rRect.right, ( u16 )rRect.bottom );
	}
#endif
