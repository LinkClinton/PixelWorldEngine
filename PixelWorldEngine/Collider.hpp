#pragma once
#pragma once

#include "pch.hpp"
#include "BaseStruct.hpp"

namespace PixelWorldEngine {

	/**
	* @brief 简单的2D碰撞盒
	*/
	class Collider {
	private:
		RectangleF rect; //描述盒子的范围
	public:
		/**
		* @brief 构造函数
		* @param[in] left 碰撞盒的左边界
		* @param[in] top 碰撞盒的上边界
		* @param[in] right 碰撞盒的右边界
		* @param[in] bottom 碰撞盒的下边界
		*/
		Collider(float left = 0, float top = 0, float right = 0, float bottom = 0);

		/**
		* @brief 构造函数
		* @param[in] rectangle 碰撞盒的范围
		*/
		Collider(RectangleF rectangle);

		/**
		* @brief 设置碰撞盒的范围
		* @param[in] left 碰撞盒的左边界
		* @param[in] left 碰撞盒的左边界
		* @param[in] left 碰撞盒的左边界
		* @param[in] left 碰撞盒的左边界
		*/
		void SetArea(float left, float top, float right, float bottom);

		/**
		* @brief 获取碰撞盒范围
		* @return 碰撞盒范围
		*/
		auto GetArea()->RectangleF;

		auto Intersect(glm::vec2 position, glm::mat4x4 transform) -> bool;

		auto Intersect(Collider other, glm::mat4x4 transform, glm::mat4x4 otherTransform) -> bool;
	};

}