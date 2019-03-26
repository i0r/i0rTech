#pragma once

// note: the enum order must be respected, since the sorting function is based on this order
enum renderable_type_t {
	RENDERABLE_TYPE_UNKNOWN,
	RENDERABLE_TYPE_EDITOR_GRID,
	RENDERABLE_TYPE_DECAL,
	RENDERABLE_TYPE_PRIMITIVE,
	RENDERABLE_TYPE_MESH,
	RENDERABLE_TYPE_WIREFRAME,
	RENDERABLE_TYPE_UI_TEXTURE,
	RENDERABLE_TYPE_TEXT,
};

struct renderable_t {
	explicit renderable_t()  : Type( RENDERABLE_TYPE_UNKNOWN ), 
								                            Object( nullptr ) {

	}
	
	~renderable_t() {
		Type   = RENDERABLE_TYPE_UNKNOWN;
		Object = nullptr; // do not delete the object unless we are explicitly asked to
	}

	renderable_type_t   Type;
	void*               Object;
};

static INLINE bool RenderableCompare( renderable_t* r1, renderable_t* r2 ) {
	return r1->Type < r2->Type;
}
