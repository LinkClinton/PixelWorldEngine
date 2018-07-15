#pragma once

#include "pch.hpp"

#include "Utility.hpp"
#include "GraphicsEnum.hpp"
#include "BaseStruct.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		class Graphics;

		/**
		 * @brief ��ɫ����Դ
		 */
		class ShaderResource {
#ifdef _WIN32
		public:
			ID3D11Resource* resource; //Direct3D �ӿ�
			ID3D11ShaderResourceView* resourceView; //Direct3D �ӿ�
#endif // _WIN32
		public:
			/**
			 * @brief ��������
			 */
			~ShaderResource();

		};

		/**
		 * @brief ��ά����
		 */
		class Texture2D :public ShaderResource {
		protected:
			Graphics* graphics; //..

			int width, height; //��Ⱥ͸߶�

			int size; //��С����λbyte
			int rowPitch; //һ��Ԫ�صĴ�С
			int mipLevels; //mipLevel

			PixelFormat pixelFormat; //���ظ�ʽ

#ifdef _WIN32
		public:
			D3D11_TEXTURE2D_DESC desc; //��������ӿ�
#endif // _WIN32

		public:
			/**
			 * @brief ���캯��
			 * @param[in] graphics ͼ�νӿ�
			 * @param[in] Data ����
			 * @param[in] Width ���
			 * @param[in] Height �߶�
			 * @param[in] PixelFormat ���ظ�ʽ
			 * @param[in] MipLevels mipLevel
			 */
			Texture2D(Graphics* graphics, void* Data, int Width, int Height, PixelFormat PixelFormat = PixelFormat::Unknown, int MipLevels = 1);

			/**
			 * @brief ���캯�����������������������
			 * @param[in] srcTexture Դ����
			 * @param[in] srcRect Ҫ�����������Χ
			 */
			Texture2D(Texture2D* srcTexture, PixelWorldEngine::Rectangle srcRect);

			/**
			 * @brief ��������
			 */
			~Texture2D();

			/**
			 * @brief �����������һ���ָ��ƹ���
			 * @parma[in] srcTexture �����Ƶ�����
			 * @param[in] ���Ƶ��������X��ʼλ��
			 * @param[in] ���Ƶ��������Y��ʼλ��
			 * @param[in] srcRect Ҫ���Ƶ�Դ���������
			 */
			void CopyFromTexture2D(Texture2D* srcTexture, int dstPositionX, int dstPositionY, PixelWorldEngine::Rectangle srcRect);

			/**
			 * @brief ��������
			 * @param[in] data ���ݣ�ע���Сһ��
			 */
			void Update(void* data);

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

		/**
		 * @brief �������飬����������������ɫ����Դ�ķ�ʽ�洢
		 */
		class BufferArray :public ShaderResource {
		protected:
			Graphics* graphics; //..

			int size; //��С����λbyte
			int count; //����ĳ���

#ifdef _WIN32
		public:
			D3D11_BUFFER_DESC desc; //��������ṹ

			ID3D11Buffer* buffer; //Direct3D �ӿ�
#endif // _WIN32
			
		public:
			/**
			 * @brief ���캯��
			 * @param[in] graphics ͼ�νӿ�
			 * @param[in] data ������Ϣ
			 * @param[in] dataSize ���ݴ�С
			 * @param[in] dataCount ������������飬��ô�䳤�ȣ�����������Ϊ1
			 */
			BufferArray(Graphics* graphics, void* data, int dataSize, int dataCount = 1);

			/**
			 * @brief ��������
			 */
			~BufferArray();

			/**
			 * @brief ��������
			 * @param[in] ���ݣ�ע�����ݴ�Сһ��
			 */
			void Update(void* data);

			/**
			 * @brief ��ȡ�����С
			 * @return �����С����λbyte
			 */
			auto GetSize() -> int;

			/**
			 * @brief ��ȡ����
			 * @return ����
			 */
			auto GetCount() -> int;
		};
	}
}
