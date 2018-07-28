#pragma once

#include "pch.hpp"

#include "Collider.hpp"
#include "Transform.hpp"
#include "PixelObject.hpp"

namespace PixelWorldEngine {

	namespace Internal {

		struct CollideObject {
			Collider Collider;

			glm::mat4x4 OldTransform;
			glm::mat4x4 Transform;

			PixelObject* Object;

			typedef std::vector<CollideObject> Collection;
		};

		struct CollideEvent {
			CollideObject* CollideObject[2];

			typedef std::vector<CollideEvent> Collection;
		};

		class CollideSolver {
		private:
			static void BuildCollideObjects(PixelObject* object, glm::mat4x4 baseTransform, glm::mat4x4 baseOldTransform, CollideObject::Collection &collideObjects);

			static void IntersectWithEnterAndLeaveEvent(CollideObject::Collection &collideObjectsA, CollideObject::Collection &collideObjectsB,
				CollideEvent::Collection &enterEvents, CollideEvent::Collection &leaveEvents);

			static auto IntersectWithCollideEvent(CollideObject::Collection & collideObjectsA, CollideObject::Collection &collideObjectsB,
				CollideEvent::Collection &collideEvents) -> bool;

			static auto Intersect(CollideObject::Collection &collideObjectsA, CollideObject::Collection &collideObjectsB) -> bool;
		public:
			void SolveCollide(PixelObject* rootObject);

			static auto Intersect(PixelObject* object1, PixelObject* object2);
		};

	}
}