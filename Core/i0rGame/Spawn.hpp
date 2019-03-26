#pragma once

//enum actor_type_t;

struct spawn_point_t {
	~spawn_point_t() {
		Position = vec3f();
		Rotation = vec3f();
	}

	vec3f        Position;
	vec3f        Rotation;
/*actor_type_t Type;
	bool         Assigned;*/
};
