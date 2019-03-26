#include <i0rCore/Common.hpp>
#include <i0rCore/Hashing/Crc32.hpp>
#include <i0rCore/IInputHandler.hpp>
#include <i0rCore/IFileSystem.hpp>
#include <i0rRender/Camera.hpp>
#include <i0rCore/IEngine.hpp>
#include <i0rRender/IRenderer.hpp>
#include <i0rRender/AABB.hpp>
#include <i0rRender/Mesh.hpp>
#include <i0rRender/Decal.hpp>
#include <i0rCore/Configuration.hpp>
#include <i0rRender/ITextureManager.hpp>
#include "GameLogic.hpp"

GameLogic::GameLogic() {
	m_ProjectName    = "SampleProject";
	m_ActiveGameMode = GAME_LOGIC_MODE_NONE;
	m_NextGameMode   = GAME_LOGIC_MODE_NONE;
	m_MouseCursor    = nullptr;
	m_MouseDecal     = nullptr;
	m_ActiveAsset    = nullptr;
}

GameLogic::~GameLogic() {
	m_ProjectName    = "";
	m_ActiveGameMode = GAME_LOGIC_MODE_NONE;
	m_NextGameMode   = GAME_LOGIC_MODE_NONE;

	renderable_ui_texture_t* cursorObj = ( renderable_ui_texture_t* )m_MouseCursor->Object;
	cursorObj->~renderable_ui_texture_t();
	RENDER_ALLOCATED_FREE( cursorObj );
	cursorObj = nullptr;

	m_MouseCursor->~renderable_t();
	RENDER_ALLOCATED_FREE( m_MouseCursor );
	m_MouseCursor = nullptr;

	decal_t* decalObj = ( decal_t* )m_MouseDecal->Object;
	decalObj->~decal_t();
	RENDER_ALLOCATED_FREE( decalObj );
	decalObj = nullptr;

	m_MouseDecal->~renderable_t();
	RENDER_ALLOCATED_FREE( m_MouseDecal );
	m_MouseDecal = nullptr;

	m_ActiveAsset = nullptr;
}

bool GameLogic::Initialize( instances_t* instances ) {
	Instance = *instances;

	Instance.InputHandler->m_Mouse.RelativeMouse = true;
	Instance.InputHandler->UpdateAction(
		HashStringCRC32( "GAME_CHANGE_MODE" ),
		{ 1, std::bind( []() {
			if( Instance.GameLogic->GetActiveGameMode() == GAME_LOGIC_MODE_EDITOR ) {
				Instance.InputHandler->m_Mouse.SetRelative( true );
				Instance.InputHandler->UpdateRelativeMouse();
				Instance.SceneManager->SetActiveCamera( Instance.SceneManager->GetCameraFirstPerson() );
				Instance.GameLogic->SetActiveGameMode( GAME_LOGIC_MODE_PLAYING );
			} else {
				Instance.InputHandler->m_Mouse.SetRelative( false );
				Instance.SceneManager->SetActiveCamera( Instance.SceneManager->GetCameraEditor() );
				Instance.GameLogic->SetActiveGameMode( GAME_LOGIC_MODE_EDITOR );
			}
		} ) } );

	m_MouseCursor = RENDER_ALLOCATE( renderable_t );
	m_MouseCursor->Type = RENDERABLE_TYPE_UI_TEXTURE;

	renderable_ui_texture_t* cursorObj = RENDER_ALLOCATE( renderable_ui_texture_t );
	cursorObj->Width = 32;
	cursorObj->Height = 32;
	cursorObj->Texture = Instance.TextureManager->GetTexture2D( "Interface/cursor" );

	m_MouseCursor->Object = cursorObj;
	m_MouseDecal = RENDER_ALLOCATE( renderable_t );
	m_MouseDecal->Type = RENDERABLE_TYPE_DECAL;

	decal_t* mouseDecal = RENDER_ALLOCATE( decal_t );
	m_MouseDecal->Object = mouseDecal;

	mouseDecal->WorldPosition = vec3f( 0.0f, 1.0f, 0.0f );
	mouseDecal->Scale = vec3f( 32.0f, 32.0f, 64.0f );
	mouseDecal->Rotation = quatf();

	mouseDecal->ModelMatrix.Identity();
	mouseDecal->ModelMatrix.Translate( mouseDecal->WorldPosition );
	mouseDecal->ModelMatrix.Rotate( mouseDecal->Rotation );
	mouseDecal->ModelMatrix.Scale( mouseDecal->Scale );

	mouseDecal->Material       = RENDER_ALLOCATE( material_t );
	mouseDecal->Material->Data = RENDER_ALLOCATE( material_data_t );

	mouseDecal->Material->Hashcode = HashStringCRC32( "Mouse_Decal" );
	mouseDecal->Material->Index = Instance.Renderer->AllocateMaterialSlot( mouseDecal->Material->Data );
	mouseDecal->Material->Layers[TEXTURE_LAYER_DIFFUSE] = Instance.TextureManager->GetTexture2D( "Editor/C_terrain_decal" );
	mouseDecal->Material->Layers[TEXTURE_LAYER_NORMAL] = Instance.TextureManager->GetTexture2D( "Editor/C_terrain_decal_normal" );

	mouseDecal->Material->Data->Alpha = 255;
	mouseDecal->Material->Data->DiffuseColor = colorrgbf( 1.0f );
	mouseDecal->Material->Data->DSSHEFactors = vec4f( 0.800000012f, 0.498039216f, 0.0941176489f, 0.000000000f );
	mouseDecal->Material->Data->Flags = MATERIAL_FLAG_IS_DECAL;
	mouseDecal->Material->Data->Shading = 17;
	mouseDecal->Material->Data->SpecularColor = colorrgbf( 1.0f );

	mouseDecal->Material->Build();
	
	#ifdef FLAG_DEBUG
	Instance.SceneManager->Create( true );
	#else
	Instance.SceneManager->Create( false );
	#endif 

	return true;
}

void GameLogic::OnTick() {
	switch( m_ActiveGameMode ) {
		case GAME_LOGIC_MODE_PLAYING:
			Instance.SceneManager->OnTick();
			Instance.InputHandler->UpdateRelativeMouse();
			break;

		case GAME_LOGIC_MODE_TEST:
			ChangeScene( "Scenes/Dev/Test_Sandbox.its", GAME_LOGIC_MODE_PLAYING );
			break;

		case GAME_LOGIC_MODE_TRANSITION:
			ChangeMode( m_NextGameMode );
			m_NextGameMode = GAME_LOGIC_MODE_NONE;
			break;

		case GAME_LOGIC_MODE_EDITOR:
			Instance.SceneManager->OnTick();
			Instance.InputHandler->UpdateMouse();
			EditorMouseUpdate();
			break;

		case GAME_LOGIC_MODE_UNKNOWN:
		case GAME_LOGIC_MODE_NONE:
		default:
			break;
	}
}

void GameLogic::ChangeMode( const game_logic_mode_t newMode ) {
	m_ActiveGameMode = newMode;
	CONSOLE_PRINT_INFO( "GameLogic::ChangeMode => New mode is %i\n", newMode );
}

void GameLogic::ChangeScene( const char* sceneName, const game_logic_mode_t nextGameMode ) {
	ChangeMode( GAME_LOGIC_MODE_LOADING );
	Instance.FileSystem->LoadStaticScene( sceneName );

	CONSOLE_PRINT_INFO( "GameLogic::ChangeScene => New scene is %s\n", sceneName );

	m_NextGameMode = nextGameMode;

	ChangeMode( GAME_LOGIC_MODE_TRANSITION );
	Instance.Renderer->CacheSceneRessources();
}

void GameLogic::EditorMouseUpdate() {
	renderable_ui_texture_t* mouseObj = ( renderable_ui_texture_t* )( m_MouseCursor->Object );
	mouseObj->X                       =  Instance.InputHandler->m_Mouse.PositionX;
	mouseObj->Y                       = -Instance.InputHandler->m_Mouse.PositionY;

	vec2f mouseGl   = vec2f( ( f32 )mouseObj->X - mouseObj->Width / 2, ( f32 )mouseObj->Y );
	vec3f nearPoint = vec3f( 0.0f ),
							rDir      = vec3f( 0.0f ),
							decalPos  = vec3f( 0.0f, 1.0f, 0.0f );

	Instance.Renderer->MousePicking( mouseGl, nearPoint, rDir );
	rDir.y = -1.0f;

	bool decalHitOnce = false;
	for( Mesh* m : Instance.SceneManager->GetMeshes() ) {
		if( m_ActiveAsset && decalHitOnce ) break;

	#ifdef FLAG_DEBUG
		if( m->m_BoundingBox.RayHitTest( nearPoint, rDir, decalPos ) ) {
			m->m_BoundingBox.DebugColor = colorrgbaf( 1.0f, 0.0f, 0.0f, 1.0f );

			if( Instance.InputHandler->m_Mouse.LeftButton && !m_ActiveAsset ) m_ActiveAsset = m;
		} else {
			m->m_BoundingBox.DebugColor = colorrgbaf( 0.0f, 1.0f, 0.0f, 1.0f );
		}
	#endif

		if( !decalHitOnce && m->m_BoundingBox.RayHitTest( nearPoint, rDir, decalPos ) ) {
			decalHitOnce = true;// decalPos.y = currentCamera->GetCenter()->y * 0.005f;
		}
	}

	if( m_ActiveAsset ) {
		vec3f selectionPos = decalPos;

		selectionPos.y = Instance.SceneManager->GetActiveCamera()->GetCenter()->y * 0.25f;
		m_ActiveAsset->m_WorldTranslation = selectionPos;
		m_ActiveAsset->m_BoundingBox.WorldPosition = m_ActiveAsset->m_WorldTranslation;
		m_ActiveAsset->UpdateModelMatrix();
	}

	m_ActiveAsset = nullptr;

	decal_t* mouseDecal = (decal_t*)m_MouseDecal->Object;

	mouseDecal->WorldPosition = decalPos;

	mouseDecal->ModelMatrix.Identity();
	mouseDecal->ModelMatrix.Translate( mouseDecal->WorldPosition );
	mouseDecal->ModelMatrix.Rotate( mouseDecal->Rotation );
	mouseDecal->ModelMatrix.Scale( mouseDecal->Scale );
}

void GameLogic::OncePerTick() {
	// Instance.Renderer->PushRenderableInterface( m_MouseCursor );

	// if( edition de terrain )
	Instance.Renderer->PushRenderableGeometry( m_MouseDecal );
}