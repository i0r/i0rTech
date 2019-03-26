#pragma once

struct material_t;

struct decal_t {
	explicit decal_t()  : WorldPosition(), 
								                       Scale(),
								                       Rotation(),
								                       ModelMatrix(),
								                       Material( nullptr ) {

	}

	~decal_t() {
		WorldPosition = vec3f();
		Scale         = vec3f();
		Rotation      = quatf();
		ModelMatrix   = mat4x4f();

		Material->~material_t();
		RENDER_ALLOCATED_FREE( Material );
		Material = nullptr;
	}

	vec3f       WorldPosition;
	vec3f       Scale;
	quatf       Rotation;
	mat4x4f     ModelMatrix;
	
	material_t* Material;
};
