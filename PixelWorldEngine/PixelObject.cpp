#include "PixelObject.hpp"
#include "PixelWorld.hpp"

auto PixelWorldEngine::PixelObject::MoveAxisXMap(float translation) -> float
{
	if (pixelWorld == nullptr) return positionX;
	if (pixelWorld->worldMap == nullptr) return positionX + translation;

	float PositionX = positionX;

	int worldWidth = pixelWorld->GetWorldMap()->GetWidth() - 1;
	int worldHeight = pixelWorld->GetWorldMap()->GetHeight() - 1;

	auto worldMap = pixelWorld->GetWorldMap();

	float mapBlockSize = worldMap->GetMapBlockSize();

	int GridTop = Utility::Limit((int)ceil((positionY + 1) / mapBlockSize) - 1, 0, worldHeight);
	int GridBottom = Utility::Limit((int)ceil((positionY + height) / mapBlockSize) - 1, 0, worldHeight);

	if (translation >= 0) {
		float originX = PositionX + width - 1;
		float targetX = PositionX + width - 1 + translation;

		int originGrid = Utility::Limit((int)ceil((originX + 1) / mapBlockSize) - 1, 0, worldWidth);
		int targetGrid = Utility::Limit((int)ceil((targetX + 1) / mapBlockSize) - 1, 0, worldWidth);

		bool moveEnable = true;

		for (int x = originGrid; x <= targetGrid; x++) {

			for (int y = GridTop; y <= GridBottom; y++) {
				if (worldMap->GetMapData(x, y)->MoveEnable == false) {
					moveEnable = false; break;
				}
			}

			if (moveEnable == false) {
				PositionX = x * mapBlockSize - width;
				break;
			}
		}

		if (moveEnable == true)
			PositionX = Utility::Min(PositionX + translation, worldWidth * mapBlockSize);
	}
	else {
		float originX = PositionX;
		float targetX = PositionX + translation;

		int originGrid = Utility::Limit((int)ceil((originX + 1) / mapBlockSize) - 1, 0, worldWidth);
		int targetGrid = Utility::Limit((int)ceil((targetX + 1) / mapBlockSize) - 1, 0, worldWidth);

		bool moveEnable = true;

		for (int x = originGrid; x >= targetGrid; x--) {

			for (int y = GridTop; y <= GridBottom; y++) {
				if (worldMap->GetMapData(x, y)->MoveEnable == false) {
					moveEnable = false; break;
				}
			}

			if (moveEnable == false) break;
		}

		if (moveEnable == true)
			PositionX = Utility::Max(PositionX + translation, 0.0f);
	}

	return PositionX;
}

auto PixelWorldEngine::PixelObject::MoveAxisYMap(float translation) -> float
{
	if (pixelWorld == nullptr) return positionY;
	if (pixelWorld->worldMap == nullptr) return positionY + translation;

	float PositionY = positionY;

	int worldWidth = pixelWorld->GetWorldMap()->GetWidth() - 1;
	int worldHeight = pixelWorld->GetWorldMap()->GetHeight() - 1;

	auto worldMap = pixelWorld->GetWorldMap();

	float mapBlockSize = worldMap->GetMapBlockSize();

	int GridLeft = Utility::Limit((int)ceil((positionX + 1) / mapBlockSize) - 1, 0, worldWidth);
	int GridRight = Utility::Limit((int)ceil((positionX + width) / mapBlockSize) - 1, 0, worldWidth);

	if (translation >= 0) {
		float originY = PositionY + height - 1;
		float targetY = PositionY + height - 1 + translation;

		int originGrid = Utility::Limit((int)ceil((originY + 1) / mapBlockSize) - 1, 0, worldHeight);
		int targetGrid = Utility::Limit((int)ceil((targetY + 1) / mapBlockSize) - 1, 0, worldHeight);

		bool moveEnable = true;

		for (int y = originGrid; y <= targetGrid; y++) {

			for (int x = GridLeft; x <= GridRight; x++) {
				if (worldMap->GetMapData(x, y)->MoveEnable == false) {
					moveEnable = false; break;
				}
			}

			if (moveEnable == false) {
				PositionY = y * mapBlockSize - height;
				break;
			}
		}

		if (moveEnable == true)
			PositionY = Utility::Min(PositionY + translation, worldHeight * mapBlockSize);
	}
	else {
		float originY = PositionY;
		float targetY = PositionY + translation;

		int originGrid = Utility::Limit((int)ceil((originY+1) / mapBlockSize) - 1, 0, worldHeight);
		int targetGrid = Utility::Limit((int)ceil((targetY +1)/ mapBlockSize) - 1, 0, worldHeight);

		bool moveEnable = true;

		for (int y = originGrid; y >= targetGrid; y--) {

			for (int x = GridLeft; x <= GridRight; x++) {
				if (worldMap->GetMapData(x, y)->MoveEnable == false) {
					moveEnable = false; break;
				}
			}

			if (moveEnable == false) break;
		}

		if (moveEnable == true)
			PositionY = Utility::Max(PositionY + translation, 0.0f);
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
	:Object(Name, PositionX, PositionY, Width, Height)
{	
	collider.SetArea(positionX, positionY, positionX + width, positionY + height);
}

void PixelWorldEngine::PixelObject::Move(float translationX, float translationY)
{
	if (pixelWorld == nullptr) return;

	float resultX = MoveAxisXMap(translationX);
	float resultY = MoveAxisYMap(translationY);

	auto targetCollider = Collider(resultX, resultY, resultX + width, resultY + height);

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


	targetCollider = Collider(resultX, positionY, resultX + width, positionY + height);

	for (auto it = pixelWorld->pixelObjects.begin(); it != pixelWorld->pixelObjects.end(); it++) {
		if (it->first == name || it->second->collider.IsEnablePhysics() == false) continue;

		if (targetCollider.Intersect(it->second->collider) == true) {
			resultX = positionX;
			break;
		}
	}

	targetCollider = Collider(positionX, resultY, positionX + width, resultY + height);

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

	collider.SetArea(positionX, positionY, positionX + width, positionY + height);

	OnMove(realTranslationX, realTranslationY);
}

void PixelWorldEngine::PixelObject::SetDepthLayer(int DepthLayer)
{
	if (depthLayer != DepthLayer && pixelWorld != nullptr) {
		depthLayer = DepthLayer;
		pixelWorld->pixelObjectLayer.erase(this);
		pixelWorld->pixelObjectLayer.insert(this);
	}

	depthLayer = DepthLayer;
}

void PixelWorldEngine::PixelObject::EnablePhysicsCollision(bool enable)
{
	collider.EnablePhysics(enable);
}

auto PixelWorldEngine::PixelObject::IsEnableCollider() -> bool
{
	return collider.IsEnablePhysics();
}
