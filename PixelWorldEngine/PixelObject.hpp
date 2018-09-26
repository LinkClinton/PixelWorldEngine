#pragma once

#include "pch.hpp"

#include "Events.hpp"

#include "Transform.hpp"
#include "Collider.hpp"
#include "BaseStruct.hpp"

#include "GraphicsFont.hpp"
#include "Text.hpp"

namespace PixelWorldEngine {

	class PixelObject;

	namespace Internal {

		/**
		 * @brief ���ڱȽ���������ֵ�ıȽϺ���
		 */
		class PixelObjectCompare {
		public:
			bool operator () (PixelObject* object1, PixelObject* object2)const;
		};

		/**
		 * @brief ������¼�������
		 */
		class PixelObjectProcess {
		public:
			static void ProcessUpdate(PixelObject* object);

			static void ProcessAfterUpdate(PixelObject* object);

			static void ProcessMouseEnter(PixelObject* object);

			static void ProcessMouseLeave(PixelObject* object);

			static void ProcessMouseMove(PixelObject* object, Events::MouseMoveEvent* eventArg, glm::mat4x4 baseTransformMatrix);

			static void ProcessMouseClick(PixelObject* object, Events::MouseClickEvent* eventArg, glm::mat4x4 baseTransformMatrix);

			static void ProcessMouseWheel(PixelObject* object, Events::MouseWheelEvent* eventArg, glm::mat4x4 baseTransformMatrix);

			static void ProcessKeyClick(PixelObject* object, Events::KeyClickEvent* eventArg);

			static void ProcessObjectCollide(PixelObject* objectA, PixelObject* objectB);

			static void ProcessObjectEnter(PixelObject* objectA, PixelObject* objectB);
			
			static void ProcessObjectLeave(PixelObject* objectA, PixelObject* objectB);
		};

		class CollideSolver;

	}

	namespace Events {

		typedef std::function<void(PixelObject*, PixelObject*)> CollideEventHandler;
		typedef std::vector<CollideEventHandler> CollideEventHandlers;

	}

	/**
	 * @brief ���壬����ĺ����࣬���ɸ���ʵ����ֻ��ʹ��new����
	 */
	class PixelObject {
	private:
		std::string name; //���������
		
		PixelObject* parent; //����ĸ���

		float width; //����Ŀ��
		float height; //����ĸ߶�

		int depth; //�������ȣ����ھ�������ͬ���������ʾ���Ⱥ�˳��
	
		bool isHover; //����Ƿ�����������
		bool isSizeChange; //��С�Ƿ�ı�

		Collider collider; //��ײ��

		Transform oldTransform; //�ɵı任��������һ�δ���任֮ǰ�ı任

		std::map<std::string, PixelObject*> childrenNameIndex; //��������
		std::multiset<PixelObject*, Internal::PixelObjectCompare> childrenDepthSort; //�������壬�����������

		/**/
		PixelWorldEngine::Text* textInstance; //��������
		int objectID; //�����ţ�ͬʱҲ�����ֵ���Ⱦ���

		static int ObjectCount; //��¼�������
		static std::vector<int> FreeObjectID; //���е�ID

		/**
		 * @brief ��������ı任�����ɱ任�ı���±任
		 * @param[in] object ���壬��������������
		 */
		static void UpdateTransform(PixelObject* object); 

		/**
		 * @brief ����������ı�����
		 * @param[in] object ����
		 */
		static void UpdateText(PixelObject* object);

		/**
		 * @brief ��ȡ����ı�ţ�����п����ţ���ôʹ�ÿ����ţ������������
		 * @return ����ı��
		 */
		static auto GetObjectID() -> int;

		friend class PixelWorld;
		friend class Internal::PixelObjectProcess;
		friend class Internal::PixelObjectCompare;
		friend class Internal::CollideSolver;
	protected:
		/**
		 * @brief �����¼�
		 * @param[in] sender ˭������
		 */
		virtual void OnUpdate(void* sender) {}

		/**
		 * @brief ���½����󴥷�
		 * @param[in] sender ˭������
		 */
		virtual void OnAfterUpdate(void* sender) {}

		/**
		 * @brief �������¼��������������巶Χ��ʱ�򴥷�
		 * @param[in] sender ˭������
		 */
		virtual void OnMouseEnter(void* sender) {}

		/**
		 * @brief ����뿪�¼���������뿪���巶Χ��ʱ�򴥷�
		 * @param[in] sender ˭������
		 */
		virtual void OnMouseLeave(void* sender) {}

		/**
		 * @brief ����ƶ��¼���������ƶ���ʱ�򴥷���Ҫ����������巶Χ��
		 * @param[in] sender ˭������
		 * @parma[in] eventArg �¼���Ϣ
		 */
		virtual void OnMouseMove(void* sender, Events::MouseMoveEvent* eventArg) {}

		/**
		 * @brief ������¼������������ʱ�򴥷���Ҫ����������巶Χ��
		 * @param[in] sender ˭������
		 * @param[in] eventArg �¼���Ϣ
		 */
		virtual void OnMouseClick(void* sender, Events::MouseClickEvent* eventArg) {}

		/**
		* @brief �������¼�������������ʱ�򴥷���Ҫ����������巶Χ��
		* @param[in] sender ˭������
		* @param[in] eventArg �¼���Ϣ
		*/
		virtual void OnMouseWheel(void* sender, Events::MouseWheelEvent* eventArg) {}

		/**
		 * @brief ���̰����¼��������̰��»��ߵ����ʱ�򴥷���Ҫ�����������̰����¼�
		 * @param[in] sender ˭������
		 * @param[in] eventArg �¼���Ϣ
		 */
		virtual void OnKeyClick(void* sender, Events::KeyClickEvent* eventArg) {}

		/**
		 * @brief ������ײ�¼���ֻ�����忪����������ײ�Լ���������������Żᴥ��
		 * @param[in] objectA ��ײ����A
		 * @param[in] objectB ��ײ����B
		 */
		virtual void OnObjectCollide(PixelObject* objectA, PixelObject* objectB) {}

		/**
		 * @brief ��������¼���ֻ������ر���������ײ�Լ���������������Żᴥ��
		 * @param[in] objectA �ཻ����A
		 * @param[in] objectB �ཻ����B
		 */
		virtual void OnObjectEnter(PixelObject* objectA, PixelObject* objectB) {}

		/**
		* @brief �����뿪�¼���ֻ������ر���������ײ�Լ���������������Żᴥ��
		* @param[in] objectA �뿪����A
		* @param[in] objectB �뿪����B
		*/
		virtual void OnObjectLeave(PixelObject* objectA, PixelObject* objectB) {}
	public:
		Events::UpdateEventHandlers Update; //�����¼�����
		Events::UpdateEventHandlers AfterUpdate; //������ɺ��¼�
		Events::MouseEnterEventHandlers MouseEnter; //�������¼�����
		Events::MouseLeaveEventHandlers MouseLeave; //����뿪�¼�����
		Events::MouseMoveHandlers MouseMove; //����ƶ��¼�����
		Events::MouseClickHandlers MouseClick; //��갴���¼�����
		Events::MouseWheelHandlers MouseWheel; //�������¼�����
		Events::KeyClickEventHandlers KeyClick; //���̰����¼�����
		Events::CollideEventHandlers ObjectCollide; //������ײ�¼�����
		Events::CollideEventHandlers ObjectEnter; //�����ཻ�¼�����
		Events::CollideEventHandlers ObjectLeave; //�����뿪�¼�����

		Transform Transform; //����ı任������λ����ת����

		int RenderObjectID; //�������Ⱦ��ţ�Ĭ��Ϊ0

		float Opacity; //����Ĳ�͸���ȣ�Ĭ��Ϊ1

		glm::vec3 TextColor; //������ɫ��Ĭ��Ϊ (0, 0, 0)

		std::string Text; //�ı����ݣ�����Ϊ""��������ʾ����

		Graphics::Font* Font; //���壬����Ϊnullptr�Ļ���������ʾ����

		bool IsEnableVisual; //�Ƿ�������ʾ��Ĭ��Ϊtrue
		bool IsEnableRead; //�Ƿ���������¼���Ĭ��Ϊfalse
		bool IsEnablePhysicsCollide; //�Ƿ�����������ײ��Ĭ��Ϊfalse
	public:
		/**
		 * @brief ���캯��
		 * @param[in] name ���������
		 * @param[in] transform ����ı任
		 */
		PixelObject(std::string name, PixelWorldEngine::Transform transform = PixelWorldEngine::Transform());

		/**
		 * @brief �������죬�����ֽ�����PixelObject + ��������
		 * @param[in] transform ����ı任
		 */
		PixelObject(PixelWorldEngine::Transform transform = PixelWorldEngine::Transform());

		/**
		 * @brief Ĭ����������
		 */
		~PixelObject();

		/**
		 * @brief ������������
		 * @param[in] depth ��������
		 */
		virtual void SetDepth(int depth);

		/**
		 * @brief ��������Ĵ�С
		 * @param[in] width ����Ŀ��
		 * @param[in] height ����ĸ߶�
		 */
		virtual void SetSize(float width, float height);

		/**
		 * @brief ��������ĸ���
		 * @param[in] parent ��������
		 */
		virtual void SetParent(PixelObject* parent);

		/**
		 * @brief ��������������壬�������ö��
		 * @param[in] child ������
		 */
		virtual void SetChild(PixelObject* child);

		/**
		 * @brief �Ƴ�������
		 * @param[in] name �����������
		 */
		virtual void CancelChild(std::string name);

		/**
		 * @brief ��ȡ���������
		 * @return ���������
		 */
		virtual auto GetName() -> std::string;

		/**
		 * @brief ��ȡ��������
		 * @param[in] ���
		 */
		virtual auto GetDepth() -> int;

		/**
		 * @brief ��ȡ����Ĵ�С
		 * @param[in] ����Ĵ�С
		 */
		virtual auto GetSize() -> SizeF;
		
		/**
		 * @brief ��ȡ����ĸ��ף����û�оͷ���nullptr
		 * @return ����ĸ���
		 */
		virtual auto GetParent() -> PixelObject*;

		/**
		 * @brief ��ȡ����������壬���û�оͷ���nullptr
		 * @return �����������
		 */
		virtual auto GetChildren(std::string name) -> PixelObject*;

		/**
		 * @brief ����һ�����崴��һ���µ�����
		 * @param[in] name �����������
		 * @param[in] object ������
		 * @return ������ĵ�ַ
		 */
		static auto CreateFromInstance(std::string name, PixelObject* object) -> PixelObject*;
	};

}