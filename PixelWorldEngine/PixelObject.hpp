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
	class PixelObject {
	private:
		std::string name;
		
		PixelObject* parent;

		float width;
		float height;

		int depth;
	
		bool isHover;
		bool isEnableRead;
		bool isEnablePhysicsCollide;

		Collider collider;

		Transform oldTransform;

		std::map<std::string, PixelObject*> childrenNameIndex;
		std::multiset<PixelObject*, Internal::PixelObjectCompare> childrenDepthSort;

		friend class PixelWorld;
		friend class Internal::PixelObjectProcess;
		friend class Internal::PixelObjectCompare;
		friend class Internal::CollideSolver;
	protected:
		virtual void OnUpdate(void* sender) {}

		virtual void OnMouseEnter(void* sender) {}

		virtual void OnMouseLeave(void* sender) {}

		virtual void OnMouseMove(void* sender, Events::MouseMoveEvent* eventArg) {}

		virtual void OnMouseClick(void* sender, Events::MouseClickEvent* eventArg) {}

		virtual void OnMouseWheel(void* sender, Events::MouseWheelEvent* eventArg) {}

		virtual void OnKeyClick(void* sender, Events::KeyClickEvent* eventArg) {}

		virtual void OnObjectCollide(PixelObject* objectA, PixelObject* objectB) {}

		virtual void OnObjectEnter(PixelObject* objectA, PixelObject* objectB) {}

		virtual void OnObjectLeave(PixelObject* objectA, PixelObject* objectB) {}
	public:
		Events::UpdateEventHandlers Update;
		Events::MouseEnterEventHandlers MouseEnter;
		Events::MouseLeaveEventHandlers MouseLeave;
		Events::MouseMoveHandlers MouseMove;
		Events::MouseClickHandlers MouseClick;
		Events::MouseWheelHandlers MouseWheel;
		Events::KeyClickEventHandlers KeyClick;
		Events::CollideEventHandlers ObjectCollide;
		Events::CollideEventHandlers ObjectEnter;
		Events::CollideEventHandlers ObjectLeave;

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