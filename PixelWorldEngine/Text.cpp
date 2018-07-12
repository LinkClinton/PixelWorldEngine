#include "Text.hpp"

void PixelWorldEngine::Text::CreateTextTexture()
{
	if (font == nullptr || text == "") return;

	byte color256[3];

	color256[0] = (byte)(color[0] * 255);
	color256[1] = (byte)(color[1] * 255);
	color256[2] = (byte)(color[2] * 255);


}

PixelWorldEngine::Text::Text(Graphics::Font * Font, std::string Text, float red, float green, float blue)
{
	graphics = Font->graphics;

	text = Text;
	font = Font;

	color[0] = red;
	color[1] = green;
	color[2] = blue;

	width = font->size * text.length();
	height = font->size;

	textTexture = nullptr;

	CreateTextTexture();
}
