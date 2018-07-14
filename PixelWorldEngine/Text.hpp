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
		 * @brief �����ı�����ĸ߶�
		 * @param[in] text �ı�
		 * @param[in] font ʹ�õ�����
		 * @param[in] textWidth �ı����
		 * @return �ı�����ĸ߶�
		 */
		static auto CalculateHeight(std::string text, Graphics::Font* font, int textWidth) -> int;

		/**
		 * @brief �����ı����ݽ��������������ı�������ȥ
		 * @param[in] text �ı�
		 */
		static void CreateText(Text* text);
	public:
		/**
		 * @brief ���캯��
		 * @param[in] text �ı�
		 * @param[in] width ���
		 * @param[in] height �߶ȣ����Ϊ0����ô�Զ�����
		 */
		Text(std::string text, Graphics::Font* font, int width, int height = 0);

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