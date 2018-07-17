#include "Object.hpp"
#include "DebugLayer.hpp"

PixelWorldEngine::Object::Object(std::string Name, float PositionX, float PositionY, float Width, float Height)
{
	name = Name;

	positionX = PositionX;
	positionY = PositionY;

	width = Width;
	height = Height;

	renderObjectID = 0;
	depthLayer = 0;
	opacity = 1.0f;

	pixelWorld = nullptr;

	effectColor[0] = 1.0f;
	effectColor[1] = 1.0f;
	effectColor[2] = 1.0f;

	DebugReturn(DebugLayer::Assert(width <= 0 || height <= 0, Error::WidthOrHeightLessThanZero, Name, "Object"));
}

void PixelWorldEngine::Object::SetSize(float objectWidth, float objectHeight)
{
	DebugReturn(DebugLayer::Assert(objectWidth <= 0 || objectHeight <= 0, Error::WidthOrHeightLessThanZero, name, FunctionName));

	width = objectWidth;
	height = objectHeight;
}

void PixelWorldEngine::Object::SetPosition(float x, float y)
{
	positionX = x;
	positionY = y;
}

void PixelWorldEngine::Object::SetRenderObjectID(int id)
{
	renderObjectID = id;
}

void PixelWorldEngine::Object::SetOpacity(float Opacity)
{
	opacity = Opacity;
}

void PixelWorldEngine::Object::SetEffectColor(float red, float green, float blue)
{
	effectColor[0] = red;
	effectColor[1] = green;
	effectColor[2] = blue;
}

auto PixelWorldEngine::Object::GetWidth() -> float
{
	return width;
}

auto PixelWorldEngine::Object::GetHeight() -> float
{
	return height;
}

auto PixelWorldEngine::Object::GetPositionX() -> float
{
	return positionX;
}

auto PixelWorldEngine::Object::GetPositionY() -> float
{
	return positionY;
}

auto PixelWorldEngine::Object::GetRenderObjectID() -> int
{
	return renderObjectID;
}

auto PixelWorldEngine::Object::GetDepthLayer() -> int
{
	return depthLayer;
}

auto PixelWorldEngine::Object::GetOpacity() -> float
{
	return opacity;
}

auto PixelWorldEngine::Object::GetEffectColor() -> float *
{
	return effectColor;
}

auto PixelWorldEngine::Object::GetName() -> std::string
{
	return name;
}