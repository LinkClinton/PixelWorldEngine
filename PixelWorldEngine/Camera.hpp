#pragma once

#include "pch.hpp"

#include "BaseStruct.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PixelWorldEngine {

	/**
	 * @brief ��������������ǿ����ķ�Χ�Լ�����
	 */
	class Camera {
	private:
		RectangleF rect; //������ܹ�����������

		glm::mat4x4 project; //ͶӰ����ע�����ǵ�ͶӰ����
	public:
		/**
		 * @brief Ĭ�Ϲ��캯��
		 */
		Camera() = default;

		/**
		 * @brief ���캯��
		 * @param[in] rectangle ��ʾ������ܹ������ķ�Χ
		 */
		Camera(RectangleF area);

		/**
		 * @brief ����������ܹ������ķ�Χ
		 * @param[in] area ��ʾ��Χ
		 */
		void SetRectangle(RectangleF area);

		/**
		 * @brief ��ȡ������ܹ������ķ�Χ
		 * @return һ���������ͣ�������Χ
		 */
		auto GetRectangle() -> RectangleF;

		/**
		 * @brief ��ȡ�������ӦͶӰ����
		 * @return һ��4x4�ľ���
		 */
		auto GetMatrix() -> glm::mat4x4;
	};

}
