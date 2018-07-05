#include "UIObject.hpp"

PixelWorldEngine::UIObject::UIObject(std::string Name, float PositionX, float PositionY, float Width, float Height)
{
	name = Name;

	positionX = PositionX;
	positionY = PositionY;

	width = Width;
	height = Height;

	opacity = 1.0f;

	borderWidth = 1.0f;

	renderObjectID = 0;
	depthLayer = 0;
}

void PixelWorldEngine::UIObject::SetBorderColor(float red, float green, float blue, float alpha)
{
	borderColor[0] = red;
	borderColor[1] = green;
	borderColor[2] = blue;
	borderColor[3] = alpha;
}

void PixelWorldEngine::UIObject::SetPosition(float PositionX, float PositionY)
{
	positionX = PositionX;
	positionY = PositionY;
}

void PixelWorldEngine::UIObject::SetPositionX(float PositionX)
{
	positionX = PositionX;
}

void PixelWorldEngine::UIObject::SetPositionY(float PositionY)
{
	positionY = PositionY;
}

void PixelWorldEngine::UIObject::SetSize(float Width, float Height)
{
	width = Width;
	height = Height;
}

void PixelWorldEngine::UIObject::SetWidth(float Width)
{
	width = Width;
}

void PixelWorldEngine::UIObject::SetHeight(float Height)
{
	height = Height;
}

void PixelWorldEngine::UIObject::SetOpacity(float Opacity)
{
	opacity = Opacity;
}

void PixelWorldEngine::UIObject::SetBorderWidth(float width)
{
	borderWidth = width;
}

void PixelWorldEngine::UIObject::SetRenderObjectID(int id)
{
	renderObjectID = id;
}

void PixelWorldEngine::UIObject::SetDepthLayer(int DepthLayer)
{
	depthLayer = DepthLayer;
}

void PixelWorldEngine::UIObject::RegisterUIObject(UIObject * object)
{
	children[object->name] = object;
}

void PixelWorldEngine::UIObject::UnRegisterUIObject(UIObject * object)
{
	children.erase(object->name);
}

void PixelWorldEngine::UIObject::UnRegisterUIObject(std::string name)
{
	children.erase(name);
}

auto PixelWorldEngine::UIObject::GetBorderColor() -> float *
{
	return borderColor;
}

auto PixelWorldEngine::UIObject::GetPositionX() -> float
{
	return positionX;
}

auto PixelWorldEngine::UIObject::GetPositionY() -> float
{
	return positionY;
}

auto PixelWorldEngine::UIObject::GetWidth() -> float
{
	return width;
}

auto PixelWorldEngine::UIObject::GetHeight() -> float
{
	return height;
}

auto PixelWorldEngine::UIObject::GetOpacity() -> float
{
	return opacity;
}

auto PixelWorldEngine::UIObject::GetBorderWidth() -> float
{
	return borderWidth;
}

auto PixelWorldEngine::UIObject::GetRenderObjectID() -> int
{
	return renderObjectID;
}

auto PixelWorldEngine::UIObject::GetDepthLayer() -> int
{
	return depthLayer;
}