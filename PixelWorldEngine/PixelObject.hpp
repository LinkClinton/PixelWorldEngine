#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	class PixelWorld;

	/**
	 * 世界中的物体
	 */
	class PixelObject {
	private:
		PixelWorld * pixelWorld; //所在的世界
	protected:
		std::string name; //物体的名称

		float positionX; //物体中心点的X坐标，默认为0
		float positionY; //物体中心点的Y坐标，默认为0

		float size; //物体大小，默认为1

		int renderObjectID; //渲染物体的ID，默认为0，即不渲染

		friend class PixelWorld;
	public:
		/**
		 * @brief 构造函数
		 * @param[in] Name 物体的名字
		 * @param[in] PositionX 物体(中心点)的X坐标
		 * @param[in] PositionY 物体(中心点)的Y坐标
		 * @param[in] Size 物体的大小
		 */
		PixelObject(std::string Name, float PositionX = 0.f, float PositionY = 0.f, float Size = 1.f);

		/**
		 * @brief 移动物体，只有在物体被加载到世界中去后才有效
		 * @param[in] translationX 物体在X方向的位移
		 * @param[in] translationY 物体在Y方向的位移
		 */
		void Move(float translationX, float translationY);

		/**
		 * @brief 设置物体的大小
		 * @param[in] objectSize 物体的大小
		 */
		void SetSize(float objectSize);

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
		 * @brief 获取物体的大小
		 * @return 物体的大小
		 */
		auto GetSize() -> float;

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