#pragma once

#include <i0rCore/Colors.hpp>

struct font_t;

struct renderable_char_t {
	vec4f T1;
	vec4f T2;
};

struct renderable_string_t {
	~renderable_string_t() {
		delete[] Characters;
		Font         = nullptr;
		TextLength   = 0;
		TextColor    = colorrgbaf();
		OutlineColor = colorrgbaf();
	}

	font_t*                 Font;
	i32                     TextLength;
	colorrgbaf              TextColor;
	colorrgbaf              OutlineColor;
	renderable_char_t*      Characters;
}; // struct renderable_text_string_t
