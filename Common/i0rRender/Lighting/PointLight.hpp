#pragma once

struct point_light_t {
	explicit point_light_t( const vec3f position, const vec3f color, const f32 power )  
	: Position( position, power ), 
			Color( color ),
			QuadraticAttenuation( 0.0f ),
			LinearAttenuation( 0.0f ),
			Radius( 0.0f ) {

	}

	explicit point_light_t()  : Position(),
								                             Color(),
								                             QuadraticAttenuation( 0.0f ), 
								                             LinearAttenuation( 0.0f ),
								                             Radius( 0.0f ) {

	}

	~point_light_t() {
		Position             = vec4f();
		Color                = vec3f();
		QuadraticAttenuation = 0.0f;
		LinearAttenuation    = 0.0f;
		Radius               = 0.0f;
	}

	vec4f       Position;

	vec3f       Color;

	f32         QuadraticAttenuation;
	f32         LinearAttenuation;
	f32         Radius;
	u32         __PADDING__;
};
