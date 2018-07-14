#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	class Text;
	class FileData;

	namespace Graphics {

		class Graphics;
		class Texture2D;

		struct CharacterCodeMetrics {
		public:
			Texture2D* texture;
			int advance;
			int horiBearingX;
			int horiBearingY;
		};

		class Font {
		private:
			Graphics* graphics;

			FT_Face fontFace;

			FileData* fontData;

			int size;

			std::map<char16_t, CharacterCodeMetrics> characterIndex;

			friend class PixelWorldEngine::Text;
		public:
			Font(Graphics* graphics, FileData* data, int size);

			~Font();

			void FreeCache();

			auto GetCharacterCodeMetrics(char16_t characterCode) -> CharacterCodeMetrics;

			auto GetSize() -> int;
		};

	}

}