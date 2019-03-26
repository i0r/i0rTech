#include "../Common.hpp"
#include <i0rCore/IEngine.hpp>
#include "Spawn.hpp"
#include <i0rGame/IGameLogic.hpp>
#include <i0rGame/Actor.hpp>

Actor::Actor()  : Position( vec3f() ), 
								                  m_Rotation( vec3f() ), 
								                  Top( vec3f( 0.0f, 1.0f, 0.0f ) ), 
								                  Right( vec3f() ),
								                  m_Type( ACTOR_TYPE_NONE ) {

}

Actor::~Actor() {
	Position   = vec3f();
	m_Rotation = quatf();
	Top        = vec3f();
	Right      = vec3f();
	m_Type     = ACTOR_TYPE_NONE;
}

void Actor::SpawnAt( spawn_point_t* spawn ) {
	Position = spawn->Position;
	m_Rotation = quatf();
}

void Actor::SpawnAt( const vec3f position, const quatf rotation ) {
	Position = position;
	m_Rotation = rotation;
}

void Actor::Move( vec3f direction ) {
	Position += direction;
}

void Actor::MoveForward() {
	if( Instance.GameLogic->GetActiveGameMode() == GAME_LOGIC_MODE_PLAYING )
		Move( !( Eye * (f32)Instance.Engine->GetDeltaFrameTime() * ACTOR_DEFAULT_SPEED ) );
}

void Actor::MoveBackward() {
	if( Instance.GameLogic->GetActiveGameMode() == GAME_LOGIC_MODE_PLAYING )
		Move( ( Eye * (f32)Instance.Engine->GetDeltaFrameTime() * ACTOR_DEFAULT_SPEED ) );
}

void Actor::MoveLeft() {
	if( Instance.GameLogic->GetActiveGameMode() == GAME_LOGIC_MODE_PLAYING )
		Move( !( Right * (f32)Instance.Engine->GetDeltaFrameTime() * ACTOR_DEFAULT_SPEED ) );
}

void Actor::MoveRight() {
	if( Instance.GameLogic->GetActiveGameMode() == GAME_LOGIC_MODE_PLAYING )
		Move( ( Right * (f32)Instance.Engine->GetDeltaFrameTime() * ACTOR_DEFAULT_SPEED ) );
}
