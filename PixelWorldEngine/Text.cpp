#include "Text.hpp"

#include "Encoding.hpp"

auto PixelWorldEngine::Text::CalculateHeight(std::string text, Graphics::Font* font, int textWidth) -> int
{
	auto utf16Text = Encoding::ToUTF16String(text);

	//获取全局度量
	auto sizeMetrics = font->fontFace->size->metrics;

	//获取字符中最高的高度
	auto maxHeight = sizeMetrics.ascender - sizeMetrics.descender;

	//获取标准线之间的距离
	auto baseLineDistance = sizeMetrics.height >> 6;

	int penPositionX = 0;
	int penPositionY = baseLineDistance;

	for (auto it = utf16Text.begin(); it != utf16Text.end(); it++) {

		//换行
		if (*it == '\n') {
			penPositionX = 0;
			penPositionY += baseLineDistance;

			continue;
		}

		//获取文字纹理
		auto codeMetrics = font->GetCharacterCodeMetrics(*it);

		//换行
		if (penPositionX + codeMetrics.advance >= textWidth) {
			penPositionX = 0;
			penPositionY += baseLineDistance;
		}

		//移动笔的X坐标
		penPositionX += codeMetrics.advance;
	}

	return penPositionY;
}

void PixelWorldEngine::Text::CreateText(Text* text)
{
	//转换为UTF16
	auto utf16Text = Encoding::ToUTF16String(text->text);

	//获取全局度量
	auto sizeMetrics = text->font->fontFace->size->metrics;

	//获取字符中最高的高度
	auto maxHeight = sizeMetrics.ascender - sizeMetrics.descender;
	
	//获取标准线之间的距离
	auto baseLineDistance = sizeMetrics.height >> 6;
	
	//获取标准线
	auto baseLine = ((sizeMetrics.height - maxHeight) / 2 + sizeMetrics.ascender) >> 6;

	int penPositionX = 0;
	int penPositionY = baseLine;

	for (auto it = utf16Text.begin(); it != utf16Text.end(); it++) {

		//换行
		if (*it == '\n') {
			penPositionX = 0;
			penPositionY += baseLineDistance;
		
			continue;
		}

		//获取文字纹理
		auto codeMetrics = text->font->GetCharacterCodeMetrics(*it);

		//换行
		if (penPositionX + codeMetrics.advance >= text->width) {
			penPositionX = 0;
			penPositionY += baseLineDistance;
		}
		
		//获取纹理的属性，如果纹理超过了范围，那么我们就只拷贝部分
		auto textureWidth = codeMetrics.texture->GetWidth();
		auto textureHeight = Utility::Min(codeMetrics.texture->GetHeight(), text->height - penPositionY + codeMetrics.horiBearingY);

		//文字已经超过了范围
		if (textureHeight < 0) return;

		//拷贝字符纹理到文本纹理上
		text->textTexture->CopyFromTexture2D(codeMetrics.texture,
			penPositionX + codeMetrics.horiBearingX,
			penPositionY - codeMetrics.horiBearingY,
			Rectangle(0, 0, textureWidth, textureHeight));

		//移动笔的X坐标
		penPositionX += codeMetrics.advance;
	}
}

PixelWorldEngine::Text::Text(std::string Text, Graphics::Font * Font, int Width, int Height)
{
	graphics = Font->graphics;

	text = Text;
	font = Font;

	width = Width;
	height = Height;

	if (height == 0)
		height = CalculateHeight(text, font, width);

	textTexture = new Graphics::Texture2D(graphics, nullptr, width, height, Graphics::PixelFormat::A8);

	CreateText(this);
}

auto PixelWorldEngine::Text::GetWidth() -> int
{
	return width;
}

auto PixelWorldEngine::Text::GetHeight() -> int
{
	return height;
}

auto PixelWorldEngine::Text::GetText() -> std::string
{
	return text;
}

auto PixelWorldEngine::Text::GetFont() -> Graphics::Font *
{
	return font;
}

auto PixelWorldEngine::Text::GetTexture() -> Graphics::Texture2D *
{
	return textTexture;
}
