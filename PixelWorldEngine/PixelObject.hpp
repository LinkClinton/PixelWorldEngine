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
		 * @brief 用于比较物体的深度值的比较函数
		 */
		class PixelObjectCompare {
		public:
			bool operator () (PixelObject* object1, PixelObject* object2)const;
		};

		/**
		 * @brief 物体的事件处理函数
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
	 * @brief 物体，引擎的核心类，不可复制实例，只能使用new构造
	 */
	class PixelObject {
	private:
		std::string name; //物体的名字
		
		PixelObject* parent; //物体的父亲

		float width; //物体的宽度
		float height; //物体的高度

		int depth; //物体的深度，用于决定处于同层的物体显示的先后顺序
	
		bool isHover; //鼠标是否悬浮在上面
		bool isSizeChange; //大小是否改变

		Collider collider; //碰撞盒

		Transform oldTransform; //旧的变换，即在上一次处理变换之前的变换

		std::map<std::string, PixelObject*> childrenNameIndex; //儿子物体
		std::multiset<PixelObject*, Internal::PixelObjectCompare> childrenDepthSort; //儿子物体，按照深度排序

		/**/
		PixelWorldEngine::Text* textInstance; //文字纹理
		int objectID; //物体编号，同时也是文字的渲染编号

		static int ObjectCount; //记录物体个数
		static std::vector<int> FreeObjectID; //空闲的ID

		/**
		 * @brief 更新物体的变换，将旧变换改变成新变换
		 * @param[in] object 物体，包括其子孙物体
		 */
		static void UpdateTransform(PixelObject* object); 

		/**
		 * @brief 更新物体的文本内容
		 * @param[in] object 物体
		 */
		static void UpdateText(PixelObject* object);

		/**
		 * @brief 获取物体的编号，如果有空余编号，那么使用空余编号，否则新增编号
		 * @return 物体的编号
		 */
		static auto GetObjectID() -> int;

		friend class PixelWorld;
		friend class Internal::PixelObjectProcess;
		friend class Internal::PixelObjectCompare;
		friend class Internal::CollideSolver;
	protected:
		/**
		 * @brief 更新事件
		 * @param[in] sender 谁触发了
		 */
		virtual void OnUpdate(void* sender) {}

		/**
		 * @brief 更新结束后触发
		 * @param[in] sender 谁触发了
		 */
		virtual void OnAfterUpdate(void* sender) {}

		/**
		 * @brief 鼠标进入事件，当鼠标进入物体范围的时候触发
		 * @param[in] sender 谁触发了
		 */
		virtual void OnMouseEnter(void* sender) {}

		/**
		 * @brief 鼠标离开事件，当鼠标离开物体范围的时候触发
		 * @param[in] sender 谁触发了
		 */
		virtual void OnMouseLeave(void* sender) {}

		/**
		 * @brief 鼠标移动事件，当鼠标移动的时候触发，要求鼠标在物体范围内
		 * @param[in] sender 谁触发了
		 * @parma[in] eventArg 事件信息
		 */
		virtual void OnMouseMove(void* sender, Events::MouseMoveEvent* eventArg) {}

		/**
		 * @brief 鼠标点击事件，当鼠标点击的时候触发，要求鼠标在物体范围内
		 * @param[in] sender 谁触发了
		 * @param[in] eventArg 事件信息
		 */
		virtual void OnMouseClick(void* sender, Events::MouseClickEvent* eventArg) {}

		/**
		* @brief 鼠标滚动事件，当鼠标滚动的时候触发，要求鼠标在物体范围内
		* @param[in] sender 谁触发了
		* @param[in] eventArg 事件信息
		*/
		virtual void OnMouseWheel(void* sender, Events::MouseWheelEvent* eventArg) {}

		/**
		 * @brief 键盘按键事件，当键盘按下或者弹起的时候触发，要求允许开启键盘按键事件
		 * @param[in] sender 谁触发了
		 * @param[in] eventArg 事件信息
		 */
		virtual void OnKeyClick(void* sender, Events::KeyClickEvent* eventArg) {}

		/**
		 * @brief 物体碰撞事件，只有物体开启了物理碰撞以及处于世界层的物体才会触发
		 * @param[in] objectA 碰撞物体A
		 * @param[in] objectB 碰撞物体B
		 */
		virtual void OnObjectCollide(PixelObject* objectA, PixelObject* objectB) {}

		/**
		 * @brief 物体进入事件，只有物体关闭了物理碰撞以及处于世界层的物体才会触发
		 * @param[in] objectA 相交物体A
		 * @param[in] objectB 相交物体B
		 */
		virtual void OnObjectEnter(PixelObject* objectA, PixelObject* objectB) {}

		/**
		* @brief 物体离开事件，只有物体关闭了物理碰撞以及处于世界层的物体才会触发
		* @param[in] objectA 离开物体A
		* @param[in] objectB 离开物体B
		*/
		virtual void OnObjectLeave(PixelObject* objectA, PixelObject* objectB) {}
	public:
		Events::UpdateEventHandlers Update; //更新事件集合
		Events::UpdateEventHandlers AfterUpdate; //更新完成后事件
		Events::MouseEnterEventHandlers MouseEnter; //鼠标进入事件集合
		Events::MouseLeaveEventHandlers MouseLeave; //鼠标离开事件集合
		Events::MouseMoveHandlers MouseMove; //鼠标移动事件集合
		Events::MouseClickHandlers MouseClick; //鼠标按键事件集合
		Events::MouseWheelHandlers MouseWheel; //鼠标滚轮事件集合
		Events::KeyClickEventHandlers KeyClick; //键盘按键事件集合
		Events::CollideEventHandlers ObjectCollide; //物体碰撞事件集合
		Events::CollideEventHandlers ObjectEnter; //物体相交事件集合
		Events::CollideEventHandlers ObjectLeave; //物体离开事件集合

		Transform Transform; //物体的变换，包括位移旋转缩放

		int RenderObjectID; //物体的渲染编号，默认为0

		float Opacity; //物体的不透明度，默认为1

		glm::vec3 TextColor; //字体颜色，默认为 (0, 0, 0)

		std::string Text; //文本内容，设置为""将不会显示文字

		Graphics::Font* Font; //字体，设置为nullptr的话将不会显示文字

		bool IsEnableVisual; //是否允许显示，默认为true
		bool IsEnableRead; //是否允许键盘事件，默认为false
		bool IsEnablePhysicsCollide; //是否允许物理碰撞，默认为false
	public:
		/**
		 * @brief 构造函数
		 * @param[in] name 物体的名字
		 * @param[in] transform 物体的变换
		 */
		PixelObject(std::string name, PixelWorldEngine::Transform transform = PixelWorldEngine::Transform());

		/**
		 * @brief 匿名构造，其名字将会是PixelObject + 其物体编号
		 * @param[in] transform 物体的变换
		 */
		PixelObject(PixelWorldEngine::Transform transform = PixelWorldEngine::Transform());

		/**
		 * @brief 默认析构函数
		 */
		~PixelObject();

		/**
		 * @brief 设置物体的深度
		 * @param[in] depth 物体的深度
		 */
		virtual void SetDepth(int depth);

		/**
		 * @brief 设置物体的大小
		 * @param[in] width 物体的宽度
		 * @param[in] height 物体的高度
		 */
		virtual void SetSize(float width, float height);

		/**
		 * @brief 设置物体的父亲
		 * @param[in] parent 父亲物体
		 */
		virtual void SetParent(PixelObject* parent);

		/**
		 * @brief 设置物体的子物体，可以设置多个
		 * @param[in] child 子物体
		 */
		virtual void SetChild(PixelObject* child);

		/**
		 * @brief 移除子物体
		 * @param[in] name 子物体的名字
		 */
		virtual void CancelChild(std::string name);

		/**
		 * @brief 获取物体的名字
		 * @return 物体的名字
		 */
		virtual auto GetName() -> std::string;

		/**
		 * @brief 获取物体的深度
		 * @param[in] 深度
		 */
		virtual auto GetDepth() -> int;

		/**
		 * @brief 获取物体的大小
		 * @param[in] 物体的大小
		 */
		virtual auto GetSize() -> SizeF;
		
		/**
		 * @brief 获取物体的父亲，如果没有就返回nullptr
		 * @return 物体的父亲
		 */
		virtual auto GetParent() -> PixelObject*;

		/**
		 * @brief 获取物体的子物体，如果没有就返回nullptr
		 * @return 物体的子物体
		 */
		virtual auto GetChildren(std::string name) -> PixelObject*;

		/**
		 * @brief 基于一个物体创建一个新的物体
		 * @param[in] name 新物体的名字
		 * @param[in] object 旧物体
		 * @return 新物体的地址
		 */
		static auto CreateFromInstance(std::string name, PixelObject* object) -> PixelObject*;
	};

}