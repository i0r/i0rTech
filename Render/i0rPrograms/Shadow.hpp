#pragma once

class Shadow {
	DECLARE_MEMBER( private, f32,     OrthoNear )
	DECLARE_MEMBER( private, f32,     OrthoFar )
	DECLARE_MEMBER( private, IShader*, Shader )
	DECLARE_MEMBER( private, mat4x4f, ViewProjection )

	public: 
		Shadow();
		~Shadow();
		
		bool Initialize();
		void RenderScene( mat4x4f viewProjection );

	private:
		void DrawSingleMesh( Mesh* meshData );
};
