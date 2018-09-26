#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief �任���ͣ�����һ�������λ����ת����
	 */
	class Transform {
	private:
		glm::vec3 position; //λ��
		glm::vec3 scale; //����
		glm::vec3 forward; //����(0, 1)ΪĬ��ֵ

		float angle; //�Ƕȣ��������Ĭ�ϳ���ļн�

		glm::mat4x4 matrix; //�任����
	public:
		/**
		 * @brief ���캯��
		 */
		Transform();

		/**
		 * @brief ����λ��
		 * @param[in] position λ�ã�Ĭ��Ϊ (0, 0)
		 */
		void SetPosition(glm::vec2 position);

		/**
		 * @brief ��������
		 * @param[in] scale ���ţ�Ĭ��Ϊ (1, 1)
		 */
		void SetScale(glm::vec2 scale);

		/**
		 * @brief ���ó��򣬻�ı�Ƕ�
		 * @param[in] forward ����Ĭ��Ϊ (0, 1)
		 */
		void SetForward(glm::vec2 forward);

		/**
		 * @brief ���ýǶȣ���ı䳯��
		 * @param[in] angle �Ƕȣ�Ĭ��Ϊ0
		 */
		void SetRotate(float angle);

		/**
		 * @brief ��ȡλ��
		 * @return λ��
		 */
		auto GetPosition() -> glm::vec2;

		/**
		* @brief ��ȡ����
		* @return ����
		*/
		auto GetScale() -> glm::vec2;

		/**
		* @brief ��ȡ����
		* @return ����
		*/
		auto GetForward() -> glm::vec2;

		/**
		* @brief ��ȡ�Ƕ�
		* @return �Ƕ�
		*/
		auto GetRotate() -> float;

		/**
		* @brief ��ȡ�任����
		* @return ����
		*/
		auto GetMatrix() -> glm::mat4x4;

		/**
		 * @brief ͨ���任�������任����
		 * @param[in] transform �任
		 * @return �任����
		 */
		static auto CreateMatrixFromTransform(const Transform &transform) -> glm::mat4x4;

		friend auto operator + (const Transform &left, const Transform &right) -> Transform;
		friend auto operator - (const Transform &left, const Transform &right) -> Transform;
		friend auto operator * (const Transform &left, float value) -> Transform;
		friend auto operator / (const Transform &left, float value) -> Transform;
	};

}
