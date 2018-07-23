#pragma once

#include "pch.hpp"

#include "Utility.hpp"
#include "GraphicsEnum.hpp"
#include "BaseStruct.hpp"
#include "ShaderResource.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		/**
		* @brief ��ά����
		*/
		class Texture2D :public ShaderResource {
		private:
			Graphics * graphics; //..

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
			* @param[in] dstPositionX ���Ƶ��������X��ʼλ��
			* @param[in] dstPositionY ���Ƶ��������Y��ʼλ��
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

	}
}