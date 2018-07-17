#pragma once

#include "pch.hpp"
#include "Collider.hpp"
#include "Object.hpp"

namespace PixelWorldEngine {

	class PixelWorld;
	class PixelObject;
	
	/**
	 * @brief �����¼�
	 * @param[in] float �����ϴθ��¼����ʱ�䣬��λ��
	 */
	typedef std::function<void(float)> UpdateHandler;

	/**
	 * @brief ��ײ�¼�
	 * @param[in] �ƶ�������
	 * @param[in] �漰�����������
	 */
	typedef std::function<void(void*, PixelObject*)> CollideHandler;

	typedef std::vector<UpdateHandler> UpdateHandlers;
	typedef std::vector<CollideHandler> CollideHandlers;

	/**
	 * �����е�����
	 */
	class PixelObject : public Object {
	private:
		/**
		* @brief �ƶ����壬ֻ�������屻���ص�������ȥ�����Ч������ֻ��X�᷽�����λ�ƣ���ֻ��������͵�ͼ֮��Ĺ�ϵ
		* @param[in] translation ������X�����λ��
		* @return �����ƶ����X����
		*/
		auto MoveAxisXMap(float translation) -> float;

		/**
		* @brief �ƶ����壬ֻ�������ڱ����ص�������ȥ�����Ч������ֻ��Y�᷽�����λ�ƣ���ֻ��������͵�ͼ֮��Ĺ�ϵ
		* @param[in] translation ������Y�����λ��
		* @return �����ƶ����Y����
		*/
		auto MoveAxisYMap(float translation) -> float;
	private:
		Collider collider; //��ײ��

	protected:
		/**
		 * @brief �����¼�
		 * @param[in] deltaTime ������һ�θ��¾�����ʱ�䣬��λ��
		 */
		virtual void OnUpdate(float deltaTime);

		/**
		 * @brief �ƶ��¼���λ�������ᾭ��������ײ
		 * @param[in] translationX ��X���λ��
		 * @param[in] translationY ��Y���λ��
		 */
		virtual void OnMove(float translationX, float translationY);

		/**
		 * @brief ���������ƶ���ʱ���������������ʱ�򣬽��ᴥ��
		 * @oaram[in] sender ˭����������
		 * @param[in] piexlObject ��ײ��������
		 */
		virtual void OnCollide(void* sender, PixelObject* piexlObject);

		/**
		 * @brief ��������뵽���������ʱ������ֻ��������û�п���������ײ��ʱ�򴥷�
		 * @param[in] sender ˭����������
		 * @param[in] pixelObject �����������
		 */
		virtual void OnEnter(void* sender, PixelObject* pixelObject);

		/**
		 * @brief �������뿪֮ǰ����������ʱ�򴥷���ֻ��������û�п���������ײ��ʱ�򴥷�
		 * @param[in] sender ˭�뿪������
		 * @parma[in] pixelObject ���뿪������
		 */
		virtual void OnLeave(void* sender, PixelObject* pixelObject);

		friend class PixelWorld;
		friend class PixelObjectCompare;
	public:
		UpdateHandlers Update; //�����¼�
		CollideHandlers Enter; //�����������ʱ�򴥷�������μ�OnEnter
		CollideHandlers Leave; //���뿪�����ʱ�򴥷�������μ�OnLeave
		CollideHandlers Collide; //��������ײ������������ʱ�򴥷�������μ�OnCollide
	public:
		/**
		 * @brief ���캯��
		 * @param[in] Name ���������
		 * @param[in] PositionX ����(���ĵ�)��X���꣬Ĭ��Ϊ0
		 * @param[in] PositionY ����(���ĵ�)��Y���꣬Ĭ��Ϊ0
		 * @param[in] Width ����Ŀ�ȣ�Ĭ��Ϊ1
		 * @param[in] Height ����ĸ߶ȣ�Ĭ��Ϊ1
		 */
		PixelObject(std::string Name, float PositionX = 0.f, float PositionY = 0.f, float Width = 1.f, float Height = 1);

		/**
		 * @brief �ƶ����壬ֻ�������屻���ص�������ȥ�����Ч�������ǵ�λ��˳��������X�ᣬȻ����Y��
		 * @param[in] translationX ������X�����λ��
		 * @param[in] translationY ������Y�����λ��
		 */
		void Move(float translationX, float translationY);

		/**
		 * @brief ������Ȳ㣬��Ƚϴ�ĻḲ����Ƚ�С�ģ���������ͬ��ô���δ֪
		 * @param[in] depthLayer ��Ȳ㣬Ĭ��Ϊ0
		 */
		void SetDepthLayer(int depthLayer);

		/**
		* @brief ��������Ĵ�С
		* @param[in] objectWidth ����Ŀ�ȣ�Ĭ��Ϊ1
		* @param[in] objectHeight ����ĸ߶ȣ�Ĭ��Ϊ1
		*/
		virtual void SetSize(float objectWidth, float objectHeight)override;

		/**
		* @brief �����������꣬���ｫ���ῼ���κ��������أ���˿��ܵ���һЩ��������Ų�����
		* @param[in] x �����X���꣬Ĭ��Ϊ0
		* @param[in] y �����Y���꣬Ĭ��Ϊ0
		*/
		virtual void SetPosition(float x, float y)override;

		/**
		 * @brief �Ƿ�����������ײ��true��Ϊ������false�����ر�
		 * @param[in] enable �Ƿ�����Ĭ��Ϊtrue
		 */
		void EnablePhysicsCollision(bool enable);

		/**
		* @brief ��ȡ�������ײ�е�״̬�����Ƿ���
		* @return �Ż���ײ�е�״̬
		*/
		auto IsEnableCollider() -> bool;
	};

	class PixelObjectCompare {
	public:
		bool operator() (PixelObject* object1, PixelObject* object2)const {
			return object1->depthLayer < object2->depthLayer;
		}
	};
}