#pragma once

#include "pch.hpp"
#include "Collider.hpp"

namespace PixelWorldEngine {

	class PixelWorld;

	/**
	 * �����е�����
	 */
	class PixelObject {
	private:
		PixelWorld * pixelWorld; //���ڵ�����

	    /**
		* @brief �ƶ����壬ֻ�������屻���ص�������ȥ�����Ч������ֻ��X�᷽�����λ�ƣ���ֻ��������͵�ͼ֮��Ĺ�ϵ
	    * @param[in] translation ������X�����λ��
		*/
		auto MoveAxisXMap(float translation) -> float;

		/**
		* @brief �ƶ����壬ֻ�������ڱ����ص�������ȥ�����Ч������ֻ��Y�᷽�����λ�ƣ���ֻ��������͵�ͼ֮��Ĺ�ϵ
		* @param[in] translation ������Y�����λ��
		*/
		auto MoveAxisYMap(float translation) -> float;
	protected:
		std::string name; //���������

		float positionX; //�������ĵ��X���꣬Ĭ��Ϊ0
		float positionY; //�������ĵ��Y���꣬Ĭ��Ϊ0

		float width; //�����ȣ�Ĭ��Ϊ1
		float height; //����߶ȣ�Ĭ��Ϊ1

		float halfWidth; //�����ȵ�һ�룬Ĭ��Ϊ0.5f
		float halfHeight; //����߶ȵ�һ�룬Ĭ��Ϊ0.5f

		int renderObjectID; //��Ⱦ�����ID��Ĭ��Ϊ0��������Ⱦ

		Collider collider; //��ײ��

		friend class PixelWorld;
	public:
		/**
		 * @brief ���캯��
		 * @param[in] Name ���������
		 * @param[in] PositionX ����(���ĵ�)��X����
		 * @param[in] PositionY ����(���ĵ�)��Y����
		 * @param[in] Width ����Ŀ��
		 * @param[in] Height ����ĸ߶�
		 */
		PixelObject(std::string Name, float PositionX = 0.f, float PositionY = 0.f, float Width = 1.f, float Height = 1);

		/**
		 * @brief �ƶ����壬ֻ�������屻���ص�������ȥ�����Ч�������ǵ�λ��˳��������X�ᣬȻ����Y��
		 * @param[in] translationX ������X�����λ��
		 * @param[in] translationY ������Y�����λ��
		 */
		void Move(float translationX, float translationY);

		/**
		 * @brief ��������Ĵ�С
		 * @param[in] objectWidth ����Ŀ��
		 * @param[in] objectHeight ����ĸ߶�
		 */
		void SetSize(float objectWidth, float objectHeight);

		/**
		 * @brief ��������Ĵ�С
		 * @param[in] Size ����Ĵ�С
		 */
		void SetSize(float Size);

		/**
		 * @brief �����������꣬���ｫ���ῼ���κ��������أ���˿��ܵ���һЩ��������Ų�����
		 * @param[in] x �����X����
		 * @param[in] y �����Y����
		 */
		void SetPosition(float x, float y);

		/**
		 * @brief �����������ȾID
		 * @param[in] id �������ȾID
		 */
		void SetRenderObjectID(int id);

		/**
		 * @brief ������ײ��״̬��true��Ϊ������false�����ر�
		 * @param[in] enable ��ײ�е�״̬
		 */
		void EnableCollider(bool enable);

		/**
		 * @brief ��ȡ����Ŀ��
		 * @return ����Ŀ��
		 */
		auto GetWidth() -> float;

		/**
		 * @brief ��ȡ�������ײ�е�״̬�����Ƿ���
		 * @return �Ż���ײ�е�״̬
		 */
		auto IsEnableCollider() -> bool;

		/**
		 * @brief ��ȡ����ĸ߶�
		 * @return ����ĸ߶�
		 */
		auto GetHeight() -> float;

		/**
		 * @brief ��ȡ����(���ĵ�)��X����
		 * @return X����
		 */
		auto GetPositionX() -> float;

		/**
		 * @brief ��ȡ����(���ĵ�)��Y����
		 * @return Y����
		 */
		auto GetPositionY() -> float;

		/**
		 * @brief ��ȡ�������ȾID
		 * @return ��ȾID
		 */
		auto GetRenderObjectID() -> int;

		/**
		 * @brief ��ȡ���������
		 * @return ���������
		 */
		auto GetName() -> std::string;
	};

}