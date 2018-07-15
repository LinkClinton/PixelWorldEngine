#pragma once

#include "pch.hpp"

#include "KeyCode.hpp"

namespace PixelWorldEngine {

#define NO_MESSAGE -1

	namespace Events {

		//�����¼�
		struct EventBase {

		};


		//����ƶ��¼�
		struct MouseMoveEvent :EventBase {
			int x, y; //���λ��

			MouseMoveEvent() = default;
		};

		//��갴��ö��
		enum MouseButton {
			Left, //���
			Middle, //�м�
			Right //�Ҽ�
		};

		//��갴���¼�
		struct MouseClickEvent :EventBase {
			int x, y; //���λ��
			bool isDown; //�Ƿ���
			MouseButton button; //�ĸ�����

			MouseClickEvent() = default;
		};

		//�������¼�
		struct MouseWheelEvent :EventBase {
			int x, y; //���λ��
			int offset; //����ֵ�������������ϻ�������֮����

			MouseWheelEvent() = default;
		};

		//���̰����¼�
		struct KeyClickEvent :EventBase {
			KeyCode keyCode; //����������
			bool isDown; //�Ƿ���

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

		//��һ���¼�����������¼�����
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
