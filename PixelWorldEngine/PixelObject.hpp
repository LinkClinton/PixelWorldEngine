#pragma once

#include "pch.hpp"
#include "Collider.hpp"

namespace PixelWorldEngine {

	class PixelWorld;

	/**
	 * 世界中的物体
	 */
	class PixelObject {
	private:
		PixelWorld * pixelWorld; //所在的世界

	    /**
		* @brief 移动物体，只有在物体被加载到世界中去后才有效，这里只对X轴方向进行位移，且只讨论物体和地图之间的关系
	    * @param[in] translation 物体在X方向的位移
		*/
		auto MoveAxisXMap(float translation) -> float;

		/**
		* @brief 移动物体，只有物体在被加载到世界中去后才有效，这里只对Y轴方向进行位移，且只讨论物体和地图之间的关系
		* @param[in] translation 物体在Y方向的位移
		*/
		auto MoveAxisYMap(float translation) -> float;
	protected:
		std::string name; //物体的名称

		float positionX; //物体中心点的X坐标，默认为0
		float positionY; //物体中心点的Y坐标，默认为0

		float width; //物体宽度，默认为1
		float height; //物体高度，默认为1

		float halfWidth; //物体宽度的一半，默认为0.5f
		float halfHeight; //物体高度的一半，默认为0.5f

		int renderObjectID; //渲染物体的ID，默认为0，即不渲染

		Collider collider; //碰撞盒

		friend class PixelWorld;
	public:
		/**
		 * @brief 构造函数
		 * @param[in] Name 物体的名字
		 * @param[in] PositionX 物体(中心点)的X坐标
		 * @param[in] PositionY 物体(中心点)的Y坐标
		 * @param[in] Width 物体的宽度
		 * @param[in] Height 物体的高度
		 */
		PixelObject(std::string Name, float PositionX = 0.f, float PositionY = 0.f, float Width = 1.f, float Height = 1);

		/**
		 * @brief 移动物体，只有在物体被加载到世界中去后才有效，且我们的位移顺序是优先X轴，然后再Y轴
		 * @param[in] translationX 物体在X方向的位移
		 * @param[in] translationY 物体在Y方向的位移
		 */
		void Move(float translationX, float translationY);

		/**
		 * @brief 设置物体的大小
		 * @param[in] objectWidth 物体的宽度
		 * @param[in] objectHeight 物体的高度
		 */
		void SetSize(float objectWidth, float objectHeight);

		/**
		 * @brief 设置物体的大小
		 * @param[in] Size 物体的大小
		 */
		void SetSize(float Size);

		/**
		 * @brief 设置物体坐标，这里将不会考虑任何其他因素，因此可能导致一些其他问题概不负责
		 * @param[in] x 物体的X坐标
		 * @param[in] y 物体的Y坐标
		 */
		void SetPosition(float x, float y);

		/**
		 * @brief 设置物体的渲染ID
		 * @param[in] id 物体的渲染ID
		 */
		void SetRenderObjectID(int id);

		/**
		 * @brief 设置碰撞盒状态，true则为开启，false则代表关闭
		 * @param[in] enable 碰撞盒的状态
		 */
		void EnableCollider(bool enable);

		/**
		 * @brief 获取物体的宽度
		 * @return 物体的宽度
		 */
		auto GetWidth() -> float;

		/**
		 * @brief 获取物体的碰撞盒的状态，即是否开启
		 * @return 放回碰撞盒的状态
		 */
		auto IsEnableCollider() -> bool;

		/**
		 * @brief 获取物体的高度
		 * @return 物体的高度
		 */
		auto GetHeight() -> float;

		/**
		 * @brief 获取物体(中心点)的X坐标
		 * @return X坐标
		 */
		auto GetPositionX() -> float;

		/**
		 * @brief 获取物体(中心点)的Y坐标
		 * @return Y坐标
		 */
		auto GetPositionY() -> float;

		/**
		 * @brief 获取物体的渲染ID
		 * @return 渲染ID
		 */
		auto GetRenderObjectID() -> int;

		/**
		 * @brief 获取物体的名字
		 * @return 物体的名字
		 */
		auto GetName() -> std::string;
	};

}