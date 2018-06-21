#pragma once

#include "pch.hpp"

#include "BaseStruct.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PixelWorldEngine {

	/**
	 * @brief 摄像机，决定我们看到的范围以及区域
	 */
	class Camera {
	private:
		RectangleF rect; //摄像机能够看到的区域

		glm::mat4x4 project; //投影矩阵，注意我们的投影类型
	public:
		/**
		 * @brief 默认构造函数
		 */
		Camera() = default;

		/**
		 * @brief 构造函数
		 * @param[in] rectangle 表示摄像机能够看到的范围
		 */
		Camera(RectangleF area);

		/**
		 * @brief 设置摄像机能够看到的范围
		 * @param[in] area 表示范围
		 */
		void SetRectangle(RectangleF area);

		/**
		 * @brief 获取摄像机能够看到的范围
		 * @return 一个矩形类型，描述范围
		 */
		auto GetRectangle() -> RectangleF;

		/**
		 * @brief 获取摄像机对应投影矩阵
		 * @return 一个4x4的矩阵
		 */
		auto GetMatrix() -> glm::mat4x4;
	};

}
