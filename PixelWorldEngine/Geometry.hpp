#pragma once

#include "pch.hpp"

#include "BaseStruct.hpp"

#include "Graphics.hpp"
#include "BufferResource.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		/**
		 * @brief 图形基本类型，用于生存各种基本图形
		 */
		class Geometry {
		protected:
			Graphics* graphics; //..

			Buffer* vertexBuffer; //顶点缓冲
			Buffer* indexBuffer; //索引缓冲
		public:
			/**
			 * @brief 构造函数
			 * @param[in] graphics 图形接口
			 */
			Geometry(Graphics* graphics);

			/**
			 * @brief 析构函数
			 */
			~Geometry();

			/**
			 * @brief 获取顶点缓冲
			 * @return 顶点缓冲的地址
			 */
			auto GetVertexBuffer() -> Buffer*;

			/**
			 * @brief 获取索引缓冲
			 * @return 索引缓冲的地址
			 */
			auto GetIndexBuffer() -> Buffer*;
		};

		/**
		 * @brief 矩形图形类型，注意其顶点Z轴方向的值为1
		 */
		class RectangleF :public Geometry {
		public:
			/**
			 * @brief 构造函数
			 * @param[in] left 左边界
			 * @param[in] top 上边界
			 * @param[in] right 右边界
			 * @param[in] bottom 下边界
			 * @param[in] graphics 图形接口
			 */
			RectangleF(float left, float top, float right, float bottom, Graphics* graphics);
		};

	}

}