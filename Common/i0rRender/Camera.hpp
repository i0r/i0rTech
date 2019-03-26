#pragma once

#include <i0rGame/Actor.hpp>
#include <i0rCore/Configuration.hpp>
#include "Frustum.hpp"

enum camera_type_t {
	CAMERA_TYPE_GENERIC,
	CAMERA_TYPE_FIRST_PERSON,
	CAMERA_TYPE_EDITOR,
	CAMERA_TYPE_COUNT,
};

/*====================================
Camera
	Generic Camera class
====================================*/
class Camera {
	DECLARE_MEMBER( protected, vec3f*,        Top )
	DECLARE_MEMBER( protected, vec3f*,        Eye )
	DECLARE_MEMBER( protected, vec3f*,        Right )
	DECLARE_MEMBER( protected, vec3f*,        Center )
	DECLARE_MEMBER( protected, frustum_t*,    Frustum )
	DECLARE_MEMBER( protected, mat4x4f,       ViewMatrix )
	DECLARE_MEMBER( protected, f32,           Yaw )
	DECLARE_MEMBER( protected, f32,           Pitch )
	DECLARE_MEMBER( protected, Actor*,        FocusedActor )
	DECLARE_MEMBER( protected, camera_type_t, Type )
	
	public:
		explicit Camera( vec3f vecCenter, vec3f vecLookAt, vec3f vecTop )  : m_Top( &vecTop ), 
								                                                                       m_Eye( &vecLookAt ),
								                                                                       m_Right( nullptr ),
								                                                                       m_Center( &vecCenter ),
								                                                                       m_Frustum( nullptr ), 
								                                                                       m_ViewMatrix( 0.0f ), 
								                                                                       m_Yaw( (f32)PI ), 
								                                                                       m_Pitch( 0.0f ),
								                                                                       m_FocusedActor( nullptr ), 
								                                                                       m_Type( CAMERA_TYPE_GENERIC ) {
			m_Frustum = RENDER_ALLOCATE( frustum_t );
			this->Update();
		}

		virtual ~Camera() {
			m_Top    = nullptr;
			m_Eye    = nullptr;
			m_Right  = nullptr;
			m_Center = nullptr;

			m_Frustum->~frustum_t();
			RENDER_ALLOCATED_FREE( m_Frustum );
			m_Frustum = nullptr;

			m_ViewMatrix.Clear();

			m_Yaw   = 0.0f;
			m_Pitch = 0.0f;

			m_FocusedActor = nullptr;
			
			m_Type = CAMERA_TYPE_GENERIC;
		}

		/*====================================
		UpdateEye
			Update camera orientation (eye component)
		====================================*/
		virtual void UpdateEye( const f32 deltaTime, const i32 mouseX, const i32 mouseY )  = 0;

		/*====================================
		Update
			Frame update for a camera object
		====================================*/
		INLINE void Update() {
			m_ViewMatrix = LookAt<f32>( *m_Center, ( *m_Center + *m_Eye ), *m_Top );
		}

		/*====================================
		FollowActor
			Focuses the camera on a specific actor
		====================================*/
		INLINE void FollowActor( Actor* actor ) {
			m_FocusedActor = actor;
		
			m_Eye          = &actor->Eye;
			m_Center       = &actor->Position;
			m_Top          = &actor->Top;
			m_Right        = &actor->Right;
		}
};

class FirstPersonCamera : public Camera {
	public:
		explicit FirstPersonCamera( vec3f vecCenter, vec3f vecLookAt, vec3f vecTop )  : Camera( vecCenter, vecLookAt, vecTop ) {
			m_Type = CAMERA_TYPE_FIRST_PERSON;
		}

		explicit FirstPersonCamera()  : Camera( vec3f(), vec3f(), vec3f() ) {
			m_Type = CAMERA_TYPE_FIRST_PERSON;
		}

		~FirstPersonCamera() {

		}

		void UpdateEye( const f32 deltaTime, const i32 mouseX, const i32 mouseY ) {
			f32 newYaw   = m_Yaw   + ( ( f32 )Instance.Configuration->Input.MouseSensitivity * deltaTime * mouseX );
			f32 newPitch = m_Pitch + ( ( f32 )Instance.Configuration->Input.MouseSensitivity * deltaTime * mouseY );

			m_Yaw   += ( newYaw   - m_Yaw )   * ( f32 )Instance.Configuration->Input.MouseAcc;
			m_Pitch += ( newPitch - m_Pitch ) * ( f32 )Instance.Configuration->Input.MouseAcc;

			if( m_Yaw < -PI ) {
				m_Yaw += (f32)TWO_PI;
			} else if( m_Yaw > PI ) {
				m_Yaw -= (f32)TWO_PI;
			}

			if( m_Pitch < -PI_2 ) {
				m_Pitch = (f32)-PI_2;
			} else if( m_Pitch > PI_2 ) {
				m_Pitch = (f32)PI_2;
			}

			m_Eye->x = cos( m_Pitch ) * sin( m_Yaw );
			m_Eye->y = sin( m_Pitch );
			m_Eye->z = cos( m_Pitch ) * cos( m_Yaw );
			m_Eye->Normalize();

			const f32 yawResult = m_Yaw - (f32)PI_2;

			m_Right->x = sin( yawResult );
			m_Right->y = 0.0f;
			m_Right->z = cos( yawResult );

			m_Right->Normalize();

			*m_Top = Cross( *m_Right, *m_Eye );
			m_Top->Normalize();
		}
};

class EditorCamera : public Camera {
	public:
		explicit EditorCamera( vec3f vecCenter, f32 lookAtAngle )  
			: Camera( vecCenter, vec3f( 0.0f, lookAtAngle / 90.0f, 0.0f ), vec3f( 0.0f, 1.0f, 0.0f ) ) {
			m_Type   = CAMERA_TYPE_EDITOR;
			m_Top    = new vec3f( -0.30f, 0.60f, 0.70f );
			m_Eye    = new vec3f( 0.25f, 0.77f, -0.58f );
			m_Right  = new vec3f( 0.90f, 0.0f, 0.40f );
			m_Center = new vec3f( 98.00f, 140.00f, -130.00f );
		}

		explicit EditorCamera( vec3f vecCenter, vec3f vecLookAt )  
			: Camera( vecCenter, vecLookAt, vec3f( 0.0f, 1.0f, 0.0f ) ) {
			m_Type = CAMERA_TYPE_EDITOR;
		}

		explicit EditorCamera()  
			: Camera( vec3f( 0.0f ), vec3f( 0.0f, -1.0f, 0.0f ), vec3f( 0.0f, 1.0f, 0.0f ) ) {
			m_Type = CAMERA_TYPE_EDITOR;
		}

		~EditorCamera() {
			SAFE_DELETE( m_Top )
			SAFE_DELETE( m_Eye )
			SAFE_DELETE( m_Right )
			SAFE_DELETE( m_Center )
		}

		void UpdateEye( const f32 deltaTime, const i32 mouseX, const i32 mouseY ) {
			const i32 editX = mouseX - Instance.Configuration->Display.WindowWidth  / 2;
			const i32 editY = -mouseY - Instance.Configuration->Display.WindowHeight / 2;
		
			const i32 safeX = Instance.Configuration->Display.WindowWidth  / 3,
								     safeY = Instance.Configuration->Display.WindowHeight / 3;

			// check if the mouse cursor is outside the screen 'safe zone' (roughly 1/3² of the screen)
			if( editX < -safeX || editX > safeX || editY < -safeY || editY > safeY ) {
				m_Center->x += (f32)Instance.Configuration->Input.MouseSensitivity * deltaTime * editX * m_Eye->x * 0.01f;
				m_Center->z += (f32)Instance.Configuration->Input.MouseSensitivity * deltaTime * editY * m_Eye->y * 0.01f;
			}
		}
};
