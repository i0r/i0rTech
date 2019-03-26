#pragma once

// 'free' slots: 0; 14; 12; 13
#define TEXTURE_BIND_COMPOSITION          GENERIC_RENDER_TEXTURE_SLOT_1
#define TEXTURE_BIND_ACTIVE_FONT          GENERIC_RENDER_TEXTURE_SLOT_2  // :todo: cnst bind for debug font?
#define TEXTURE_BIND_GRID                 GENERIC_RENDER_TEXTURE_SLOT_3
#define TEXTURE_BIND_DEFAULT_TEXTURE      GENERIC_RENDER_TEXTURE_SLOT_4
#define TEXTURE_BIND_PARABOLOID_FRONT     GENERIC_RENDER_TEXTURE_SLOT_5
#define TEXTURE_BIND_PARABOLOID_REAR      GENERIC_RENDER_TEXTURE_SLOT_6
#define TEXTURE_BIND_SUN_CSM              GENERIC_RENDER_TEXTURE_SLOT_7

// 8 -> 14: active material layers (albedo, normals, emissive, roughness, ...)
#define TEXTURE_BIND_ACTIVE_MATERIAL_BASE       GENERIC_RENDER_TEXTURE_SLOT_8 

#define TEXTURE_BIND_ACTIVE_MATERIAL_ALBEDO     GENERIC_RENDER_TEXTURE_SLOT_8
#define TEXTURE_BIND_ACTIVE_MATERIAL_METALNESS  GENERIC_RENDER_TEXTURE_SLOT_9
#define TEXTURE_BIND_ACTIVE_MATERIAL_NORMAL     GENERIC_RENDER_TEXTURE_SLOT_10
#define TEXTURE_BIND_ACTIVE_MATERIAL_ROUGHNESS  GENERIC_RENDER_TEXTURE_SLOT_11
#define TEXTURE_BIND_ACTIVE_MATERIAL_AMBIENT    GENERIC_RENDER_TEXTURE_SLOT_13

#define TEXTURE_BIND_ACTIVE_INTERFACE_TEXTURE   GENERIC_RENDER_TEXTURE_SLOT_15
