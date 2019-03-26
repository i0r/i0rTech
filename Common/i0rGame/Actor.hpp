#pragma once

class Camera;
struct spawn_point_t;

static constexpr f32 ACTOR_DEFAULT_SPEED = 1.0f;

enum actor_type_t {
	ACTOR_TYPE_NONE, // should be used to flag an error; use zombie as default
	ACTOR_TYPE_ZOMBIE, // actor controlled by none
	ACTOR_TYPE_HUMAN,
	ACTOR_TYPE_AI,
	ACTOR_TYPE_SP
};
	
class Actor {
	DECLARE_MEMBER( private, actor_type_t, Type )
	DECLARE_MEMBER( private, quatf,        Rotation )

	public:
		explicit Actor() ;
		~Actor() ;

		void SpawnAt( spawn_point_t* spawn ) ;
		void SpawnAt( const vec3f position, const quatf rotation ) ;

		void MoveForward() ;
		void MoveBackward() ;
		void MoveLeft() ;
		void MoveRight() ;

		vec3f Top;
		vec3f Right;
		vec3f Eye;
		vec3f Position;

	private:
		void Move( vec3f direction ) ;
};