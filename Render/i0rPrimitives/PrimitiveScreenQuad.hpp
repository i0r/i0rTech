#pragma once

class VertexArray;
class VertexBuffer;

class ScreenQuad {
	public:
		static IIndiceBuffer* VAO;
		static IVertexBuffer* VBO;

		static void Create() ;
};
