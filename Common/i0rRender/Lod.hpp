#pragma once

class IIndiceBuffer;
class IVertexBuffer;

struct lod_t {
	explicit lod_t() ;
	~lod_t() ;
	void Draw() const ;

	f32                            LOD;
	
	IVertexBuffer*                 VBO;
	IIndiceBuffer*                 VAO;
};

static bool CompareLOD( lod_t* lod1, lod_t* lod2 ) {
	return lod1->LOD < lod2->LOD;
}
