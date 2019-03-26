#pragma once

class IIndiceBuffer;
class IVertexBuffer;

class EditorGrid {
	public:
		static IIndiceBuffer*   VAO;
		static IVertexBuffer*   VBO;
		static mat4x4f          ModelMatrix;
		static IShader*         ShaderMrt;
		static Itexture_t*      Texture;
		static material_data_t* Material;

		static void Create() ;
		static void Draw() ;
};
