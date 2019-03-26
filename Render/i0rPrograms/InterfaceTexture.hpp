#pragma once

struct renderable_t;
class IShader;

class InterfaceTexture {
	public:
		static bool Initialize();
		static void Render( renderable_t* renderable );

		static IShader* Shader_;
};
