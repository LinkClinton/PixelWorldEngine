#include "Text.hpp"

#include "Encoding.hpp"
#include "DebugLayer.hpp"

#define MAX_TEXT_WIDTH 1000000
#define MAX_TEXT_HEIGHT 1000000

auto PixelWorldEngine::Text::CalculateSize(std::string text, Graphics::Font* font, int maxWidth, int maxHeight) -> std::pair<int, int>
{
	auto utf16Text = Encoding::ToUTF16String(text);

	//��ȡȫ�ֶ���
	auto sizeMetrics = font->fontFace->size->metrics;

	//��ȡ��׼��֮��ľ���
	auto baseLineDistance = sizeMetrics.height >> 6;

	int penPositionX = 0;
	int penPositionY = baseLineDistance;

	std::pair<int, int> result;

	for (auto it = utf16Text.begin(); it != utf16Text.end(); it++) {

		//����
		if (*it == '\n') {
			penPositionX = 0;
			penPositionY += baseLineDistance;

			result.first = Utility::Max(result.first, penPositionX);
			result.second = Utility::Max(result.second, penPositionY);

			continue;
		}

		//��ȡ��������
		auto codeMetrics = font->GetCharacterCodeMetrics(*it);

		//����
		if (penPositionX + codeMetrics.advance >= maxWidth) {
			
			penPositionX = 0;
			penPositionY += baseLineDistance;
		}

		//�ƶ��ʵ�X����
		penPositionX += codeMetrics.advance;

		//������������������Ҫ������Ⱦ������б�Ҫ���������ֿ��
		result.first = Utility::Max(result.first, penPositionX + codeMetrics.horiBearingX);
		result.second = Utility::Max(result.second, penPositionY);
	}

	result.first = Utility::Min(result.first, maxWidth);
	result.second = Utility::Min(result.second, maxHeight);

	return result;
}

void PixelWorldEngine::Text::CreateText(Text* text)
{
	//ת��ΪUTF16
	auto utf16Text = Encoding::ToUTF16String(text->text);

	//��ȡȫ�ֶ���
	auto sizeMetrics = text->font->fontFace->size->metrics;

	//��ȡ�ַ�����ߵĸ߶�
	auto maxHeight = sizeMetrics.ascender - sizeMetrics.descender;
	
	//��ȡ��׼��֮��ľ���
	auto baseLineDistance = sizeMetrics.height >> 6;
	
	//��ȡ��׼��
	auto baseLine = ((sizeMetrics.height - maxHeight) / 2 + sizeMetrics.ascender) >> 6;

	int penPositionX = 0;
	int penPositionY = baseLine;

	for (auto it = utf16Text.begin(); it != utf16Text.end(); it++) {

		//����
		if (*it == '\n') {
			penPositionX = 0;
			penPositionY += baseLineDistance;
		
			continue;
		}

		//��ȡ��������
		auto codeMetrics = text->font->GetCharacterCodeMetrics(*it);

		//����
		if (penPositionX + codeMetrics.advance >= text->width) {
			penPositionX = 0;
			penPositionY += baseLineDistance;
		}
		
		//��ȡ��������ԣ�����������˷�Χ����ô���Ǿ�ֻ��������
		auto textureWidth = codeMetrics.texture->GetWidth();
		auto textureHeight = Utility::Min(codeMetrics.texture->GetHeight(), text->height - penPositionY + codeMetrics.horiBearingY);

		//�����Ѿ������˷�Χ
		if (textureHeight < 0) return;

		//�����ַ������ı�������
		text->textTexture->CopyFromTexture2D(codeMetrics.texture,
			penPositionX + codeMetrics.horiBearingX,
			penPositionY - codeMetrics.horiBearingY,
			Rectangle(0, 0, textureWidth, textureHeight));

		//�ƶ��ʵ�X����
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

	//����������Ϊ0�Ļ�����ô���Ǿͽ�������Ϊ���޴�
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
