#include "GraphicsFont.hpp"

PixelWorldEngine::Graphics::Font::Font(Graphics * Graphics, FileData* data, int size)
{
	graphics = Graphics;

	FT_New_Memory_Face(graphics->fontLibrary, data->GetData(), data->GetSize(),
		0, &fontFace);

	FT_Set_Pixel_Sizes(fontFace, 0, size);

	FT_Select_Charmap(fontFace, FT_Encoding::FT_ENCODING_UNICODE);
}

auto PixelWorldEngine::Graphics::Font::GetSize() -> int
{
	return size;
}
