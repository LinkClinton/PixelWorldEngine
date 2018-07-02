#include "Collider.hpp"

PixelWorldEngine::Collider::Collider(float left, float top, float right, float bottom)
{
	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;

	isEnable = true;
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

void PixelWorldEngine::Collider::SetEnable(bool enable)
{
	isEnable = enable;
}


auto PixelWorldEngine::Collider::GetArea() -> RectangleF
{
	return rect;
}

auto PixelWorldEngine::Collider::IsEnable() -> bool
{
	return isEnable;
}

auto PixelWorldEngine::Collider::Intersect(Collider collider) -> bool
{
	if (isEnable == false || collider.isEnable == false) return false;

	//X-Axis
	if (rect.right < collider.rect.left) return false;
	if (rect.left > collider.rect.right) return false;

	//Y-Axis
	if (rect.bottom < collider.rect.top) return false;
	if (rect.top > collider.rect.bottom) return false;

	return true;
}

auto PixelWorldEngine::Collider::Translate(Collider collider, float x, float y) -> Collider
{
	return Collider(RectangleF::Transform(collider.rect, x, y));
}