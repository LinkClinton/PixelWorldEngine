#pragma once

#include "pch.hpp"

#include "Graphics.hpp"
#include "DataManager.hpp"

namespace PixelWorldEngine {

	class Text;

	namespace Graphics {

		class Font {
		private:
			Graphics* graphics;

			FT_Face fontFace;

			int size;

			friend class PixelWorldEngine::Text;
		public:
			Font(Graphics* graphics, FileData* data, int size);
				
			auto GetSize() -> int;
		};

	}

}