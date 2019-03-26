#pragma once

class ShadowCascade {
	public:
		static void ComputeMatrices() ;
		static void ShadowPass() ;

		static void Create() ;

		static void Debug() ;

		static IFrameBuffer* sun;
		static IShader* shaderDebug;

	private:
		static f32 GetSceneRadius() ;
		static void DrawSingleMesh( renderable_t* mesh ) ;
		static f32 ComputeCSMSplitDistance( const i32 split ) ;
};
