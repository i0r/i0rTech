#pragma once

class IFrameBuffer;
class IShader;

class Composition {
	public:
		static bool     Initialize() ;
		static void     Render( IFrameBuffer* fbo ) ;

		static IShader* Shader_;

	private:
		static void CreateShader() ;
};
