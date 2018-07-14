#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	class PixelWorld;

	class Object {
	protected:
		PixelWorld* pixelWorld; //���ڵ�����

		std::string name; //����

		float positionX; //���Ͻ�Xλ�ã�Ĭ��Ϊ0
		float positionY; //���Ͻ�Yλ�ã�Ĭ��Ϊ0

		float width; //��ȣ�Ĭ��Ϊ1
		float height; //�߶ȣ�Ĭ��Ϊ1

		float opacity; //��͸����
		float effectColor[3]; //��Ч��ɫ

		int renderObjectID; //��Ⱦ�����ID��Ĭ��Ϊ0��������Ⱦ
		int depthLayer; //��������ڵ���Ȳ㣬Ĭ��Ϊ0����Ƚϴ�ĻḲ����Ƚ�С��

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
		Object(std::string Name, float PositionX = 0.f, float PositionY = 0.f, float Width = 1.f, float Height = 1);

		/**
		* @brief ��������Ĵ�С
		* @param[in] objectWidth ����Ŀ��
		* @param[in] objectHeight ����ĸ߶�
		*/
		virtual void SetSize(float objectWidth, float objectHeight);

		/**
		* @brief �����������꣬���ｫ���ῼ���κ��������أ���˿��ܵ���һЩ��������Ų�����
		* @param[in] x �����X����
		* @param[in] y �����Y����
		*/
		virtual void SetPosition(float x, float y);

		/**
		* @brief �����������ȾID
		* @param[in] id �������ȾID
		*/
		virtual void SetRenderObjectID(int id);

		/**
		* @brief ���ò�͸����
		* @param[in] opacity ��͸����
		*/
		virtual void SetOpacity(float opacity);

		/**
		* @brief ������Ч��ɫ����������������������ɫ�������Ч��ɫ���г˷��ĵ�������ɫ
		* @param[in] red ��ɫ����
		* @param[in] green ��ɫ����
		* @param[in] blue ��ɫ����
		*/
		virtual void SetEffectColor(float red = 1.0f, float green = 1.0f, float blue = 1.0f);

		/**
		* @brief ��ȡ����Ŀ��
		* @return ����Ŀ��
		*/
		virtual auto GetWidth() -> float;

		/**
		* @brief ��ȡ����ĸ߶�
		* @return ����ĸ߶�
		*/
		virtual auto GetHeight() -> float;

		/**
		* @brief ��ȡ����(���ĵ�)��X����
		* @return X����
		*/
		virtual auto GetPositionX() -> float;

		/**
		* @brief ��ȡ����(���ĵ�)��Y����
		* @return Y����
		*/
		virtual auto GetPositionY() -> float;

		/**
		* @brief ��ȡ�������ȾID
		* @return ��ȾID
		*/
		virtual auto GetRenderObjectID() -> int;

		/**
		* @brief ��ȡ��Ȳ�
		* @return ��Ȳ�
		*/
		virtual auto GetDepthLayer() -> int;

		/**
		* @brief ��ȡ��͸����
		* @return ��͸����
		*/
		virtual auto GetOpacity() -> float;

		/**
		* @brief ��ȡ��Ч��ɫ
		* @return ��ɫ
		*/
		virtual auto GetEffectColor() -> float*;

		/**
		* @brief ��ȡ���������
		* @return ���������
		*/
		virtual auto GetName()->std::string;
	};

}