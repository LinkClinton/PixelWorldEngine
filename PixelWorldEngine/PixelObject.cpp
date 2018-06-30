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
}

PixelWorldEngine::PixelObject::PixelObject(std::string Name, float PositionX, float PositionY, float Size)
{
	name = Name;

	positionX = PositionX;
	positionY = PositionY;

	width = Size;
	height = Size;

	halfWidth = width * 0.5f;
	halfHeight = height * 0.5f;

	renderObjectID = 0;
}

void PixelWorldEngine::PixelObject::MoveAxisX(float translation)
{
	if (pixelWorld == nullptr || pixelWorld->GetWorldMap() == nullptr) return;

	int worldWidth = pixelWorld->GetWorldMap()->GetWidth() - 1;
	int worldHeight = pixelWorld->GetWorldMap()->GetHeight() - 1;

	auto worldMap = pixelWorld->GetWorldMap();

	float renderObjectSize = pixelWorld->GetRenderObjectSize();

	int GridTop = Utility::Limit((int)floor((positionY - halfHeight) / renderObjectSize), 0, worldHeight);
	int GridBottom = Utility::Limit((int)ceil((positionY + halfHeight) / renderObjectSize), 0, worldHeight);

	if (translation >= 0) {
		float originX = positionX + halfWidth;
		float targetX = positionX + translation + halfWidth;

		int originGrid = Utility::Limit((int)floor(originX / renderObjectSize), 0, worldWidth);
		int targetGrid = Utility::Limit((int)ceil(targetX / renderObjectSize), 0, worldWidth);

		bool moveEnable = true;

		for (int x = originGrid; x <= targetGrid; x++) {

			for (int y = GridTop; y <= GridBottom; y++) {
				if (worldMap->GetMapData(x, y)->MoveEnable == false) {
					moveEnable = false; break;
				}
			}

			if (moveEnable == false) {
				positionX = x * renderObjectSize - halfWidth;
				break;
			}
		}

		if (moveEnable == true)
			positionX = Utility::Min(positionX + translation, worldWidth * renderObjectSize + halfWidth);
	}
	else {
		float originX = positionX - halfWidth;
		float targetX = positionX + translation - halfWidth;

		int originGrid = Utility::Limit((int)ceil(originX / renderObjectSize), 0, worldWidth);
		int targetGrid = Utility::Limit((int)floor(targetX / renderObjectSize), 0, worldWidth);

		bool moveEnable = true;

		for (int x = originGrid; x >= targetGrid; x--) {

			for (int y = GridTop; y <= GridBottom; y++) {
				if (worldMap->GetMapData(x, y)->MoveEnable == false) {
					moveEnable = false; break;
				}
			}

			if (moveEnable == false) {
				positionX = x * renderObjectSize + renderObjectSize + halfWidth;
				break;
			}
		}

		if (moveEnable == true)
			positionX = Utility::Max(positionX + translation, 0.f);
	}
}

void PixelWorldEngine::PixelObject::MoveAxisY(float translation)
{
	if (pixelWorld == nullptr || pixelWorld->GetWorldMap() == nullptr) return;

	int worldWidth = pixelWorld->GetWorldMap()->GetWidth() - 1;
	int worldHeight = pixelWorld->GetWorldMap()->GetHeight() - 1;

	auto worldMap = pixelWorld->GetWorldMap();

	float renderObjectSize = pixelWorld->GetRenderObjectSize();

	int GridLeft = Utility::Limit((int)floor((positionX - halfWidth) / renderObjectSize), 0, worldWidth);
	int GridRight = Utility::Limit((int)ceil((positionX + halfWidth) / renderObjectSize), 0, worldWidth);

	if (translation >= 0) {
		float originY = positionY + halfHeight;
		float targetY = positionY + translation + halfHeight;

		int originGrid = Utility::Limit((int)floor(originY / renderObjectSize), 0, worldHeight);
		int targetGrid = Utility::Limit((int)ceil(targetY / renderObjectSize), 0, worldHeight);

		bool moveEnable = true;

		for (int y = originGrid; y <= targetGrid; y++) {

			for (int x = GridLeft; x <= GridRight; x++) {
				if (worldMap->GetMapData(x, y)->MoveEnable == false) {
					moveEnable = false; break;
				}
			}

			if (moveEnable == false) {
				positionY = y * renderObjectSize - halfHeight;
				break;
			}
		}

		if (moveEnable == true)
			Utility::Min(positionY + translation, worldHeight * renderObjectSize + halfHeight);
	}
	else {
		float originY = positionY - halfHeight;
		float targetY = positionY + translation - halfHeight;

		int originGrid = Utility::Limit((int)ceil(originY / renderObjectSize), 0, worldHeight);
		int targetGrid = Utility::Limit((int)floor(targetY / renderObjectSize), 0, worldHeight);

		bool moveEnable = true;

		for (int y = originGrid; y >= targetGrid; y--) {

			for (int x = GridLeft; x <= GridRight; x++) {
				if (worldMap->GetMapData(x, y)->MoveEnable == false) {
					moveEnable = false; break;
				}
			}

			if (moveEnable == false) {
				positionY = y * renderObjectSize + renderObjectSize + halfHeight;
				break;
			}
		}

		if (moveEnable == true)
			positionY = Utility::Min(positionY + translation, 0.f);
	}
}

void PixelWorldEngine::PixelObject::Move(float translationX, float translationY)
{
	MoveAxisX(translationX);
	MoveAxisY(translationY);
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