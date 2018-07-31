#pragma once

#include "pch.hpp"

#include "ShaderResource.hpp"
#include "GraphicsFont.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief 文本生成类型，通过字体用于构造文本纹理
	 */
	class Text {
	private:
		Graphics::Graphics* graphics; //..

		Graphics::Texture2D* textTexture; //最后的文本纹理
		Graphics::Font* font; //使用的字体

		std::string text; //文本

		int width; //文本纹理的宽度
		int height; //文本纹理的高度

		/**
		 * @brief 计算文本纹理的大小
		 * @param[in] text 文本
		 * @param[in] font 使用的字体
		 * @param[in] maxWidth 最大的宽度
		 * @param[in] maxHeight 最大的高度
		 * @return 文本纹理的大小
		 */
		static auto CalculateSize(std::string text, Graphics::Font* font, int maxWidth, int maxHeight)->std::pair<int, int>;

		/**
		 * @brief 根据文本数据将文字纹理创建到文本纹理中去
		 * @param[in] text 文本
		 */
		static void CreateText(Text* text);
	public:
		/**
		 * @brief 构造函数
		 * @param[in] text 文本
		 * @param[in] width 最大宽度，如果为0，那么就只有一行
		 * @param[in] height 最大高度，如果为0，那么自动生成适合的高度
		 */
		Text(std::string text, Graphics::Font* font, int maxWidth = 0, int maxHeight = 0);

		/**
		 * @brief 获取宽度
		 * @return 宽度
		 */
		auto GetWidth() -> int;

		/**
		* @brief 获取高度
		* @return 高度
		*/
		auto GetHeight() -> int;

		/**
		 * @brief 获取文本
		 * @return 文本
		 */
		auto GetText() -> std::string;

		/**
		 * @brief 获取字体
		 * @return 字体
		 */
		auto GetFont() -> Graphics::Font*;

		/**
		 * @brief 获取纹理
		 * @return 纹理
		 */
		auto GetTexture() -> Graphics::Texture2D*; 
	};

}