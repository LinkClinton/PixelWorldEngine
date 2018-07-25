#pragma once

#include "pch.hpp"

#include "Events.hpp"

#include "Transform.hpp"
#include "Collider.hpp"
#include "BaseStruct.hpp"

namespace PixelWorldEngine {

	class PixelObject;

	namespace Internal {

		class PixelObjectCompare {
		public:
			bool operator () (PixelObject* object1, PixelObject* object2)const;
		};

		class PixelObjectProcess {
		public:
			static void ProcessUpdate(PixelObject* object);

			static void ProcessMouseEnter(PixelObject* object);

			static void ProcessMouseLeave(PixelObject* object);

			static void ProcessMouseMove(PixelObject* object, Events::MouseMoveEvent* eventArg, glm::mat4x4 baseTransformMatrix);

			static void ProcessMouseClick(PixelObject* object, Events::MouseClickEvent* eventArg, glm::mat4x4 baseTransformMatrix);

			static void ProcessMouseWheel(PixelObject* object, Events::MouseWheelEvent* eventArg, glm::mat4x4 baseTransformMatrix);

			static void ProcessKeyClick(PixelObject* object, Events::KeyClickEvent* eventArg);
		};

	}



	class PixelObject {
	private:
		std::string name;
		
		PixelObject* parent;

		float width;
		float height;

		int depth;
	
		bool isHover;
		bool isEnableRead;

		Collider collider;

		std::map<std::string, PixelObject*> childrenNameIndex;
		std::multiset<PixelObject*, Internal::PixelObjectCompare> childrenDepthSort;

		friend class PixelWorld;
		friend class Internal::PixelObjectProcess;
		friend class Internal::PixelObjectCompare;
	protected:
		virtual void OnUpdate(void* sender) {}

		virtual void OnMouseEnter(void* sender) {}

		virtual void OnMouseLeave(void* sender) {}

		virtual void OnMouseMove(void* sender, Events::MouseMoveEvent* eventArg) {}

		virtual void OnMouseClick(void* sender, Events::MouseClickEvent* eventArg) {}

		virtual void OnMouseWheel(void* sender, Events::MouseWheelEvent* eventArg) {}

		virtual void OnKeyClick(void* sender, Events::KeyClickEvent* eventArg) {}
	public:
		Events::UpdateEventHandlers UpdateEvent;
		Events::MouseEnterEventHandlers MouseEnterEvent;
		Events::MouseLeaveEventHandlers MouseLeaveEvent;
		Events::MouseMoveHandlers MouseMoveEvent;
		Events::MouseClickHandlers MouseClickEvent;
		Events::MouseWheelHandlers MouseWheelEvent;
		Events::KeyClickEventHandlers KeyClickEvent;

		Transform Transform;

		int RenderObjectID;

		float Opacity;
	public:
		PixelObject(std::string name, PixelWorldEngine::Transform transform = PixelWorldEngine::Transform());

		virtual void SetDepth(int depth);

		virtual void SetSize(float width, float height);

		virtual void SetParent(PixelObject* parent);

		virtual void SetChild(PixelObject* child);

		virtual void CancelChild(std::string name);

		virtual auto GetName() -> std::string;

		virtual auto GetDepth() -> int;

		virtual auto GetSize() -> SizeF;
		
		virtual auto GetParent() -> PixelObject*;

		virtual auto GetChildren(std::string name) -> PixelObject*;
	};

}