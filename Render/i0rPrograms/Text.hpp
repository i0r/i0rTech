#pragma once

class IShader;

struct font_t;
struct renderable_text_t;

#define TEXT_MAX_LENGTH 4096

class Text {
	DECLARE_MEMBER( private, IShader*, Shader )

	#ifdef FLAG_DEBUG
		DECLARE_MEMBER( private, font_t*, FontDebug )
	#endif

	public:
		Text();
		~Text();

		bool Initialize();
		void Compute( font_t* font, const i32 x, const i32 y, colorrgbaf textColor, colorrgbaf outlineColor,
								        const f32 &fontSize, const char* text, ... );
		void Print( renderable_t* textString );

	private:
		char m_FinalTextBuffer[TEXT_MAX_LENGTH];
};
