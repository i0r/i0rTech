#include "Common.hpp"
#include "EnvironmentProbe.hpp"

environment_probe_t::environment_probe_t( const vec3f &position )  : WorldPosition( position ) {
	Create();
}

environment_probe_t::environment_probe_t()  : WorldPosition( vec3f( 0.0f ) ) {
	Create();
}

environment_probe_t::~environment_probe_t() {
	WorldPosition = vec3f();

	RearHemisphere->~Itexture_t();
	RENDER_ALLOCATED_FREE( RearHemisphere );
	RearHemisphere = nullptr;

	FrontHemisphere->~Itexture_t();
	RENDER_ALLOCATED_FREE( FrontHemisphere );
	FrontHemisphere = nullptr;
}

void environment_probe_t::Create() {
	RearHemisphere            = Instance.GraphicsApiContext->CreateTexture();
	RearHemisphere->IsManaged = false;
	RearHemisphere->FixedSlot = TEXTURE_BIND_PARABOLOID_REAR;
	RearHemisphere->Name      = "ProbeRear";
	RearHemisphere->CreateTexture2D( { GENERIC_RENDER_RGBA16, GENERIC_RENDER_RGBA, GENERIC_RENDER_UCHAR, GENERIC_RENDER_CLAMP_TO_EDGE, 0, GENERIC_RENDER_LINEAR,
		GENERIC_LINEAR_MIPMAP_LINEAR }, ENVIRONMENT_PROBE_SIZE, ENVIRONMENT_PROBE_SIZE );

	FrontHemisphere            = Instance.GraphicsApiContext->CreateTexture();
	FrontHemisphere->IsManaged = false;
	FrontHemisphere->FixedSlot = TEXTURE_BIND_PARABOLOID_FRONT;
	FrontHemisphere->Name      = "ProbeFront";
	FrontHemisphere->CreateTexture2D( { GENERIC_RENDER_RGBA16, GENERIC_RENDER_RGBA, GENERIC_RENDER_UCHAR, GENERIC_RENDER_CLAMP_TO_EDGE, 0, GENERIC_RENDER_LINEAR,
		GENERIC_LINEAR_MIPMAP_LINEAR }, ENVIRONMENT_PROBE_SIZE, ENVIRONMENT_PROBE_SIZE );
}