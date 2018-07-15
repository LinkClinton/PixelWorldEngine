#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	class Text;
	class FileData;

	namespace Graphics {

		class Graphics;
		class Texture2D;

		/**
		 * @brief 字形度量
		 */
		struct CharacterCodeMetrics {
		public:
			Texture2D* texture; //存储字形的纹理
			int advance; //两个原点之间的距离
			int horiBearingX; //原点到字形左边的距离
			int horiBearingY; //原点(基准线)到字形顶部的距离
		};

		/**
		 * @brief 字体
		 */
		class Font {
		private:
			Graphics* graphics; //..

			FT_Face fontFace; //FreeType字体文件

			FileData* fontData; //字体数据

			int size; //字体大小

			std::map<char16_t, CharacterCodeMetrics> characterIndex; //字体索引缓存

			friend class PixelWorldEngine::Text;
		public:
			/**
			 * @brief 构造函数
			 * @param[in] graphics 图形接口
			 * @param[in] data 字体数据
			 * @param[in] size 字体大小
			 */
			Font(Graphics* graphics, FileData* data, int size);

			/**
			 * @brief 析构函数
			 */
			~Font();

			/**
			 * @brief 释放缓存资源
			 */
			void FreeCache();

			/**
			 * @brief 获取对应文字的字形数据，注意文字编码为UTF-16，只支持常用文字
			 */
			auto GetCharacterCodeMetrics(char16_t characterCode) -> CharacterCodeMetrics;

			/**
			 * @brief 获取字体大小
			 * @return 字体大小
			 */
			auto GetSize() -> int;
		};

	}

}