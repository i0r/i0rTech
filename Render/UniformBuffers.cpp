#include "Common.hpp"
#include <i0rRender/UniformBuffers.hpp>

ubo_common_t Common          = {};
void*        uboCommon       = 0;

ubo_lights_t Lights          = {};
void*        uboLights       = 0;

ubo_sky_t Sky                = {};
void*     uboSky             = 0;

ubo_materials_t Materials    = {};
void*           uboMaterials = 0;
		
ubo_shadows_t Shadows        = {};
void*         uboShadows     = 0;
