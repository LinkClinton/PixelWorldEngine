#include "Text.hpp"

#include "Encoding.hpp"
#include "DebugLayer.hpp"

#define MAX_TEXT_WIDTH 1000000
#define MAX_TEXT_HEIGHT 1000000

auto PixelWorldEngine::Text::CalculateSize(std::string text, Graphics::Font* font, int maxWidth, int maxHeight) -> std::pair<int, int>
{
	auto utf16Text = Encoding::ToUTF16String(text);

	//获取全局度量
	auto sizeMetrics = font->fontFace->size->metrics;

	//获取标准线之间的距离
	auto baseLineDistance = sizeMetrics.height >> 6;

	int penPositionX = 0;
	int penPositionY = baseLineDistance;

	std::pair<int, int> result;

	for (auto it = utf16Text.begin(); it != utf16Text.end(); it++) {

		//换行
		if (*it == '\n') {
			penPositionX = 0;
			penPositionY += baseLineDistance;

			result.first = Utility::Max(result.first, penPositionX);
			result.second = Utility::Max(result.second, penPositionY);

			continue;
		}

		//获取文字纹理
		auto codeMetrics = font->GetCharacterCodeMetrics(*it);

		//换行
		if (penPositionX + codeMetrics.advance >= maxWidth) {
			
			penPositionX = 0;
			penPositionY += baseLineDistance;
		}

		//移动笔的X坐标
		penPositionX += codeMetrics.advance;

		//宽度上由于这个文字需要进行渲染，因此有必要考虑其文字宽度
		result.first = Utility::Max(result.first, penPositionX + codeMetrics.horiBearingX);
		result.second = Utility::Max(result.second, penPositionY);
	}

	result.first = Utility::Min(result.first, maxWidth);
	result.second = Utility::Min(result.second, maxHeight);

	return result;
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

PixelWorldEngine::Text::Text(std::string Text, Graphics::Font * Font, int maxWidth, int maxHeight)
{
	DebugReturn(DebugLayer::Assert(maxWidth < 0, Error::WidthOrHeightLessThanZero, "Text", FunctionName));
	DebugReturn(DebugLayer::Assert(Font == nullptr, Error::TheObjectIsNull, "Font", FunctionName));

	graphics = Font->graphics;

	text = Text;
	font = Font;

	//当最大的限制为0的话，那么我们就将其设置为无限大
	if (maxWidth == 0) maxWidth = MAX_TEXT_WIDTH;
	if (maxHeight == 0) maxHeight = MAX_TEXT_HEIGHT;

	auto size = CalculateSize(text, font, maxWidth, maxHeight);
	
	width = size.first;
	height = size.second;

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
