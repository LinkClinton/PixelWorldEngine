#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief 变换类型，描述一个物体的位移旋转缩放
	 */
	class Transform {
	private:
		glm::vec3 position; //位置
		glm::vec3 scale; //缩放
		glm::vec3 forward; //朝向，(0, 1)为默认值

		float angle; //角度，即朝向和默认朝向的夹角

		glm::mat4x4 matrix; //变换矩阵
	public:
		/**
		 * @brief 构造函数
		 */
		Transform();

		/**
		 * @brief 设置位置
		 * @param[in] position 位置，默认为 (0, 0)
		 */
		void SetPosition(glm::vec2 position);

		/**
		 * @brief 设置缩放
		 * @param[in] scale 缩放，默认为 (1, 1)
		 */
		void SetScale(glm::vec2 scale);

		/**
		 * @brief 设置朝向，会改变角度
		 * @param[in] forward 朝向，默认为 (0, 1)
		 */
		void SetForward(glm::vec2 forward);

		/**
		 * @brief 设置角度，会改变朝向
		 * @param[in] angle 角度，默认为0
		 */
		void SetRotate(float angle);

		/**
		 * @brief 获取位置
		 * @return 位置
		 */
		auto GetPosition() -> glm::vec2;

		/**
		* @brief 获取缩放
		* @return 缩放
		*/
		auto GetScale() -> glm::vec2;

		/**
		* @brief 获取朝向
		* @return 朝向
		*/
		auto GetForward() -> glm::vec2;

		/**
		* @brief 获取角度
		* @return 角度
		*/
		auto GetRotate() -> float;

		/**
		* @brief 获取变换矩阵
		* @return 矩阵
		*/
		auto GetMatrix() -> glm::mat4x4;

		/**
		 * @brief 通过变换来构建变换矩阵
		 * @param[in] transform 变换
		 * @return 变换矩阵
		 */
		static auto CreateMatrixFromTransform(const Transform &transform) -> glm::mat4x4;

		friend auto operator + (const Transform &left, const Transform &right) -> Transform;
		friend auto operator - (const Transform &left, const Transform &right) -> Transform;
		friend auto operator * (const Transform &left, float value) -> Transform;
		friend auto operator / (const Transform &left, float value) -> Transform;
	};

}
