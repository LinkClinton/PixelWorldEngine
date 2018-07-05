#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	class PixelWorld;

	/**
	 * @brief UI物体，主要是用于呈现UI等各种和游戏世界无关的东西
	 */
	class UIObject {
	private:
		std::string name; //名字

		float positionX; //左上角X位置，默认为0
		float positionY; //左上角Y位置，默认为0

		float width; //宽度，默认为1
		float height; //高度，默认为1

		float opacity; //不透明度，默认为1

		float borderWidth; //边界宽度，注意其总宽度不会变，因此除非宽度为0，否则真实纹理渲染范围会比真实范围小，默认为1
		float borderColor[4]; //边界颜色，默认为 (0, 0, 0, 1)

		int renderObjectID; //渲染ID，默认为0
		int depthLayer; //深度层，默认为0

		std::map<std::string, UIObject*> children; //子物体，一个物体可以做多个物体的子物体
		
		friend class PixelWorld;
		friend class UIObjectCompare;
	public:
		/**
		 * @brief 构造函数
		 * @param[in] name 名字
		 * @param[in] positionX 左上角X坐标，默认为0
		 * @param[in] positionY 左上角Y坐标，默认为0
		 * @param[in] width 宽度，默认为1
		 * @param[in] height 高度，默认为1
		 */
		UIObject(std::string name, float positionX = 0, float positionY = 0,
			float width = 1.0f, float height = 1.0f);

		/**
		 * @brief 设置边界颜色
		 * @param[in] red 红色分量
		 * @param[in] green 绿色分量
		 * @param[in] blue 蓝色分量
		 * @param[in] alpha alpha分量
		 */
		void SetBorderColor(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f);

		/**
		 * @brief 设置位置
		 * @param[in] positionX 左上角X坐标，默认为0
		 * @param[in] positionY 左上角Y坐标，默认为0
		 */
		void SetPosition(float positionX, float positionY);

		/**
		 * @brief 设置X轴位置
		 * @param[in] positionX 左上角X坐标，默认为0
		 */
		void SetPositionX(float positionX);

		/**
		 * @brief 设置Y轴位置
		 * @param[in] positionY 左上角Y坐标，默认为0
		 */
		void SetPositionY(float positionY);

		/**
		 * @brief 设置大小
		 * @param[in] width 宽度，默认为1
		 * @param[in] height 高度，默认为1
		 */
		void SetSize(float width, float height);

		/**
		 * @brief 设置宽度
		 * @param[in] width 宽度，默认为1
		 */
		void SetWidth(float width);

		/**
		 * @brief 设置高度
		 * @param[in] heiht 高度，默认为1
		 */
		void SetHeight(float height);

		/**
		 * @brief 设置不透明度
		 * @param[in] opacity 不透明度，默认为1
		 */
		void SetOpacity(float opacity);

		/**
		 * @brief 设置边界宽度
		 * @param[in] width 边界宽度，默认为1
		 */
		void SetBorderWidth(float width);

		/**
		 * @brief 设置渲染ID
		 * @param[in] id 渲染ID，默认为0
		 */
		void SetRenderObjectID(int id);

		/**
		 * @brief 设置深度层
		 * @param[in] depthLayer 深度层，默认为0
		 */
		void SetDepthLayer(int depthLayer);

		/**
		* @brief 注册UIObject作为其儿子
		* @param[in] object 物体
		*/
		void RegisterUIObject(UIObject* object);

		/**
		* @brief 卸载UIObject作为其儿子
		* @param[in] object 物体
		*/
		void UnRegisterUIObject(UIObject* object);

		/**
		* @brief 卸载UIObject作为其儿子
		* @param[in] name 物体的名字
		*/
		void UnRegisterUIObject(std::string name);

		/**
		 * @brief 获取边界颜色，返回的是一个指针，请勿直接修改
		 * @return 头元素地址，按照RGBA顺序排列
		 */
		auto GetBorderColor() -> float*;

		/**
		 * @brief 获取左上角位置X坐标
		 * @return 左上角位置X坐标
		 */
		auto GetPositionX() -> float;

		/**
		 * @brief 获取左上角位置Y坐标
		 * @return 左上角位置Y坐标
		 */
		auto GetPositionY() -> float;

		/**
		 * @brief 获取宽度
		 * @return 宽度
		 */
		auto GetWidth() -> float;

		/**
		 * @brief 获取高度
		 * @return 高度
		 */
		auto GetHeight() -> float;

		/**
		 * @brief 获取不透明度
		 * @return 不透明度
		 */
		auto GetOpacity() -> float;

		/**
		 * @brief 获取边界宽度
		 * @return 边界宽度
		 */
		auto GetBorderWidth() -> float;

		/**
		 * @brief 获取渲染ID
		 * @return 渲染ID
		 */
		auto GetRenderObjectID() -> int;

		/**
		 * @brief 获取深度层
		 * @return 深度层
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