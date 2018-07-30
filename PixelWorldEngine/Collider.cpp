#include "Collider.hpp"

#include "Utility.hpp"

auto PixelWorldEngine::Collider::SubIntersect(Collider colliderA, Collider colliderB, glm::mat4x4 transformA, glm::mat4x4 TransformB) -> bool
{
	glm::vec4 position[4];

	position[0] = glm::vec4(colliderB.rect.left, colliderB.rect.top, 0.0f, 1.0f);
	position[1] = glm::vec4(colliderB.rect.left, colliderB.rect.bottom, 0.0f, 1.0f);
	position[2] = glm::vec4(colliderB.rect.right, colliderB.rect.bottom, 0.0f, 1.0f);
	position[3] = glm::vec4(colliderB.rect.right, colliderB.rect.top, 0.0f, 1.0f);

	auto matrix = glm::inverse(transformA) * TransformB;

	for (int i = 0; i < 4; i++) {
		auto localPosition = matrix * position[i];

		if (localPosition.x >= colliderA.rect.left && localPosition.x <= colliderA.rect.right &&
			localPosition.y >= colliderA.rect.top && localPosition.y <= colliderA.rect.bottom) return true;
	}

	return false;
}

PixelWorldEngine::Collider::Collider(float left, float top, float right, float bottom)
{
	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;
}

PixelWorldEngine::Collider::Collider(RectangleF rectangle)
{
	rect = rectangle;
}

void PixelWorldEngine::Collider::SetArea(float left, float top, float right, float bottom)
{
	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;
}

auto PixelWorldEngine::Collider::GetArea() -> RectangleF
{
	return rect;
}

auto PixelWorldEngine::Collider::Intersect(glm::vec2 position, glm::mat4x4 transform) -> bool
{
	auto localPosition = glm::inverse(transform) * glm::vec4(position, 0.0f, 1.0f);

	if (localPosition.x >= rect.left && localPosition.x <= rect.right &&
		localPosition.y >= rect.top && localPosition.y <= rect.bottom)
		return true;

	return false;
}

auto PixelWorldEngine::Collider::Intersect(Collider other, glm::mat4x4 transform, glm::mat4x4 otherTransform) -> bool
{
	return SubIntersect(*this, other, transform, otherTransform) | SubIntersect(other, *this, otherTransform, transform);
}
