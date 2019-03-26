#pragma once

#define ENVIRONMENT_PROBE_SIZE 256

struct environment_probe_t {
	explicit environment_probe_t( const vec3f &position ) ;
	explicit environment_probe_t() ;
	~environment_probe_t() ;

	vec3f       WorldPosition;
	Itexture_t* RearHemisphere;
	Itexture_t* FrontHemisphere;

	private:
		void Create() ;
};