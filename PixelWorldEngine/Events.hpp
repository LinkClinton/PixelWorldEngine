#pragma once

#include "pch.hpp"

#include "KeyCode.hpp"

namespace PixelWorldEngine {

#define NO_MESSAGE -1

	namespace Events {

		enum class EventType :int {
			EventOther,
			EventMouseMove,
			EventMouseClick,
			EventMouseWheel,
			EventBoardClick,
			EventGetFocus,
			EventLostFocus,
			EventMouseLeave,
			EventSizeChanged
		};

		//Base Event
		struct EventBase {

		};


		//MouseMove Event
		struct MouseMoveEvent :EventBase {
			int x, y;

			MouseMoveEvent() = default;
		};

		//enum of mouse button
		enum MouseButton {
			Left, Middle, Right
		};

		//MouseClick Event
		struct MouseClickEvent :EventBase {
			int x, y;
			bool isDown;
			MouseButton button;

			MouseClickEvent() = default;
		};

		//MouseWheel Event
		struct MouseWheelEvent :EventBase {
			int x, y;
			int offset; //the mouse wheel move + is up,- is down

			MouseWheelEvent() = default;
		};

		//BoardClick Event
		struct KeyClickEvent :EventBase {
			KeyCode keyCode;
			bool isDown;

			KeyClickEvent() = default;
		};

		//SizeChange Event
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
	}
}
