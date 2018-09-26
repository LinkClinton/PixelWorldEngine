#include "CollideSolver.hpp"

int PixelWorldEngine::Internal::CollideSolver::iterations = 5;

void PixelWorldEngine::Internal::CollideSolver::BuildCollideObjects(PixelObject * object, glm::mat4x4 baseTransform,
	glm::mat4x4 baseOldTransform, CollideObject::Collection & collideObjects)
{
	CollideObject collideObject;

	collideObject.Collider = object->collider;
	collideObject.Transform = baseTransform * object->Transform.GetMatrix();
	collideObject.OldTransform = baseOldTransform * object->oldTransform.GetMatrix();
	collideObject.Object = object;

	collideObjects.push_back(collideObject);

	for (auto it = object->childrenDepthSort.begin(); it != object->childrenDepthSort.end(); it++)
		BuildCollideObjects(*it, collideObject.Transform, collideObject.OldTransform, collideObjects);
}

void PixelWorldEngine::Internal::CollideSolver::IntersectWithEnterAndLeaveEvent(CollideObject::Collection & collideObjectsA,
	CollideObject::Collection & collideObjectsB, CollideEvent::Collection & enterEvents, CollideEvent::Collection & leaveEvents)
{
	for (size_t i = 0; i < collideObjectsA.size(); i++) {
		for (size_t j = 0; j < collideObjectsB.size(); j++) {
			auto objectA = collideObjectsA[i];
			auto objectB = collideObjectsB[j];

			CollideEvent collideEvent;
			collideEvent.CollideObject[0] = &collideObjectsA[i];
			collideEvent.CollideObject[1] = &collideObjectsB[j];

			if (objectA.Object->IsEnablePhysicsCollide == false &&
				objectB.Object->IsEnablePhysicsCollide == false) {

				bool beforeState = objectA.Collider.Intersect(objectB.Collider, objectA.OldTransform, objectB.OldTransform);
				bool afterState = objectA.Collider.Intersect(objectB.Collider, objectA.Transform, objectB.Transform);

				if (beforeState == true && afterState == false) leaveEvents.push_back(collideEvent);
				if (beforeState == false && afterState == true) enterEvents.push_back(collideEvent);
			}
		}
	}
}

auto PixelWorldEngine::Internal::CollideSolver::IntersectWithCollideEvent(CollideObject::Collection & collideObjectsA, 
	CollideObject::Collection & collideObjectsB, CollideEvent::Collection & collideEvents) -> bool
{
	bool result = false;

	for (size_t i = 0; i < collideObjectsA.size(); i++) {
		for (size_t j = 0; j < collideObjectsB.size(); j++) {
			auto objectA = collideObjectsA[i];
			auto objectB = collideObjectsB[j];

			if (objectA.Object->IsEnablePhysicsCollide == true ||
				objectB.Object->IsEnablePhysicsCollide == true) {

				if (objectA.Collider.Intersect(objectB.Collider, objectA.Transform, objectB.Transform) == true) {

					result = true;

					CollideEvent collideEvent;
					collideEvent.CollideObject[0] = &collideObjectsA[i];
					collideEvent.CollideObject[1] = &collideObjectsB[j];

					collideEvents.push_back(collideEvent);
				}
			}
		}
	}

	return result;
}

auto PixelWorldEngine::Internal::CollideSolver::Intersect(CollideObject::Collection &collideObjectsA, CollideObject::Collection & collideObjectsB) -> bool
{
	for (auto itA = collideObjectsA.begin(); itA != collideObjectsA.end(); itA++) {
		for (auto itB = collideObjectsB.begin(); itB != collideObjectsB.end(); itB++) {
			if (itA->Collider.Intersect(itB->Collider, itA->Transform, itB->Transform) == true) return true;
		}
	}

	return false;
}

void PixelWorldEngine::Internal::CollideSolver::SolveCollideEvent(CollideEvent collideEvent)
{
	float leftValue = 0.0f;
	float rightValue = 1.0f;
	int times = iterations;
		
	auto transformA = collideEvent.CollideObject[0]->Object->Transform;
	auto transformB = collideEvent.CollideObject[1]->Object->Transform;

	auto oldTransformA = collideEvent.CollideObject[0]->Object->oldTransform;
	auto oldTransformB = collideEvent.CollideObject[1]->Object->oldTransform;

	auto tempTransformA = transformA - oldTransformA;
	auto tempTransformB = transformB - oldTransformB;

	for (int i = 0; i < times; i++) {
		float halfValue = (leftValue + rightValue) * 0.5f;
		
		auto matrixA = (oldTransformA + tempTransformA * halfValue).GetMatrix();
		auto matrixB = (oldTransformB + tempTransformB * halfValue).GetMatrix();

		if (collideEvent.CollideObject[0]->Collider.Intersect(collideEvent.CollideObject[1]->Collider,
			matrixA, matrixB) == true) rightValue = halfValue;
		else leftValue = halfValue;
	}

	auto value = leftValue;

	collideEvent.CollideObject[0]->Object->Transform = oldTransformA + tempTransformA * value;
	collideEvent.CollideObject[1]->Object->Transform = oldTransformB + tempTransformB * value;
	
	collideEvent.CollideObject[0]->Transform = collideEvent.CollideObject[0]->Object->Transform.GetMatrix();
	collideEvent.CollideObject[1]->Transform = collideEvent.CollideObject[1]->Object->Transform.GetMatrix();
}

void PixelWorldEngine::Internal::CollideSolver::SolveCollide(PixelObject * rootObject)
{
	std::vector<CollideObject::Collection> objectColliders;
	std::vector<CollideEvent> collideEvents;

	objectColliders.resize(rootObject->childrenDepthSort.size());

	int childrenCount = 0;

	for (auto it = rootObject->childrenDepthSort.begin(); it != rootObject->childrenDepthSort.end(); it++)
		BuildCollideObjects(*it, glm::mat4(1), glm::mat4(1), objectColliders[childrenCount++]);

	for (size_t i = 0; i < objectColliders.size(); i++) {
		for (size_t j = i + 1; j < objectColliders.size(); j++) {
			IntersectWithCollideEvent(objectColliders[i], objectColliders[j], collideEvents);
		}
	}

	//solve the collide 
	for (auto it = collideEvents.begin(); it != collideEvents.end(); it++)
		SolveCollideEvent(*it);

	for (auto it = collideEvents.begin(); it != collideEvents.end(); it++)
		Internal::PixelObjectProcess::ProcessObjectCollide(it->CollideObject[0]->Object, it->CollideObject[1]->Object);
	//end
	
	std::vector<CollideEvent> enterEvents;
	std::vector<CollideEvent> leaveEvents;

	for (size_t i = 0; i < objectColliders.size(); i++) {
		for (size_t j = i + 1; j < objectColliders.size(); j++) {
			IntersectWithEnterAndLeaveEvent(objectColliders[i], objectColliders[j],
				enterEvents, leaveEvents);
		}
	}

	for (auto it = collideEvents.begin(); it != collideEvents.end(); it++)
		PixelObjectProcess::ProcessObjectCollide(it->CollideObject[0]->Object, it->CollideObject[1]->Object);

	for (auto it = enterEvents.begin(); it != enterEvents.end(); it++)
		PixelObjectProcess::ProcessObjectEnter(it->CollideObject[0]->Object, it->CollideObject[1]->Object);

	for (auto it = leaveEvents.begin(); it != leaveEvents.end(); it++)
		PixelObjectProcess::ProcessObjectLeave(it->CollideObject[0]->Object, it->CollideObject[1]->Object);

	PixelObject::UpdateTransform(rootObject);
}

void PixelWorldEngine::Internal::CollideSolver::SetIterations(int count)
{
	iterations = count;
}

auto PixelWorldEngine::Internal::CollideSolver::Intersect(PixelObject * object1, PixelObject * object2) -> bool
{
	std::vector<CollideObject> collideObjects[2];

	BuildCollideObjects(object1, glm::mat4(1), glm::mat4(1), collideObjects[0]);
	BuildCollideObjects(object2, glm::mat4(1), glm::mat4(1), collideObjects[1]);

	return Intersect(collideObjects[0], collideObjects[1]);
}
