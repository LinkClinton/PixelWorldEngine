#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	class PixelWorld;

	class Object {
	protected:
		PixelWorld* pixelWorld; //所在的世界

		std::string name; //名字

		float positionX; //左上角X位置，默认为0
		float positionY; //左上角Y位置，默认为0

		float width; //宽度，默认为1
		float height; //高度，默认为1

		float opacity; //不透明度
		float effectColor[3]; //特效颜色

		int renderObjectID; //渲染物体的ID，默认为0，即不渲染
		int depthLayer; //物体的所在的深度层，默认为0，深度较大的会覆盖深度较小的

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
		Object(std::string Name, float PositionX = 0.f, float PositionY = 0.f, float Width = 1.f, float Height = 1);

		/**
		* @brief 设置物体的大小
		* @param[in] objectWidth 物体的宽度
		* @param[in] objectHeight 物体的高度
		*/
		virtual void SetSize(float objectWidth, float objectHeight);

		/**
		* @brief 设置物体坐标，这里将不会考虑任何其他因素，因此可能导致一些其他问题概不负责
		* @param[in] x 物体的X坐标
		* @param[in] y 物体的Y坐标
		*/
		virtual void SetPosition(float x, float y);

		/**
		* @brief 设置物体的渲染ID
		* @param[in] id 物体的渲染ID
		*/
		virtual void SetRenderObjectID(int id);

		/**
		* @brief 设置不透明度
		* @param[in] opacity 不透明度
		*/
		virtual void SetOpacity(float opacity);

		/**
		* @brief 设置特效颜色，即我们最后纹理输出的颜色将会和特效颜色进行乘法的到最后的颜色
		* @param[in] red 红色分量
		* @param[in] green 绿色分量
		* @param[in] blue 蓝色分量
		*/
		virtual void SetEffectColor(float red = 1.0f, float green = 1.0f, float blue = 1.0f);

		/**
		* @brief 获取物体的宽度
		* @return 物体的宽度
		*/
		virtual auto GetWidth() -> float;

		/**
		* @brief 获取物体的高度
		* @return 物体的高度
		*/
		virtual auto GetHeight() -> float;

		/**
		* @brief 获取物体(中心点)的X坐标
		* @return X坐标
		*/
		virtual auto GetPositionX() -> float;

		/**
		* @brief 获取物体(中心点)的Y坐标
		* @return Y坐标
		*/
		virtual auto GetPositionY() -> float;

		/**
		* @brief 获取物体的渲染ID
		* @return 渲染ID
		*/
		virtual auto GetRenderObjectID() -> int;

		/**
		* @brief 获取深度层
		* @return 深度层
		*/
		virtual auto GetDepthLayer() -> int;

		/**
		* @brief 获取不透明度
		* @return 不透明度
		*/
		virtual auto GetOpacity() -> float;

		/**
		* @brief 获取特效颜色
		* @return 颜色
		*/
		virtual auto GetEffectColor() -> float*;

		/**
		* @brief 获取物体的名字
		* @return 物体的名字
		*/
		virtual auto GetName()->std::string;
	};

}