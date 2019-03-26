#pragma once

class IVertexArray;
class IVertexBuffer;

class Quad {
	public:
		static IIndiceBuffer* VAO;
		static IVertexBuffer* VBO;

		static void Create() ;
};
