#pragma once

struct Itexture_t;

struct renderable_ui_texture_t {
	explicit renderable_ui_texture_t()  : X( 0 ), 
								                                       Y( 0 ), 
								                                       Texture( nullptr ), 
								                                       Width( 0 ),
								                                       Height( 0 ) {

	}

	~renderable_ui_texture_t() {
		X       = 0;
		Y       = 0;
		Width   = 0;
		Height  = 0;
		Texture = nullptr;
	}

	i32         X;
	i32         Y;
	u32         Width;
	u32         Height;
	Itexture_t* Texture;
};
