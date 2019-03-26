#pragma once

	class AABBWireframe {
		public:
			static bool Initialize() ;
			static void Render( renderable_t* prim ) ;
			static IShader* m_Shader;
	};
