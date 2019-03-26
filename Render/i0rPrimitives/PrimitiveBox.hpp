#pragma once

class IIndiceBuffer;
class IVertexBuffer;

class Box {
	public:
		static IIndiceBuffer* VAO;
		static IVertexBuffer* VBO;

		static void Create() ;
};
