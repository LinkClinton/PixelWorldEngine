#pragma once

#include "pch.hpp"
#include "Collider.hpp"
#include "Object.hpp"

namespace PixelWorldEngine {

	class PixelWorld;
	class PixelObject;
	
	/**
	 * @brief 更新事件
	 * @param[in] float 距离上次更新间隔的时间，单位秒
	 */
	typedef std::function<void(float)> UpdateHandler;

	/**
	 * @brief 碰撞事件
	 * @param[in] 移动的物体
	 * @param[in] 涉及的另外的物体
	 */
	typedef std::function<void(void*, PixelObject*)> CollideHandler;

	typedef std::vector<UpdateHandler> UpdateHandlers;
	typedef std::vector<CollideHandler> CollideHandlers;

	/**
	 * 世界中的物体
	 */
	class PixelObject : public Object {
	private:
		/**
		* @brief 移动物体，只有在物体被加载到世界中去后才有效，这里只对X轴方向进行位移，且只讨论物体和地图之间的关系
		* @param[in] translation 物体在X方向的位移
		* @return 物体移动后的X坐标
		*/
		auto MoveAxisXMap(float translation) -> float;

		/**
		* @brief 移动物体，只有物体在被加载到世界中去后才有效，这里只对Y轴方向进行位移，且只讨论物体和地图之间的关系
		* @param[in] translation 物体在Y方向的位移
		* @return 物体移动后的Y坐标
		*/
		auto MoveAxisYMap(float translation) -> float;
	private:
		Collider collider; //碰撞盒

	protected:
		/**
		 * @brief 更新事件
		 * @param[in] deltaTime 距离上一次更新经过的时间，单位秒
		 */
		virtual void OnUpdate(float deltaTime);

		/**
		 * @brief 移动事件，位移量将会经过考虑碰撞
		 * @param[in] translationX 在X轴的位移
		 * @param[in] translationY 在Y轴的位移
		 */
		virtual void OnMove(float translationX, float translationY);

		/**
		 * @brief 当物体在移动的时候碰到其他物体的时候，将会触发
		 * @oaram[in] sender 谁碰到了物体
		 * @param[in] piexlObject 碰撞到的物体
		 */
		virtual void OnCollide(void* sender, PixelObject* piexlObject);

		/**
		 * @brief 当物体进入到另外的物体时触发，只有在物体没有开启物理碰撞的时候触发
		 * @param[in] sender 谁进入了物体
		 * @param[in] pixelObject 被进入的物体
		 */
		virtual void OnEnter(void* sender, PixelObject* pixelObject);

		/**
		 * @brief 当物体离开之前进入的物体的时候触发，只有在物体没有开启物理碰撞的时候触发
		 * @param[in] sender 谁离开了物体
		 * @parma[in] pixelObject 被离开的物体
		 */
		virtual void OnLeave(void* sender, PixelObject* pixelObject);

		friend class PixelWorld;
		friend class PixelObjectCompare;
	public:
		UpdateHandlers Update; //更新事件
		CollideHandlers Enter; //当进入物体的时候触发，具体参见OnEnter
		CollideHandlers Leave; //当离开物体的时候触发，具体参见OnLeave
		CollideHandlers Collide; //当物体碰撞到另外的物体的时候触发，具体参见OnCollide
	public:
		/**
		 * @brief 构造函数
		 * @param[in] Name 物体的名字
		 * @param[in] PositionX 物体(中心点)的X坐标，默认为0
		 * @param[in] PositionY 物体(中心点)的Y坐标，默认为0
		 * @param[in] Width 物体的宽度，默认为1
		 * @param[in] Height 物体的高度，默认为1
		 */
		PixelObject(std::string Name, float PositionX = 0.f, float PositionY = 0.f, float Width = 1.f, float Height = 1);

		/**
		 * @brief 移动物体，只有在物体被加载到世界中去后才有效，且我们的位移顺序是优先X轴，然后再Y轴
		 * @param[in] translationX 物体在X方向的位移
		 * @param[in] translationY 物体在Y方向的位移
		 */
		void Move(float translationX, float translationY);

		/**
		 * @brief 设置深度层，深度较大的会覆盖深度较小的，如果深度相同那么结果未知
		 * @param[in] depthLayer 深度层，默认为0
		 */
		void SetDepthLayer(int depthLayer);

		/**
		* @brief 设置物体的大小
		* @param[in] objectWidth 物体的宽度，默认为1
		* @param[in] objectHeight 物体的高度，默认为1
		*/
		virtual void SetSize(float objectWidth, float objectHeight)override;

		/**
		* @brief 设置物体坐标，这里将不会考虑任何其他因素，因此可能导致一些其他问题概不负责
		* @param[in] x 物体的X坐标，默认为0
		* @param[in] y 物体的Y坐标，默认为0
		*/
		virtual void SetPosition(float x, float y)override;

		/**
		 * @brief 是否允许物理碰撞，true则为开启，false则代表关闭
		 * @param[in] enable 是否允许，默认为true
		 */
		void EnablePhysicsCollision(bool enable);

		/**
		* @brief 获取物体的碰撞盒的状态，即是否开启
		* @return 放回碰撞盒的状态
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