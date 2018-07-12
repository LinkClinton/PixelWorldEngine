#pragma once

#include "pch.hpp"

#include "ShaderResource.hpp"
#include "GraphicsFont.hpp"

namespace PixelWorldEngine {

	class Text {
	private:
		std::vector<byte> textData;

		Graphics::Graphics* graphics;

		Graphics::Texture2D* textTexture;
		Graphics::Font* font;

		std::string text;

		int width;
		int height;

		float color[3];

		void CreateTextTexture();
	public:
		Text(Graphics::Font* font, std::string text = "",
			float red = 0.0f, float green = 0.0f , float blue = 0.0f);

		void SetText(std::string text);

		void SetFont(Graphics::Font* font);

		void SetColor(float red = 0.0f, float green = 0.0f, float blue = 0.0f);

		auto GetText() -> std::string;

		auto GetFont() -> Graphics::Font*;

		auto GetColor() -> float*;

		auto GetTexture() -> Graphics::Texture2D*; 
	};

}