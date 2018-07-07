#pragma once

#include "pch.hpp"
#include "Events.hpp"

namespace PixelWorldEngine {

	class PixelWorld;
	class UIObject;

	class UIObjectCompare {
	public:
		bool operator() (UIObject* object1, UIObject* object2)const;
	};

	/**
	 * @brief UI���壬��Ҫ�����ڳ���UI�ȸ��ֺ���Ϸ�����޹صĶ���
	 */
	class UIObject {
	private:
		PixelWorld* pixelWorld; //����
		UIObject* parent; //����

		std::string name; //����

		float positionX; //���Ͻ�Xλ�ã�Ĭ��Ϊ0
		float positionY; //���Ͻ�Yλ�ã�Ĭ��Ϊ0

		float width; //��ȣ�Ĭ��Ϊ1
		float height; //�߶ȣ�Ĭ��Ϊ1

		float halfWidth; //��ȵ�һ��
		float halfHeight; //�߶ȵ�һ��

		float opacity; //��͸���ȣ�Ĭ��Ϊ1

		float angle; //�Ƕȣ���ת����Ϊ�������ģ�Ĭ��Ϊ0

		float borderWidth; //�߽��ȣ�ע�����ܿ�Ȳ���䣬��˳��ǿ��Ϊ0��������ʵ������Ⱦ��Χ�����ʵ��ΧС��Ĭ��Ϊ0
		float borderColor[3]; //�߽���ɫ��Ĭ��Ϊ (0, 0, 0)

		int renderObjectID; //��ȾID��Ĭ��Ϊ0
		int depthLayer; //��Ȳ㣬Ĭ��Ϊ0

		std::map<std::string, UIObject*> children; //�����壬һ�������������������������
		std::set<UIObject*, UIObjectCompare> childrenLayer; //�������

		glm::mat4x4 transformMatrix; //�任����
		glm::mat4x4 invTransformMatrix; //��任����

		friend class PixelWorld;
		friend class UIObjectCompare;
	private:
		/**
		 * @brief ����һ���任����
		 * @param[in] object ����
		 * @return ����
		 */
		static auto CreateTransformMatrix(UIObject* object) -> glm::mat4x4;

		static auto CreateTransformInvMatrix(UIObject* object) -> glm::mat4x4;

		/**
		 * @brief ��һ����������ĸ��׵Ķ������Ƴ�
		 * @param[in] object ����
		 */
		static void UnRegisterFromParent(UIObject* object);

		/**
		 * @brief ��һ�������PixelWolrd���Ƴ�
		 * @param[in] object ����
		 */
		static void UnRegisterFromPixelWorld(UIObject* object);

		/**
		 * @brief ���������յ��Ĺ�������ƶ�����Ϣ���������Ƿ�Ҫ����������Ϣ�������Լ�������
		 * @param[in] object ����
		 * @param[in] eventArg ��Ϣ
		 * @param[in] baseTransformMatrix ���ȵı任����
		 */
		static void ProcessMouseMoveEvent(UIObject* object, Events::MouseMoveEvent* eventArg, glm::mat4x4 baseTransformMatrix);

		/**
		* @brief ���������յ��Ĺ�����갴�µ���Ϣ���������Ƿ�Ҫ����������Ϣ�������Լ�������
		* @param[in] object ����
		* @param[in[ eventArg ��Ϣ
		* @param[in] baseTransformMatrix ���ȵı任����
		*/
		static void ProcessMouseClickEvent(UIObject* object, Events::MouseClickEvent* eventArg, glm::mat4x4 baseTransformMatrix);

		/**
		* @brief ���������յ��Ĺ�����껬�ֹ�������Ϣ���������Ƿ�Ҫ����������Ϣ�������Լ�������
		* @param[in] object ����
		* @param[in[ eventArg ��Ϣ
		* @param[in] baseTransformMatrix ���ȵı任����
		*/
		static void ProcessMouseWheelEvent(UIObject* object, Events::MouseWheelEvent* eventArg, glm::mat4x4 baseTransformMatrix);

		/**
		* @brief ���������յ��Ĺ��ڼ��̰��µ���Ϣ���������Ƿ�Ҫ����������Ϣ�������Լ�������
		* @param[in] object ����
		* @param[in[ eventArg ��Ϣ
		*/
		static void ProcessKeyClickEvent(UIObject* object, Events::KeyClickEvent* eventArg);
	protected:
		/**
		 * @brief ������ƶ���ʱ�򴥷���ע�������������巶Χ��
		 * @param[in] sender ˭��������Ϣ
		 * @param[in] eventArg ��Ϣ��Ϣ
		 */
		virtual void OnMouseMove(void* sender, Events::MouseMoveEvent* eventArg);

		/**
		 * @brief ����갴�µ�ʱ�򴥷���ע�������������巶Χ��
		 * @param[in] sender ˭��������Ϣ
		 * @param[in] eventArg ��Ϣ��Ϣ
		 */
		virtual void OnMouseClick(void* sender, Events::MouseClickEvent* eventArg);

		/** 
		 * @brief ����껬�ֹ�����ʱ�򴥷���ע�������������巶Χ��
		 * @param[in] sender ˭��������Ϣ
		 * @param[in] eventArg ��Ϣ��Ϣ
		 */
		virtual void OnMouseWheel(void* sender, Events::MouseWheelEvent* eventArg);

		/**
		 * @brief �����̰�������ȥ��ʱ�򴥷���ע������ȡ�˽���
		 * @param[in] sender ˭��������Ϣ
		 * @param[in] eventArg ��Ϣ��Ϣ
		 */
		virtual void OnKeyClick(void* sender, Events::KeyClickEvent* eventArg);
	public:
		PixelWorldEngine::Events::MouseMoveHandlers MouseMove; //����ƶ��¼�
		PixelWorldEngine::Events::MouseClickHandlers MouseClick; //��갴���¼�
		PixelWorldEngine::Events::MouseWheelHandlers MouseWheel; //�������¼�
		PixelWorldEngine::Events::KeyClickEventHandlers KeyClick; //���̰����¼�
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
		void SetBorderColor(float red = 0.0f, float green = 0.0f, float blue = 0.0f);

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
		 * @brief ������ת�Ƕȣ���ת����Ϊ�������ģ�Ĭ��Ϊ0
		 * @param[in] angle �Ƕ�
		 */
		void SetAngle(float angle);

		/**
		 * @brief ���ñ߽���
		 * @param[in] width �߽��ȣ�Ĭ��Ϊ0
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
		 * @brief ��ȡ��ת�Ƕ�
		 * @return ��ת�Ƕ�
		 */
		auto GetAngle() -> float;

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

		/**
		 * @brief ����������Ƿ��������ڲ���ע�����ﲻ���Ǹ��׽ڵ��Ӱ���Լ���ת��Ӱ��
		 * @param[in] object ����
		 * @param[in] x ���X����
		 * @param[in[ y ���Y����
		 */
		static auto IsInUIObjectRect(UIObject* object, float x, float y) -> bool;
	};

}