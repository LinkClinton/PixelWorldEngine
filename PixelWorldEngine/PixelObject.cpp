#include "PixelObject.hpp"
#include "PixelWorld.hpp"

auto PixelWorldEngine::PixelObject::MoveAxisXMap(float translation) -> float
{
	if (pixelWorld == nullptr || pixelWorld->GetWorldMap() == nullptr) return positionX;

	float PositionX = positionX;

	int worldWidth = pixelWorld->GetWorldMap()->GetWidth() - 1;
	int worldHeight = pixelWorld->GetWorldMap()->GetHeight() - 1;

	auto worldMap = pixelWorld->GetWorldMap();

	float mapBlockSize = worldMap->GetMapBlockSize();

	int GridTop = Utility::Limit((int)ceil((positionY - halfHeight + 1) / mapBlockSize) - 1, 0, worldHeight);
	int GridBottom = Utility::Limit((int)ceil((positionY + halfHeight + 1) / mapBlockSize) - 1, 0, worldHeight);

	if (translation >= 0) {
		float originX = PositionX + halfWidth + 1;
		float targetX = PositionX + translation + halfWidth + 1;

		int originGrid = Utility::Limit((int)ceil(originX / mapBlockSize) - 1, 0, worldWidth);
		int targetGrid = Utility::Limit((int)ceil(targetX / mapBlockSize) - 1, 0, worldWidth);

		bool moveEnable = true;

		for (int x = originGrid; x <= targetGrid; x++) {

			for (int y = GridTop; y <= GridBottom; y++) {
				if (worldMap->GetMapData(x, y)->MoveEnable == false) {
					moveEnable = false; break;
				}
			}

			if (moveEnable == false) {
				PositionX = x * mapBlockSize - halfWidth - 1;
				break;
			}
		}

		if (moveEnable == true)
			PositionX = Utility::Min(PositionX + translation, worldWidth * mapBlockSize + halfWidth);
	}
	else {
		float originX = PositionX - halfWidth;
		float targetX = PositionX + translation - halfWidth;

		int originGrid = Utility::Limit((int)ceil(originX / mapBlockSize) - 1, 0, worldWidth);
		int targetGrid = Utility::Limit((int)ceil(targetX / mapBlockSize) - 1, 0, worldWidth);

		bool moveEnable = true;

		for (int x = originGrid; x >= targetGrid; x--) {

			for (int y = GridTop; y <= GridBottom; y++) {
				if (worldMap->GetMapData(x, y)->MoveEnable == false) {
					moveEnable = false; break;
				}
			}

			if (moveEnable == false) {
				PositionX = x * mapBlockSize + mapBlockSize + halfWidth;
				break;
			}
		}

		if (moveEnable == true)
			PositionX = Utility::Max(PositionX + translation, halfWidth);
	}

	return PositionX;
}

auto PixelWorldEngine::PixelObject::MoveAxisYMap(float translation) -> float
{
	if (pixelWorld == nullptr || pixelWorld->GetWorldMap() == nullptr) return positionY;

	float PositionY = positionY;

	int worldWidth = pixelWorld->GetWorldMap()->GetWidth() - 1;
	int worldHeight = pixelWorld->GetWorldMap()->GetHeight() - 1;

	auto worldMap = pixelWorld->GetWorldMap();

	float mapBlockSize = worldMap->GetMapBlockSize();

	int GridLeft = Utility::Limit((int)ceil((positionX - halfWidth + 1) / mapBlockSize) - 1, 0, worldWidth);
	int GridRight = Utility::Limit((int)ceil((positionX + halfWidth + 1) / mapBlockSize) - 1, 0, worldWidth);

	if (translation >= 0) {
		float originY = PositionY + halfHeight + 1;
		float targetY = PositionY + translation + halfHeight + 1;

		int originGrid = Utility::Limit((int)ceil(originY / mapBlockSize) - 1, 0, worldHeight);
		int targetGrid = Utility::Limit((int)ceil(targetY / mapBlockSize) - 1, 0, worldHeight);

		bool moveEnable = true;

		for (int y = originGrid; y <= targetGrid; y++) {

			for (int x = GridLeft; x <= GridRight; x++) {
				if (worldMap->GetMapData(x, y)->MoveEnable == false) {
					moveEnable = false; break;
				}
			}

			if (moveEnable == false) {
				PositionY = y * mapBlockSize - halfHeight - 1;
				break;
			}
		}

		if (moveEnable == true)
			PositionY = Utility::Min(PositionY + translation, worldHeight * mapBlockSize + halfHeight);
	}
	else {
		float originY = PositionY - halfHeight;
		float targetY = PositionY + translation - halfHeight;

		int originGrid = Utility::Limit((int)ceil(originY / mapBlockSize) - 1, 0, worldHeight);
		int targetGrid = Utility::Limit((int)ceil(targetY / mapBlockSize) - 1, 0, worldHeight);

		bool moveEnable = true;

		for (int y = originGrid; y >= targetGrid; y--) {

			for (int x = GridLeft; x <= GridRight; x++) {
				if (worldMap->GetMapData(x, y)->MoveEnable == false) {
					moveEnable = false; break;
				}
			}

			if (moveEnable == false) {
				PositionY = y * mapBlockSize + mapBlockSize + halfHeight;
				break;
			}
		}

		if (moveEnable == true)
			PositionY = Utility::Max(PositionY + translation, halfHeight);
	}

	return PositionY;
}

void PixelWorldEngine::PixelObject::OnUpdate(float deltaTime)
{
}

void PixelWorldEngine::PixelObject::OnMove(float translationX, float translationY)
{
}

void PixelWorldEngine::PixelObject::OnCollide(PixelObject* which)
{
}

void PixelWorldEngine::PixelObject::OnEnter(PixelObject* pixelObject)
{
}

void PixelWorldEngine::PixelObject::OnLeave(PixelObject* pixelObject)
{
}

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

	collider.SetArea(positionX - halfWidth, positionY - halfHeight, positionX + halfWidth, positionY + halfHeight);
}

void PixelWorldEngine::PixelObject::Move(float translationX, float translationY)
{
	if (pixelWorld == nullptr) return;

	float resultX = MoveAxisXMap(translationX);
	float resultY = MoveAxisYMap(translationY);

	auto targetCollider = Collider(resultX - halfWidth, resultY - halfHeight, resultX + halfWidth, resultY + halfHeight);

	for (auto it = pixelWorld->pixelObjects.begin(); it != pixelWorld->pixelObjects.end(); it++) {
		if (it->first == name) continue;

		if (it->second->collider.IsEnablePhysics() == true) {
			if (targetCollider.Intersect(it->second->collider) == true)
				OnCollide(it->second), Events::DoEventHandlers(Collide, it->second);
		}
		else {
			bool originState = collider.Intersect(it->second->collider);
			bool targetState = targetCollider.Intersect(it->second->collider);

			if (originState == true && targetState == false)
				OnLeave(it->second), Events::DoEventHandlers(Leave, it->second);

			if (originState == false && targetState == true)
				OnEnter(it->second), Events::DoEventHandlers(Enter, it->second);
		}
	}


	targetCollider = Collider(resultX - halfWidth, positionY - halfHeight, resultX + halfWidth, positionY + halfHeight);

	for (auto it = pixelWorld->pixelObjects.begin(); it != pixelWorld->pixelObjects.end(); it++) {
		if (it->first == name || it->second->collider.IsEnablePhysics() == false) continue;

		if (targetCollider.Intersect(it->second->collider) == true) {
			resultX = positionX;
			break;
		}
	}

	targetCollider = Collider(positionX - halfWidth, resultY - halfHeight, positionX + halfWidth, resultY + halfHeight);

	for (auto it = pixelWorld->pixelObjects.begin(); it != pixelWorld->pixelObjects.end(); it++) {
		if (it->first == name || it->second->collider.IsEnablePhysics() == false) continue;

		if (targetCollider.Intersect(it->second->collider) == true) {
			resultY = positionY;
			break;
		}
	}

	auto realTranslationX = resultX - positionX;
	auto realTranslationY = resultY - positionY;

	positionX = resultX;
	positionY = resultY;

	collider.SetArea(positionX - halfWidth, positionY - halfHeight, positionX + halfWidth, positionY + halfHeight);

	OnMove(realTranslationX, realTranslationY);
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

void PixelWorldEngine::PixelObject::EnableCollider(bool enable)
{
	collider.EnablePhysics(enable);
}

auto PixelWorldEngine::PixelObject::GetWidth() -> float
{
	return width;
}

auto PixelWorldEngine::PixelObject::IsEnableCollider() -> bool
{
	return collider.IsEnablePhysics();
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