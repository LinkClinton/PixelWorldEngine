#include "Collider.hpp"

PixelWorldEngine::Collider::Collider(float left, float top, float right, float bottom)
{
	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;

	isEnablePhysics = true;
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

void PixelWorldEngine::Collider::EnablePhysics(bool enable)
{
	isEnablePhysics = enable;
}


auto PixelWorldEngine::Collider::GetArea() -> RectangleF
{
	return rect;
}

auto PixelWorldEngine::Collider::IsEnablePhysics() -> bool
{
	return isEnablePhysics;
}

auto PixelWorldEngine::Collider::Intersect(glm::vec2 position, glm::mat4x4 transform) -> bool
{
	auto localPosition = glm::inverse(transform) * glm::vec4(position, 0, 1);

	if (localPosition.x >= rect.left && localPosition.x <= rect.right &&
		localPosition.y >= rect.top && localPosition.y <= rect.bottom)
		return true;

	return false;
}
