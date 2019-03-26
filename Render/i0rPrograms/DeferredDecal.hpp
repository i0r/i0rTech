#pragma once

class IShader;

class DeferredDecal {
	public:
		static bool Initialize();
		static void Render( renderable_t* decal );

		static IShader* Shader_;
};
