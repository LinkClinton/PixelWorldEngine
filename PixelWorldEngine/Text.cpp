#include "Text.hpp"

#include "Encoding.hpp"

auto PixelWorldEngine::Text::CalculateHeight(std::string text, Graphics::Font* font, int textWidth) -> int
{
	auto utf16Text = Encoding::ToUTF16String(text);

	//��ȡȫ�ֶ���
	auto sizeMetrics = font->fontFace->size->metrics;

	//��ȡ�ַ�����ߵĸ߶�
	auto maxHeight = sizeMetrics.ascender - sizeMetrics.descender;

	//��ȡ��׼��֮��ľ���
	auto baseLineDistance = sizeMetrics.height >> 6;

	int penPositionX = 0;
	int penPositionY = baseLineDistance;

	for (auto it = utf16Text.begin(); it != utf16Text.end(); it++) {

		//����
		if (*it == '\n') {
			penPositionX = 0;
			penPositionY += baseLineDistance;

			continue;
		}

		//��ȡ��������
		auto codeMetrics = font->GetCharacterCodeMetrics(*it);

		//����
		if (penPositionX + codeMetrics.advance >= textWidth) {
			penPositionX = 0;
			penPositionY += baseLineDistance;
		}

		//�ƶ��ʵ�X����
		penPositionX += codeMetrics.advance;
	}

	return penPositionY;
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
