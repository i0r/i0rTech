#pragma once

class DeferredTiled {
	#define WORK_GROUP_SIZE 16

	DECLARE_MEMBER( private, IFrameBuffer*, FrameBuffer )
	DECLARE_MEMBER( private, IShader*,      Shader )

	public:
		DeferredTiled();
		~DeferredTiled();
			
		bool Initialize();
		bool OnResize();

		void StartGeometryPass();

		void DrawSingle( renderable_t* mesh );

		void Render();
};
