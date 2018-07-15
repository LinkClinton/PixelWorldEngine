#pragma once

#include "pch.hpp"

#include "GraphicsEnum.hpp"
#include "ShaderResource.hpp"

namespace PixelWorldEngine {

	class Application;

	namespace Graphics {

		class Graphics;

		/**
		 * @brief 渲染目标，注意渲染目标并不存储渲染数据
		 */
		class RenderTarget {
		private:
			Graphics* graphics; //..
			
			int width, height; //宽度和高度

			PixelFormat pixelFormat; //数据格式

#ifdef _WIN32
		public:
			ID3D11RenderTargetView* renderTarget; //Direct3D 接口
#endif // _WIN32

		public:
			/**
			 * @brief 构造函数
			 * @param[in] graphics 图形接口
			 * @param[in] texture 纹理，存储渲染结果
			 */
			RenderTarget(Graphics* graphics, Texture2D* texture);

			/**
			 * @brief 构造函数
			 * @param[in] graphics 图形接口
			 * @param[in] application 应用程序
			 */
			RenderTarget(Graphics* graphics, Application* application);

			/**
			 * @brief 析构函数
			 */
			~RenderTarget();

			/**
			 * @brief 清屏并将设置颜色
			 * @param[in] red 红色分量
			 * @param[in] green 绿色分量
			 * @param[in] blue 蓝色分量
			 * @param[in] alpha 透明度分量
			 */
			void Clear(float red, float green, float blue, float alpha = 1);

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
			 * @brief 获取像素格式
			 * @return 像素格式
			 */
			auto GetPixelFormat() -> PixelFormat;
		};

	}

}
