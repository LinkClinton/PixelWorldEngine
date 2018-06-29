#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	class PixelWorld;

	/**
	 * �����е�����
	 */
	class PixelObject {
	private:
		PixelWorld * pixelWorld; //���ڵ�����
	protected:
		std::string name; //���������

		float positionX; //�������ĵ��X���꣬Ĭ��Ϊ0
		float positionY; //�������ĵ��Y���꣬Ĭ��Ϊ0

		float size; //�����С��Ĭ��Ϊ1

		int renderObjectID; //��Ⱦ�����ID��Ĭ��Ϊ0��������Ⱦ

		friend class PixelWorld;
	public:
		/**
		 * @brief ���캯��
		 * @param[in] Name ���������
		 * @param[in] PositionX ����(���ĵ�)��X����
		 * @param[in] PositionY ����(���ĵ�)��Y����
		 * @param[in] Size ����Ĵ�С
		 */
		PixelObject(std::string Name, float PositionX = 0.f, float PositionY = 0.f, float Size = 1.f);

		/**
		 * @brief �ƶ����壬ֻ�������屻���ص�������ȥ�����Ч
		 * @param[in] translationX ������X�����λ��
		 * @param[in] translationY ������Y�����λ��
		 */
		void Move(float translationX, float translationY);

		/**
		 * @brief ��������Ĵ�С
		 * @param[in] objectSize ����Ĵ�С
		 */
		void SetSize(float objectSize);

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
		 * @brief ��ȡ����Ĵ�С
		 * @return ����Ĵ�С
		 */
		auto GetSize() -> float;

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