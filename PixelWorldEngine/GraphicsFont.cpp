#include "GraphicsFont.hpp"

#include "Graphics.hpp"
#include "DataManager.hpp"

PixelWorldEngine::Graphics::Font::Font(Graphics * Graphics, FileData* data, int Size)
{
	graphics = Graphics;

	size = Size;

	fontData = new FileData(*data);

	auto error = FT_New_Memory_Face(graphics->fontLibrary, fontData->GetData(), fontData->GetSize(),
		0, &fontFace);

	error = FT_Set_Pixel_Sizes(fontFace, 0, size);

	error = FT_Select_Charmap(fontFace, FT_Encoding::FT_ENCODING_UNICODE);
}

PixelWorldEngine::Graphics::Font::~Font()
{
	FreeCache();
	FT_Done_Face(fontFace);

	Utility::Delete(fontData);
}

void PixelWorldEngine::Graphics::Font::FreeCache()
{
	for (auto it = characterIndex.begin(); it != characterIndex.end(); it++) 
		Utility::Delete(it->second.texture);

	characterIndex.clear();
}

auto PixelWorldEngine::Graphics::Font::GetCharacterCodeMetrics(char16_t character) -> CharacterCodeMetrics
{
	if (characterIndex.count(character) != 0) return characterIndex[character];

	auto index = FT_Get_Char_Index(fontFace, character);

	DebugLayer::Assert(index == 0, Error::UndefinedCharacter);

	auto error = FT_Load_Glyph(fontFace, index, FT_LOAD_DEFAULT);

	auto glyph = fontFace->glyph;

	if (glyph->format != FT_Glyph_Format::FT_GLYPH_FORMAT_BITMAP)
		FT_Render_Glyph(glyph, FT_Render_Mode::FT_RENDER_MODE_NORMAL);
	
	CharacterCodeMetrics codeMetrics;
	
	codeMetrics.advance = (glyph->advance.x >> 6);
	codeMetrics.horiBearingX = (glyph->metrics.horiBearingX >> 6);
	codeMetrics.horiBearingY = (glyph->metrics.horiBearingY >> 6);
	codeMetrics.texture = new Texture2D(graphics, glyph->bitmap.buffer, glyph->bitmap.width,
		glyph->bitmap.rows, PixelFormat::A8);

	characterIndex.insert(std::pair<char16_t, CharacterCodeMetrics>(character, codeMetrics));

	return codeMetrics;
}

auto PixelWorldEngine::Graphics::Font::GetSize() -> int
{
	return size;
}
