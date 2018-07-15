#pragma once

#include "pch.hpp"

#include "GraphicsEnum.hpp"
#include "ShaderResource.hpp"

namespace PixelWorldEngine {

	class Application;

	namespace Graphics {

		class Graphics;

		/**
		 * @brief ��ȾĿ�꣬ע����ȾĿ�겢���洢��Ⱦ����
		 */
		class RenderTarget {
		private:
			Graphics* graphics; //..
			
			int width, height; //��Ⱥ͸߶�

			PixelFormat pixelFormat; //���ݸ�ʽ

#ifdef _WIN32
		public:
			ID3D11RenderTargetView* renderTarget; //Direct3D �ӿ�
#endif // _WIN32

		public:
			/**
			 * @brief ���캯��
			 * @param[in] graphics ͼ�νӿ�
			 * @param[in] texture �����洢��Ⱦ���
			 */
			RenderTarget(Graphics* graphics, Texture2D* texture);

			/**
			 * @brief ���캯��
			 * @param[in] graphics ͼ�νӿ�
			 * @param[in] application Ӧ�ó���
			 */
			RenderTarget(Graphics* graphics, Application* application);

			/**
			 * @brief ��������
			 */
			~RenderTarget();

			/**
			 * @brief ��������������ɫ
			 * @param[in] red ��ɫ����
			 * @param[in] green ��ɫ����
			 * @param[in] blue ��ɫ����
			 * @param[in] alpha ͸���ȷ���
			 */
			void Clear(float red, float green, float blue, float alpha = 1);

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
			 * @brief ��ȡ���ظ�ʽ
			 * @return ���ظ�ʽ
			 */
			auto GetPixelFormat() -> PixelFormat;
		};

	}

}
