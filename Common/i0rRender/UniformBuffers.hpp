#pragma once

#include "Material.hpp"
#include "Lighting/PointLight.hpp"

#define UBO_COMMON              2
#define UBO_LIGHTS              3
#define UBO_SKY                 4
#define UBO_MATERIALS           5
#define UBO_SHADOWS             6

#define UBO_LIGHTS_MAX_COUNT    256
#define UBO_MATERIALS_MAX_COUNT 256
#define SHADOW_DL_SPLIT_COUNT   4

/*=====================================================================
ubo_materials_t
Materials UBO
=====================================================================*/
struct ubo_shadow_slice_t {
	mat4x4f ModelView;
	f32     FarDistance;
};

struct ubo_shadows_t {
	ubo_shadow_slice_t SunSlices[SHADOW_DL_SPLIT_COUNT] = {};
};

extern ubo_shadows_t Shadows;
extern void*         uboShadows;

/*=====================================================================
ubo_materials_t
	Materials UBO
=====================================================================*/
struct ubo_materials_t {
	material_data_t Materials[UBO_MATERIALS_MAX_COUNT] = {};
	u32             MaterialsCount                     = 1;
};

extern ubo_materials_t Materials;
extern void*           uboMaterials;

/*=====================================================================
ubo_sky_t
	Hosek sky model UBO
=====================================================================*/
struct ubo_sky_t {
	vec3f A;
	vec3f B;
	vec3f C;
	vec3f D;
	vec3f E;
	vec3f F;
	vec3f G;
	vec3f H;
	vec3f I;
	vec3f Z;
	vec3f SunDirection;
	vec3f SunShadowDir;
};

extern ubo_sky_t Sky;
extern void*     uboSky;

/*=====================================================================
ubo_lights_t
	Point light UBO
=====================================================================*/
struct ubo_lights_t {
	point_light_t DynamicPointLights[UBO_LIGHTS_MAX_COUNT] = {};
	u32           PointLightsCount                         = 0;
};

extern ubo_lights_t Lights;
extern void*        uboLights;

/*=====================================================================
ubo_common_t
	Common UBO (M,V,P matrices; renderer infos, ...)
=====================================================================*/
struct ubo_common_t {
	mat4x4f View;
	mat4x4f InverseView;

	mat4x4f Projection;
	mat4x4f InverseProjection;
	
	mat4x4f ViewProjection;
	mat4x4f InverseViewProjection;

	vec3f   ViewPosition;
			
	vec2f   ScreenSize;

	f32     NearPlane;
	f32     FarPlane;
};

extern ubo_common_t Common;
extern void*        uboCommon;
