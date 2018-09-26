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

		/**
		 * @brief 检测两个碰撞盒在指定变换下是否相交
		 * @param[in] colliderA 碰撞盒A
		 * @param[in] colliderB 碰撞盒B
		 * @param[in] transformA 变换A
		 * @param[in] transformB 变换B
		 * @return 是否相交
		 */
		static auto SubIntersect(Collider colliderA, Collider colliderB, glm::mat4x4 transformA, glm::mat4x4 transformB) -> bool;
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

		/**
		 * @brief 检测碰撞盒在给定变换中是否覆盖指定点
		 * @param[in] position 指定点
		 * @param[in] transform 给定的变换
		 * @return 是否覆盖
		 */
		auto Intersect(glm::vec2 position, glm::mat4x4 transform) -> bool;

		/**
		 * @brief 检测两个碰撞盒是否相交，在给定变换的情况下
		 * @param[in] other 另外一个碰撞盒
		 * @parma[in] transform 碰撞盒的变换
		 * @param[in] otherTransform 另外一个碰撞盒的变换
		 * @return 是否相交
		 */
		auto Intersect(Collider other, glm::mat4x4 transform, glm::mat4x4 otherTransform) -> bool;
	};

}