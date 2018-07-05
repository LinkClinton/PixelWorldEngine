#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	class PixelWorld;

	/**
	 * @brief UI���壬��Ҫ�����ڳ���UI�ȸ��ֺ���Ϸ�����޹صĶ���
	 */
	class UIObject {
	private:
		std::string name; //����

		float positionX; //���Ͻ�Xλ�ã�Ĭ��Ϊ0
		float positionY; //���Ͻ�Yλ�ã�Ĭ��Ϊ0

		float width; //��ȣ�Ĭ��Ϊ1
		float height; //�߶ȣ�Ĭ��Ϊ1

		float opacity; //��͸���ȣ�Ĭ��Ϊ1

		float borderWidth; //�߽��ȣ�ע�����ܿ�Ȳ���䣬��˳��ǿ��Ϊ0��������ʵ������Ⱦ��Χ�����ʵ��ΧС��Ĭ��Ϊ1
		float borderColor[4]; //�߽���ɫ��Ĭ��Ϊ (0, 0, 0, 1)

		int renderObjectID; //��ȾID��Ĭ��Ϊ0
		int depthLayer; //��Ȳ㣬Ĭ��Ϊ0

		std::map<std::string, UIObject*> children; //�����壬һ�������������������������
		
		friend class PixelWorld;
		friend class UIObjectCompare;
	public:
		/**
		 * @brief ���캯��
		 * @param[in] name ����
		 * @param[in] positionX ���Ͻ�X���꣬Ĭ��Ϊ0
		 * @param[in] positionY ���Ͻ�Y���꣬Ĭ��Ϊ0
		 * @param[in] width ��ȣ�Ĭ��Ϊ1
		 * @param[in] height �߶ȣ�Ĭ��Ϊ1
		 */
		UIObject(std::string name, float positionX = 0, float positionY = 0,
			float width = 1.0f, float height = 1.0f);

		/**
		 * @brief ���ñ߽���ɫ
		 * @param[in] red ��ɫ����
		 * @param[in] green ��ɫ����
		 * @param[in] blue ��ɫ����
		 * @param[in] alpha alpha����
		 */
		void SetBorderColor(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f);

		/**
		 * @brief ����λ��
		 * @param[in] positionX ���Ͻ�X���꣬Ĭ��Ϊ0
		 * @param[in] positionY ���Ͻ�Y���꣬Ĭ��Ϊ0
		 */
		void SetPosition(float positionX, float positionY);

		/**
		 * @brief ����X��λ��
		 * @param[in] positionX ���Ͻ�X���꣬Ĭ��Ϊ0
		 */
		void SetPositionX(float positionX);

		/**
		 * @brief ����Y��λ��
		 * @param[in] positionY ���Ͻ�Y���꣬Ĭ��Ϊ0
		 */
		void SetPositionY(float positionY);

		/**
		 * @brief ���ô�С
		 * @param[in] width ��ȣ�Ĭ��Ϊ1
		 * @param[in] height �߶ȣ�Ĭ��Ϊ1
		 */
		void SetSize(float width, float height);

		/**
		 * @brief ���ÿ��
		 * @param[in] width ��ȣ�Ĭ��Ϊ1
		 */
		void SetWidth(float width);

		/**
		 * @brief ���ø߶�
		 * @param[in] heiht �߶ȣ�Ĭ��Ϊ1
		 */
		void SetHeight(float height);

		/**
		 * @brief ���ò�͸����
		 * @param[in] opacity ��͸���ȣ�Ĭ��Ϊ1
		 */
		void SetOpacity(float opacity);

		/**
		 * @brief ���ñ߽���
		 * @param[in] width �߽��ȣ�Ĭ��Ϊ1
		 */
		void SetBorderWidth(float width);

		/**
		 * @brief ������ȾID
		 * @param[in] id ��ȾID��Ĭ��Ϊ0
		 */
		void SetRenderObjectID(int id);

		/**
		 * @brief ������Ȳ�
		 * @param[in] depthLayer ��Ȳ㣬Ĭ��Ϊ0
		 */
		void SetDepthLayer(int depthLayer);

		/**
		* @brief ע��UIObject��Ϊ�����
		* @param[in] object ����
		*/
		void RegisterUIObject(UIObject* object);

		/**
		* @brief ж��UIObject��Ϊ�����
		* @param[in] object ����
		*/
		void UnRegisterUIObject(UIObject* object);

		/**
		* @brief ж��UIObject��Ϊ�����
		* @param[in] name ���������
		*/
		void UnRegisterUIObject(std::string name);

		/**
		 * @brief ��ȡ�߽���ɫ�����ص���һ��ָ�룬����ֱ���޸�
		 * @return ͷԪ�ص�ַ������RGBA˳������
		 */
		auto GetBorderColor() -> float*;

		/**
		 * @brief ��ȡ���Ͻ�λ��X����
		 * @return ���Ͻ�λ��X����
		 */
		auto GetPositionX() -> float;

		/**
		 * @brief ��ȡ���Ͻ�λ��Y����
		 * @return ���Ͻ�λ��Y����
		 */
		auto GetPositionY() -> float;

		/**
		 * @brief ��ȡ���
		 * @return ���
		 */
		auto GetWidth() -> float;

		/**
		 * @brief ��ȡ�߶�
		 * @return �߶�
		 */
		auto GetHeight() -> float;

		/**
		 * @brief ��ȡ��͸����
		 * @return ��͸����
		 */
		auto GetOpacity() -> float;

		/**
		 * @brief ��ȡ�߽���
		 * @return �߽���
		 */
		auto GetBorderWidth() -> float;

		/**
		 * @brief ��ȡ��ȾID
		 * @return ��ȾID
		 */
		auto GetRenderObjectID() -> int;

		/**
		 * @brief ��ȡ��Ȳ�
		 * @return ��Ȳ�
		 */
		auto GetDepthLayer() -> int;
	};

	class UIObjectCompare {
	public:
		bool operator() (UIObject* object1, UIObject* object2)const {
			return object1->depthLayer < object2->depthLayer;
		}
	};
}