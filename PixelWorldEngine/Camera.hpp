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
		 * @brief 设置摄像机能够看到的范围以及看到的区域
		 * @param[in] area 表示范围
		 */
		void SetRectangle(RectangleF area);

		/**
		 * @brief 以(positionX, positionY)为核心，构建一个长度为(margin.left + margin.right)，宽度为(margin.top + margin.bottom)的矩形范围
		 * @param[in] positionX 核心的X坐标
		 * @param[in] positionY 核心的Y坐标
		 * @param[in] margin 描述矩形的大小
		 */
		void SetFocus(float positionX, float positionY, RectangleF margin);

		/**
		 * @brief 移动摄像机，移动的时候我们能够看到的范围并不会变
		 * @param[in] transform 移动偏移值
		 */
		void Move(glm::vec2 transform);

		/**
		 * @brief 移动摄像机，移动的时候我们能够看到的范围并不会变
		 * @param[in] translationX X位移
		 * @param[in] translationY Y位移
		 */
		void Move(float translationX, float translationY);

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
