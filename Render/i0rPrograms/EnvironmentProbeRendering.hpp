#pragma once

struct environment_probe_t;
struct Itexture_t;

class EnvironmentProbeRendering {
	DECLARE_MEMBER( private, IFrameBuffer*, FrameBuffer )
	DECLARE_MEMBER( private, Itexture_t*,   Stencil )
	DECLARE_MEMBER( private, f32,           Direction )
	DECLARE_MEMBER( private, mat4x4f,       ViewMatrix )
	
	public:
		EnvironmentProbeRendering();
		~EnvironmentProbeRendering();

		bool Initialize();
		void RenderProbe( environment_probe_t* probe );
		void DrawSingleMesh( Mesh* meshData );
		void BuildSpecular();
};
