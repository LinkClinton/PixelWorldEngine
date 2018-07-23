#pragma once

#pragma once

#include "pch.hpp"

#include "Utility.hpp"
#include "GraphicsEnum.hpp"
#include "BaseStruct.hpp"
#include "ShaderResource.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		/**
		* @brief ��ά��������
		*/
		class Texture2DArray :public ShaderResource {
		private:
			Graphics * graphics; //..

			int width, height; //��Ⱥ͸߶�

			int size; //��С����λbyte
			int rowPitch; //һ��Ԫ�صĴ�С
			int mipLevels; //mipLevel
			int arraySize; //�����С

			PixelFormat pixelFormat; //���ظ�ʽ

#ifdef _WIN32
		public:
			D3D11_TEXTURE2D_DESC desc; //��������ӿ�
#endif // _WIN32

		public:
			/**
			* @brief ���캯��
			* @param[in] graphics ͼ�νӿ�
			* @param[in] Width ���
			* @param[in] Height �߶�
			* @param[in] arraySize �����С
			* @param[in] PixelFormat ���ظ�ʽ
			* @param[in] MipLevels mipLevel
			*/
			Texture2DArray(Graphics* graphics, int Width, int Height, int arraySize = 1, PixelFormat PixelFormat = PixelFormat::Unknown, int MipLevels = 1);

			/**
			* @brief ��������
			*/
			~Texture2DArray();

			/**
			* @brief �����������һ���ָ��ƹ���
			* @parma[in] srcTexture �����Ƶ�����
			* @param[in] dstPositionX ���Ƶ��������X��ʼλ��
			* @param[in] dstPositionY ���Ƶ��������Y��ʼλ��
			* @param[in] arrayIndex Ҫ���Ƶ�����������
			* @param[in] srcRect Ҫ���Ƶ�Դ���������
			*/
			void CopyFromTexture2D(Texture2D* srcTexture, int dstPositionX, int dstPositionY, int arrayIndex, PixelWorldEngine::Rectangle srcRect);

			/**
			 * @brief ���������鸴�Ƶ�����
			 * @param[in] srcTextureArray Ҫ�����Ƶ���������
			 * @param[in] dstArrayStartIndex ���Ƶ����������ʼλ��
			 * @param[in] srcArrayStartIndex �����Ƶ��������ʼλ��
			 * @param[in] arraySize Ҫ���ƵĴ�С
			 */
			void CopyFromTexture2DArray(Texture2DArray* srcTextureArray, int dstArrayStartIndex, int srcArrayStartIndex, int arraySize);

			/**
			* @brief ��������
			* @param[in] data ���ݣ�ע���Сһ��
			* @param[in] arrayIndex Ҫ���Ƶ�����������
			*/
			void Update(void* data, int arrayIndex);

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
			* @brief ��ȡ�����С
			* @return �����С����λbyte
			*/
			auto GetSize() -> int;

			/**
			 * @brief �������������С
			 * @return ���������С
			 */
			auto GetArraySize() -> int;

			/**
			* @brief ��ȡMipLevel
			* @return MipLevel11
			*/
			auto GetMipLevels() -> int;

			/**
			* @brief ��ȡ���ظ�ʽ
			* @return ���ظ�ʽ
			*/
			auto GetPixelFormat()->PixelFormat;
		};


	}
}