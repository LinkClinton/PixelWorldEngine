#pragma once

#include "pch.hpp"
#include "BaseStruct.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief �򵥵�2D��ײ��
	 */
	class Collider {
	private:
		RectangleF rect; //�������ӵķ�Χ
		
		bool isEnable; //�Ƿ�����
	public:
		/**
		 * @brief ���캯��
		 * @param[in] left ��ײ�е���߽�
		 * @param[in] top ��ײ�е��ϱ߽�
		 * @param[in] right ��ײ�е��ұ߽�
		 * @param[in] bottom ��ײ�е��±߽�
		 */
		Collider(float left = 0, float top = 0, float right = 0, float bottom = 0);

		/**
		 * @brief ���캯��
		 * @param[in] rectangle ��ײ�еķ�Χ
		 */
		Collider(RectangleF rectangle);

		/**
		 * @brief ������ײ�еķ�Χ
		 * @param[in] left ��ײ�е���߽�
		 * @param[in] top ��ײ�е��ϱ߽�
		 * @param[in] right ��ײ�е��ұ߽�
		 * @param[in] bottom ��ײ�е��±߽�
		 */
		void SetArea(float left, float top, float right, float bottom);

		/**
		 * @brief �Ƿ�����������ײ
		 * @param[in] enable ��ʾ������ײ������״̬��true��ʾ���ã�false���ʾ������
		 */
		void SetEnable(bool enable);

		/**
		 * @brief ��ȡ��ײ�з�Χ
		 * @return ��ײ�з�Χ
		 */
		auto GetArea() -> RectangleF;
		
		/**
		 * @brief ��ȡ������ײ������״̬
		 * @return ������ײ������״̬
		 */
		auto IsEnable() -> bool;

		/**
		 * @brief ����Ƿ��һ����ײ����ײ
		 * @return �Ƿ���ײ
		 */
		auto Intersect(Collider collider) -> bool;

		/**
		 * @brief ����ײ�н���λ��
		 * @param[in] collider ��ײ��
		 * @param[in] x Xλ��
		 * @param[in[ y Yλ��
		 * @return �����ײ��
		 */
		static auto Translate(Collider collider, float x, float y) -> Collider;
	};

}