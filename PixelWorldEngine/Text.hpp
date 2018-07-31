#pragma once

#include "pch.hpp"

#include "ShaderResource.hpp"
#include "GraphicsFont.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief �ı��������ͣ�ͨ���������ڹ����ı�����
	 */
	class Text {
	private:
		Graphics::Graphics* graphics; //..

		Graphics::Texture2D* textTexture; //�����ı�����
		Graphics::Font* font; //ʹ�õ�����

		std::string text; //�ı�

		int width; //�ı�����Ŀ��
		int height; //�ı�����ĸ߶�

		/**
		 * @brief �����ı�����Ĵ�С
		 * @param[in] text �ı�
		 * @param[in] font ʹ�õ�����
		 * @param[in] maxWidth ���Ŀ��
		 * @param[in] maxHeight ���ĸ߶�
		 * @return �ı�����Ĵ�С
		 */
		static auto CalculateSize(std::string text, Graphics::Font* font, int maxWidth, int maxHeight)->std::pair<int, int>;

		/**
		 * @brief �����ı����ݽ��������������ı�������ȥ
		 * @param[in] text �ı�
		 */
		static void CreateText(Text* text);
	public:
		/**
		 * @brief ���캯��
		 * @param[in] text �ı�
		 * @param[in] width ����ȣ����Ϊ0����ô��ֻ��һ��
		 * @param[in] height ���߶ȣ����Ϊ0����ô�Զ������ʺϵĸ߶�
		 */
		Text(std::string text, Graphics::Font* font, int maxWidth = 0, int maxHeight = 0);

		/**
		 * @brief ��ȡ���
		 * @return ���
		 */
		auto GetWidth() -> int;

		/**
		* @brief ��ȡ�߶�
		* @return �߶�
		*/
		auto GetHeight() -> int;

		/**
		 * @brief ��ȡ�ı�
		 * @return �ı�
		 */
		auto GetText() -> std::string;

		/**
		 * @brief ��ȡ����
		 * @return ����
		 */
		auto GetFont() -> Graphics::Font*;

		/**
		 * @brief ��ȡ����
		 * @return ����
		 */
		auto GetTexture() -> Graphics::Texture2D*; 
	};

}