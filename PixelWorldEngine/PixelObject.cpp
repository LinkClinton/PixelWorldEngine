#include "PixelObject.hpp"

PixelWorldEngine::PixelObject::PixelObject(std::string Name, float PositionX, float PositionY, float Size)
{
	name = Name;

	positionX = PositionX;
	positionY = PositionY;

	size = Size;

	renderObjectID = 0;
}

void PixelWorldEngine::PixelObject::Move(float translationX, float translationY)
{
	if (pixelWorld == nullptr) return;

	
}
