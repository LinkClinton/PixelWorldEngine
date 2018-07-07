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
	 * @brief UI物体，主要是用于呈现UI等各种和游戏世界无关的东西
	 */
	class UIObject {
	private:
		PixelWorld* pixelWorld; //父亲
		UIObject* parent; //父亲

		std::string name; //名字

		float positionX; //左上角X位置，默认为0
		float positionY; //左上角Y位置，默认为0

		float width; //宽度，默认为1
		float height; //高度，默认为1

		float halfWidth; //宽度的一半
		float halfHeight; //高度的一半

		float opacity; //不透明度，默认为1

		float angle; //角度，旋转中心为物体中心，默认为0

		float borderWidth; //边界宽度，注意其总宽度不会变，因此除非宽度为0，否则真实纹理渲染范围会比真实范围小，默认为0
		float borderColor[3]; //边界颜色，默认为 (0, 0, 0)

		int renderObjectID; //渲染ID，默认为0
		int depthLayer; //深度层，默认为0

		std::map<std::string, UIObject*> children; //子物体，一个物体可以做多个物体的子物体
		std::set<UIObject*, UIObjectCompare> childrenLayer; //子物体层

		glm::mat4x4 transformMatrix; //变换矩阵
		glm::mat4x4 invTransformMatrix; //逆变换矩阵

		friend class PixelWorld;
		friend class UIObjectCompare;
	private:
		/**
		 * @brief 创建一个变换矩阵
		 * @param[in] object 物体
		 * @return 矩阵
		 */
		static auto CreateTransformMatrix(UIObject* object) -> glm::mat4x4;

		static auto CreateTransformInvMatrix(UIObject* object) -> glm::mat4x4;

		/**
		 * @brief 将一个物体从它的父亲的儿子中移除
		 * @param[in] object 物体
		 */
		static void UnRegisterFromParent(UIObject* object);

		/**
		 * @brief 将一个物体从PixelWolrd中移除
		 * @param[in] object 物体
		 */
		static void UnRegisterFromPixelWorld(UIObject* object);

		/**
		 * @brief 处理物体收到的关于鼠标移动的消息，来决定是否要继续传递消息给物体以及子物体
		 * @param[in] object 物体
		 * @param[in] eventArg 消息
		 * @param[in] baseTransformMatrix 祖先的变换矩阵
		 */
		static void ProcessMouseMoveEvent(UIObject* object, Events::MouseMoveEvent* eventArg, glm::mat4x4 baseTransformMatrix);

		/**
		* @brief 处理物体收到的关于鼠标按下的消息，来决定是否要继续传递消息给物体以及子物体
		* @param[in] object 物体
		* @param[in[ eventArg 消息
		* @param[in] baseTransformMatrix 祖先的变换矩阵
		*/
		static void ProcessMouseClickEvent(UIObject* object, Events::MouseClickEvent* eventArg, glm::mat4x4 baseTransformMatrix);

		/**
		* @brief 处理物体收到的关于鼠标滑轮滚动的消息，来决定是否要继续传递消息给物体以及子物体
		* @param[in] object 物体
		* @param[in[ eventArg 消息
		* @param[in] baseTransformMatrix 祖先的变换矩阵
		*/
		static void ProcessMouseWheelEvent(UIObject* object, Events::MouseWheelEvent* eventArg, glm::mat4x4 baseTransformMatrix);

		/**
		* @brief 处理物体收到的关于键盘按下的消息，来决定是否要继续传递消息给物体以及子物体
		* @param[in] object 物体
		* @param[in[ eventArg 消息
		*/
		static void ProcessKeyClickEvent(UIObject* object, Events::KeyClickEvent* eventArg);
	protected:
		/**
		 * @brief 当鼠标移动的时候触发，注意鼠标必须在物体范围内
		 * @param[in] sender 谁触发了消息
		 * @param[in] eventArg 消息信息
		 */
		virtual void OnMouseMove(void* sender, Events::MouseMoveEvent* eventArg);

		/**
		 * @brief 当鼠标按下的时候触发，注意鼠标必须在物体范围内
		 * @param[in] sender 谁触发了消息
		 * @param[in] eventArg 消息信息
		 */
		virtual void OnMouseClick(void* sender, Events::MouseClickEvent* eventArg);

		/** 
		 * @brief 当鼠标滑轮滚动的时候触发，注意鼠标必须在物体范围内
		 * @param[in] sender 谁触发了消息
		 * @param[in] eventArg 消息信息
		 */
		virtual void OnMouseWheel(void* sender, Events::MouseWheelEvent* eventArg);

		/**
		 * @brief 当键盘按键按下去的时候触发，注意必须获取了焦点
		 * @param[in] sender 谁触发了消息
		 * @param[in] eventArg 消息信息
		 */
		virtual void OnKeyClick(void* sender, Events::KeyClickEvent* eventArg);
	public:
		PixelWorldEngine::Events::MouseMoveHandlers MouseMove; //鼠标移动事件
		PixelWorldEngine::Events::MouseClickHandlers MouseClick; //鼠标按下事件
		PixelWorldEngine::Events::MouseWheelHandlers MouseWheel; //鼠标滚轮事件
		PixelWorldEngine::Events::KeyClickEventHandlers KeyClick; //键盘按下事件
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
		void SetBorderColor(float red = 0.0f, float green = 0.0f, float blue = 0.0f);

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
		 * @brief 设置旋转角度，旋转中心为物体中心，默认为0
		 * @param[in] angle 角度
		 */
		void SetAngle(float angle);

		/**
		 * @brief 设置边界宽度
		 * @param[in] width 边界宽度，默认为0
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
		 * @brief 获取旋转角度
		 * @return 旋转角度
		 */
		auto GetAngle() -> float;

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

		/**
		 * @brief 返回这个点是否在物体内部，注意这里不考虑父亲节点的影响以及旋转的影响
		 * @param[in] object 物体
		 * @param[in] x 点的X坐标
		 * @param[in[ y 点的Y坐标
		 */
		static auto IsInUIObjectRect(UIObject* object, float x, float y) -> bool;
	};

}