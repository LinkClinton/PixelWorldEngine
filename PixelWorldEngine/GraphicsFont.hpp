#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	class Text;
	class FileData;

	namespace Graphics {

		class Graphics;
		class Texture2D;

		/**
		 * @brief ���ζ���
		 */
		struct CharacterCodeMetrics {
		public:
			Texture2D* texture; //�洢���ε�����
			int advance; //����ԭ��֮��ľ���
			int horiBearingX; //ԭ�㵽������ߵľ���
			int horiBearingY; //ԭ��(��׼��)�����ζ����ľ���
		};

		/**
		 * @brief ����
		 */
		class Font {
		private:
			Graphics* graphics; //..

			FT_Face fontFace; //FreeType�����ļ�

			FileData* fontData; //��������

			int size; //�����С

			std::map<char16_t, CharacterCodeMetrics> characterIndex; //������������

			friend class PixelWorldEngine::Text;
		public:
			/**
			 * @brief ���캯��
			 * @param[in] graphics ͼ�νӿ�
			 * @param[in] data ��������
			 * @param[in] size �����С
			 */
			Font(Graphics* graphics, FileData* data, int size);

			/**
			 * @brief ��������
			 */
			~Font();

			/**
			 * @brief �ͷŻ�����Դ
			 */
			void FreeCache();

			/**
			 * @brief ��ȡ��Ӧ���ֵ��������ݣ�ע�����ֱ���ΪUTF-16��ֻ֧�ֳ�������
			 */
			auto GetCharacterCodeMetrics(char16_t characterCode) -> CharacterCodeMetrics;

			/**
			 * @brief ��ȡ�����С
			 * @return �����С
			 */
			auto GetSize() -> int;
		};

	}

}