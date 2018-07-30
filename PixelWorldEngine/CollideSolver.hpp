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

			static void SolveCollideEvent(CollideEvent collideEvent);

			static int iterations;
		public:
			void SolveCollide(PixelObject* rootObject);

			/**
			 * @brief 设置模拟迭代的次数，主要是用于碰撞物体我们通过二分来逼近两者之间的距离
			 * @param[in] count 迭代次数，默认为10
			 */
			static void SetIterations(int count);

			/**
			 * @brief 检测两个物体是否相交，包括子物体的检测
			 * @param[in] object1 物体
			 * @param[in] object2 物体
			 * @return 是否相交
			 */
			static auto Intersect(PixelObject* object1, PixelObject* object2) -> bool;
		};

	}
}