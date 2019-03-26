#pragma once

#include "../i0rRender/IRenderWindow.hpp"

struct configuration_shadows_t {
	u32 Resolution;
};

enum configuration_quality_t {
	CONFIGURATION_QUALITY_LOW,
	CONFIGURATION_QUALITY_MEDIUM,
	CONFIGURATION_QUALITY_HIGH,
	CONFIGURATION_QUALITY_ULTRA,
	CONFIGURATION_QUALITY_COUNT,
};

enum graphics_api_t {
	GRAPHICS_API_OPENGL,
	GRAPHICS_API_DIRECTX_11,
	GRAPHICS_API_DIRECTX_12,
	GRAPHICS_API_VULKAN,

	GRAPHICS_API_COUNT,
};

struct configuration_t {
	struct {
		f64                     MouseSensitivity;
		f64                     MouseAcc;
		std::map<u32, u32>      KeyMapping;
	} Input;
	
	struct {
		i32                     WindowWidth;
		i32                     WindowHeight;
		window_display_mode_t   WindowDisplayMode;
		bool                    EnableVSync;

		i32                     ApiVersionMajor;
		i32                     ApiVersionMinor;

		graphics_api_t          GraphicsApi;
		bool                    EnableApiDebugLogging;
	} Display;

	struct {
		configuration_quality_t Shadows;
	} Quality;

	struct {
		f32                     Fov;
		f32                     NearPlane;
		f32                     FarPlane;

		configuration_shadows_t Shadows;
	} Graphics;
};
