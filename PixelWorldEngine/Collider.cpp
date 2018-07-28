#include "Collider.hpp"

#include "Utility.hpp"

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
	glm::vec4 position[4];

	position[0] = glm::vec4(other.rect.left, other.rect.top, 0.0f, 1.0f);
	position[1] = glm::vec4(other.rect.left, other.rect.bottom, 0.0f, 1.0f);
	position[2] = glm::vec4(other.rect.right, other.rect.bottom, 0.0f, 1.0f);
	position[3] = glm::vec4(other.rect.right, other.rect.top, 0.0f, 1.0f);

	auto matrix = glm::inverse(transform) * otherTransform;

	glm::vec2 maxValue = glm::vec2(matrix * position[0]);
	glm::vec2 minValue = glm::vec2(matrix * position[0]);

	for (int i = 1; i < 4; i++) {
		auto localPosition = matrix * position[i];

		maxValue.x = Utility::Max(maxValue.x, localPosition.x);
		maxValue.y = Utility::Max(maxValue.y, localPosition.y);
		minValue.x = Utility::Min(minValue.x, localPosition.x);
		minValue.y = Utility::Min(minValue.y, localPosition.y);
	}

	if (rect.right < minValue.x || maxValue.x < rect.left) return false;
	if (rect.bottom < minValue.y || maxValue.y < rect.top) return false;

	return true;
}
