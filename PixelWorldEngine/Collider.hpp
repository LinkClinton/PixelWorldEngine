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
		
		bool isEnable; //是否启用
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
		 * @param[in] top 碰撞盒的上边界
		 * @param[in] right 碰撞盒的右边界
		 * @param[in] bottom 碰撞盒的下边界
		 */
		void SetArea(float left, float top, float right, float bottom);

		/**
		 * @brief 是否启用物理碰撞
		 * @param[in] enable 表示物理碰撞的启用状态，true表示启用，false则表示不启用
		 */
		void SetEnable(bool enable);

		/**
		 * @brief 获取碰撞盒范围
		 * @return 碰撞盒范围
		 */
		auto GetArea() -> RectangleF;
		
		/**
		 * @brief 获取物理碰撞的启用状态
		 * @return 物理碰撞的启用状态
		 */
		auto IsEnable() -> bool;

		/**
		 * @brief 检测是否和一个碰撞盒相撞
		 * @return 是否相撞
		 */
		auto Intersect(Collider collider) -> bool;

		/**
		 * @brief 对碰撞盒进行位移
		 * @param[in] collider 碰撞盒
		 * @param[in] x X位移
		 * @param[in[ y Y位移
		 * @return 结果碰撞盒
		 */
		static auto Translate(Collider collider, float x, float y) -> Collider;
	};

}