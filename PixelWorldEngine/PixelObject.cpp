#include "PixelObject.hpp"
#include "PixelWorld.hpp"

PixelWorldEngine::PixelObject::PixelObject(std::string Name, float PositionX, float PositionY, float Width, float Height)
{
	name = Name;

	positionX = PositionX;
	positionY = PositionY;

	width = Width;
	height = Height;

	halfWidth = width * 0.5f;
	halfHeight = height * 0.5f;
	
	renderObjectID = 0;

	collider = Collider(positionX - halfWidth, positionY - halfHeight, positionX + halfWidth, positionY + halfHeight);
}

void PixelWorldEngine::PixelObject::Move(float translationX, float translationY)
{
	if (pixelWorld == nullptr || pixelWorld->GetWorldMap() == nullptr) return;

	auto worldMap = pixelWorld->GetWorldMap();

	int worldWidth = pixelWorld->GetWorldMap()->GetWidth() - 1;
	int worldHeight = pixelWorld->GetWorldMap()->GetHeight() - 1;
	float renderObjectSize = worldMap->GetBlockSize();

	if (worldMap->Intersect(Collider::Translate(collider, translationX, 0)) == false)
		positionX = Utility::Limit(positionX + translationX, halfWidth, worldWidth * renderObjectSize + halfWidth);
	if (worldMap->Intersect(Collider::Translate(collider, 0, translationY)) == false)
		positionY = Utility::Limit(positionY + translationY, halfHeight, worldHeight * renderObjectSize + halfHeight);

	collider = Collider(positionX - halfWidth, positionY - halfHeight, positionX + halfWidth, positionY + halfHeight);
}

void PixelWorldEngine::PixelObject::SetSize(float objectWidth, float objectHeight)
{
	width = objectWidth;
	height = objectHeight;

	halfWidth = width * 0.5f;
	halfHeight = height * 0.5f;
}

void PixelWorldEngine::PixelObject::SetSize(float Size)
{
	width = Size;
	height = Size;

	halfWidth = width * 0.5f;
	halfHeight = height * 0.5f;
}

void PixelWorldEngine::PixelObject::SetPosition(float x, float y)
{
	positionX = x;
	positionY = y;
}

void PixelWorldEngine::PixelObject::SetRenderObjectID(int id)
{
	renderObjectID = id;
}

auto PixelWorldEngine::PixelObject::GetWidth() -> float
{
	return width;
}

auto PixelWorldEngine::PixelObject::GetHeight() -> float
{
	return height;
}

auto PixelWorldEngine::PixelObject::GetPositionX() -> float
{
	return positionX;
}

auto PixelWorldEngine::PixelObject::GetPositionY() -> float
{
	return positionY;
}

auto PixelWorldEngine::PixelObject::GetRenderObjectID() -> int
{
	return renderObjectID;
}

auto PixelWorldEngine::PixelObject::GetName() -> std::string
{
	return name;
}