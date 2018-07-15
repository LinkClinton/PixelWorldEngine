#pragma once

#include "pch.hpp"

#include "KeyCode.hpp"

namespace PixelWorldEngine {

#define NO_MESSAGE -1

	namespace Events {

		//基础事件
		struct EventBase {

		};


		//鼠标移动事件
		struct MouseMoveEvent :EventBase {
			int x, y; //鼠标位置

			MouseMoveEvent() = default;
		};

		//鼠标按键枚举
		enum MouseButton {
			Left, //左键
			Middle, //中键
			Right //右键
		};

		//鼠标按键事件
		struct MouseClickEvent :EventBase {
			int x, y; //鼠标位置
			bool isDown; //是否按下
			MouseButton button; //哪个按键

			MouseClickEvent() = default;
		};

		//鼠标滚轮事件
		struct MouseWheelEvent :EventBase {
			int x, y; //鼠标位置
			int offset; //滑动值，正数则是往上滑动，反之往下

			MouseWheelEvent() = default;
		};

		//键盘按键事件
		struct KeyClickEvent :EventBase {
			KeyCode keyCode; //按键虚拟码
			bool isDown; //是否按下

			KeyClickEvent() = default;
		};

		
		struct SizeChangeEvent :EventBase {
			int width;
			int height;

			SizeChangeEvent() = default;
		};

		typedef std::function<void(void*, MouseMoveEvent*)> MouseMoveHandler;
		typedef std::function<void(void*, MouseClickEvent*)> MouseClickHandler;
		typedef std::function<void(void*, MouseWheelEvent*)> MouseWheelHandler;
		typedef std::function<void(void*, KeyClickEvent*)> KeyClickEventHandler;
		typedef std::function<void(void*)> UpdateEventHandler;

		typedef std::vector<MouseMoveHandler> MouseMoveHandlers;
		typedef std::vector<MouseClickHandler> MouseClickHandlers;
		typedef std::vector<MouseWheelHandler> MouseWheelHandlers;
		typedef std::vector<KeyClickEventHandler> KeyClickEventHandlers;
		typedef std::vector<UpdateEventHandler> UpdateEventHandlers;

		class EventHandler {
		public:
			friend MouseMoveHandlers operator += (MouseMoveHandlers &handlers, MouseMoveHandler handler);
			
			friend MouseClickHandlers operator += (MouseClickHandlers &handlers, MouseClickHandler handler);

			friend MouseWheelHandlers operator += (MouseWheelHandlers &handlers, MouseWheelHandler handler);

			friend KeyClickEventHandlers operator += (KeyClickEventHandlers &handlers, KeyClickEventHandler handler);

			friend UpdateEventHandlers operator += (UpdateEventHandlers &handlers, UpdateEventHandler handler);
		};

		//将一个事件集合里面的事件处理
		template<typename Handlers, typename EventArg>
		static void DoEventHandlers(Handlers handlers, void* sender, EventArg eventarg) {
			for (size_t i = 0; i < handlers.size(); i++)
				handlers[i](sender, eventarg);
		}

		template<typename Handlers>
		static void DoEventHandlers(Handlers handlers, void * sender) {
			for (size_t i = 0; i < handlers.size(); i++)
				handlers[i](sender);
		}

		template<typename Handlers, typename EventArg>
		static void DoEventHandlers(Handlers handlers, EventArg eventarg) {
			for (size_t i = 0; i < handlers.size(); i++)
				handlers[i](eventarg);
		}
	}
}
