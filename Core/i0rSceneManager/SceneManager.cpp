#include "../Common.hpp"
#include <i0rCore/IEngine.hpp>
#include <i0rCore/IInputHandler.hpp>
#include <i0rRender/Camera.hpp>
#include <i0rGame/Actor.hpp>
#include <i0rGame/IGameLogic.hpp>
#include <i0rRender/IRenderer.hpp>
#include <i0rRender/Mesh.hpp>
#include <i0rRender/Lod.hpp>
#include <i0rRender/ITexture.hpp>
#include "../i0rGame/Spawn.hpp"
#include "SceneNode.hpp"
#include "SceneManager.hpp"

SceneManager::SceneManager() {
	m_LocalActor            = nullptr;
	m_CameraFirstPerson     = nullptr;
	m_CameraEditor          = nullptr;
	m_ActiveCamera          = nullptr;
	m_IsDevEditor           = false;
	m_DisableVisibilityTest = false;
}

SceneManager::~SceneManager() {
	SAFE_DELETE( m_LocalActor )

	m_ActiveCamera = nullptr;

	m_CameraFirstPerson->~Camera();
	RENDER_ALLOCATED_FREE( m_CameraFirstPerson );
	m_CameraFirstPerson = nullptr;

	m_CameraEditor->~Camera();
	RENDER_ALLOCATED_FREE( m_CameraEditor );
	m_CameraEditor = nullptr;

	m_IsDevEditor = false;
	m_DisableVisibilityTest = false;

	Clear();

	for( spawn_point_t* s : m_Spawns ) {
		SAFE_DELETE( s )
	}

	m_Spawns.clear();

	for( point_light_t* p : m_PointLights ) {
		SAFE_DELETE( p )
	}

	m_PointLights.clear();

	memset( &Informations, 0, sizeof( scene_informations_t ) );
}

void SceneManager::Create( const bool displayEditorGrid ) {
	if( displayEditorGrid ) {
		m_IsDevEditor = true;
		CreateGridNode();
	}

	CreateLocalActor();
}

void SceneManager::Clear() {
	for( scene_node_t* node : m_Nodes ) {
		node->~scene_node_t();
		CORE_ALLOCATED_FREE( node );
	}

	m_Nodes.clear();
}

void SceneManager::OnTick() {
	if( Instance.GameLogic->GetActiveGameMode() == GAME_LOGIC_MODE_PLAYING 
		|| Instance.GameLogic->GetActiveGameMode() == GAME_LOGIC_MODE_EDITOR ) {
		m_ActiveCamera->UpdateEye( (f32)Instance.Engine->GetDeltaFrameTime(),
								                          Instance.InputHandler->GetMouse().PositionX,
								                          Instance.InputHandler->GetMouse().PositionY );
		m_ActiveCamera->Update();
	}

	if( Instance.GameLogic->GetActiveGameMode() == GAME_LOGIC_MODE_EDITOR ) {
		vec3f* camPos = m_ActiveCamera->GetCenter();
		camPos->y -= Instance.InputHandler->GetMouse().WheelY * (f32)Instance.Engine->GetDeltaFrameTime() * 10.0f; 
		Clamp( camPos->y, 50.0f, 500.0f );

		static bool isHoldingRight = false;
		static f32 defaultFov      = Instance.Configuration->Graphics.Fov;

		if( Instance.InputHandler->GetMouse().RightButton ) {
			Instance.Configuration->Graphics.Fov -= (f32)Instance.Engine->GetDeltaFrameTime() * 0.001f;

			if( Instance.Configuration->Graphics.Fov < defaultFov - 0.50f )
				Instance.Configuration->Graphics.Fov = defaultFov - 0.50f;

			Instance.Renderer->UpdateProjectionMatrix();
			isHoldingRight = true;
		} else if( isHoldingRight ) {
			Instance.Configuration->Graphics.Fov = defaultFov;
			Instance.Renderer->UpdateProjectionMatrix();
			isHoldingRight = false;
		}
	}
}

void SceneManager::AddNode( Mesh* mesh ) {
	renderable_t* renderable = RENDER_ALLOCATE( renderable_t );
	renderable->Type   = RENDERABLE_TYPE_MESH;
	renderable->Object = mesh;

	scene_node_t* meshNode = CORE_ALLOCATE_EX( scene_node_t, ( renderable ) );
	meshNode->Hashcode = mesh->m_Hashcode;

	m_Meshes.push_back(mesh);
	m_Nodes.push_back(meshNode);
}

void SceneManager::ComputeVisibility() {
	for( scene_node_t* node : m_Nodes ) {
		if( node->Flags.IsAlwaysVisible || m_DisableVisibilityTest ) {
			Instance.Renderer->PushRenderableGeometry( node->Content );
		} else {
			if( node->Content->Type == RENDERABLE_TYPE_MESH ) {
				Mesh* meshData = (Mesh*)( node->Content->Object );

				if( m_ActiveCamera->GetFrustum()->IsBoxInFrustum( &meshData->m_BoundingBox ) ) {
					Instance.Renderer->PushRenderableGeometry( node->Content );
					
					#ifdef FLAG_DEBUG
						if( Instance.Renderer->GetDisplayMeshAABB() ) Instance.Renderer->PushRenderableInterface( meshData->m_BoundingBox.DebugRender );
					#endif
				}
			}
		}
	}
}

void SceneManager::CreateLocalActor() {
	m_LocalActor = new Actor();
	m_LocalActor->SetType( ACTOR_TYPE_HUMAN );
	m_LocalActor->SpawnAt( vec3f( 0.0f, 0.50f, 0.0f ), quatf() );

	m_CameraFirstPerson = RENDER_ALLOCATE( FirstPersonCamera );
	m_CameraFirstPerson->FollowActor( m_LocalActor );

	Instance.InputHandler->UpdateAction(
		HashStringCRC32("ACTOR_ACTION_MOVE_LEFT"),
		{ 0, std::bind(&Actor::MoveLeft, m_LocalActor) } );

	Instance.InputHandler->UpdateAction(
		HashStringCRC32("ACTOR_ACTION_MOVE_RIGHT"),
		{ 0, std::bind(&Actor::MoveRight, m_LocalActor) } );

	Instance.InputHandler->UpdateAction(
		HashStringCRC32("ACTOR_ACTION_MOVE_FORWARD"),
		{ 0, std::bind(&Actor::MoveForward, m_LocalActor) } );

	Instance.InputHandler->UpdateAction(
		HashStringCRC32("ACTOR_ACTION_MOVE_BACKWARD"),
		{ 0, std::bind(&Actor::MoveBackward, m_LocalActor) } );

	m_CameraEditor = RENDER_ALLOCATE_EX( EditorCamera, ( vec3f( 0.0f, 50.0f, 0.0f ), 45.0f ) );

	m_ActiveCamera = m_CameraFirstPerson;
}

void SceneManager::CreateGridNode() {
	renderable_t* renderable = RENDER_ALLOCATE( renderable_t );
	renderable->Type   = RENDERABLE_TYPE_EDITOR_GRID;
	renderable->Object = nullptr;

	scene_node_t* gridNode = CORE_ALLOCATE_EX( scene_node_t, ( renderable ) );
	gridNode->Hashcode = HashStringCRC32( "EDITOR GRID" );
	gridNode->Flags.IsAlwaysVisible = true;

	m_Nodes.push_back( gridNode );
}
