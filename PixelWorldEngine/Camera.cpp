#include "Camera.hpp"

PixelWorldEngine::Camera::Camera(RectangleF renderObject)
{
	rect = renderObject;

	project = glm::orthoLH(rect.left, rect.right, rect.bottom, rect.top, 0.f, 1.f);
}

void PixelWorldEngine::Camera::SetRectangle(RectangleF area)
{
	rect = area;

	project = glm::orthoLH(rect.left, rect.right, rect.bottom, rect.top, 0.f, 1.f);
}

void PixelWorldEngine::Camera::SetFocus(float positionX, float positionY, RectangleF margin)
{
	rect = RectangleF(positionX - margin.left, positionY - margin.top, positionX + margin.right,
		positionY + margin.bottom);

	project = glm::orthoLH(rect.left, rect.right, rect.bottom, rect.top, 0.f, 1.f);
}

void PixelWorldEngine::Camera::Move(glm::vec2 transform)
{
	rect.left += transform.x;
	rect.top += transform.y;
	rect.right += transform.x;
	rect.bottom += transform.y;

	project = glm::orthoLH(rect.left, rect.right, rect.bottom, rect.top, 0.f, 1.f);
}

void PixelWorldEngine::Camera::Move(float translationX, float translationY)
{
	rect.left += translationX;
	rect.top += translationY;
	rect.right += translationX;
	rect.bottom += translationY;

	project = glm::orthoLH(rect.left, rect.right, rect.bottom, rect.top, 0.f, 1.f);
}

auto PixelWorldEngine::Camera::GetRectangle() -> RectangleF
{
	return rect;
}

auto PixelWorldEngine::Camera::GetMatrix() -> glm::mat4x4
{
	return project;
}
