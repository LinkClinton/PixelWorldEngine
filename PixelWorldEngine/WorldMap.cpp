#include "WorldMap.hpp"

PixelWorldEngine::WorldMap::WorldMap(std::string name, int Width, int Height, float BlockSize, MapData ** data)
{
	mapName = name;

	width = Width;
	height = Height;

	blockSize = BlockSize;

	mapData.resize(width * height);

	if (data == nullptr) return;

	for (size_t i = 0; i < mapData.size(); i++)
		mapData[i] = data[i];
}

void PixelWorldEngine::WorldMap::SetMapData(int x, int y, MapData * data)
{
	mapData[y * width + x] = data;
}

void PixelWorldEngine::WorldMap::SetBlockSize(float size)
{
	blockSize = size;

	for (auto it = colliders.begin(); it != colliders.end(); it++) {
		auto position = it->first;

		it->second = Collider(
			position.first * blockSize, position.second * blockSize,
			position.first * blockSize + blockSize - 1, position.second * blockSize + blockSize - 1);
	}
}

void PixelWorldEngine::WorldMap::EnableMovement(bool enable, int x, int y)
{
	auto position = std::pair<int, int>(x, y);

	if (enable == true && colliders.count(position) != 0)
		colliders.erase(position);

	if (enable == false)
		colliders[position] = Collider(
			position.first * blockSize, position.second * blockSize,
			position.first * blockSize + blockSize - 1, position.second * blockSize + blockSize - 1);
}

auto PixelWorldEngine::WorldMap::GetMapData(int x, int y) -> MapData *
{
	return mapData[y * width + x];
}

auto PixelWorldEngine::WorldMap::GetWidth() -> int
{
	return width;
}

auto PixelWorldEngine::WorldMap::GetHeight() -> int
{
	return height;
}

auto PixelWorldEngine::WorldMap::GetBlockSize() -> float
{
	return blockSize;
}

auto PixelWorldEngine::WorldMap::GetWorldMapDataIndex(float x, float y) -> std::pair<int, int>
{
	auto result = std::pair<int, int>();

	result.first = (int)(x / blockSize);
	result.second = (int)(y / blockSize);

	if (Utility::IsLimit(result.first, 0, width - 1) == true &&
		Utility::IsLimit(result.second, 0, height - 1) == true)
		return result;
	else return std::pair<int, int>(-1, -1);
}

auto PixelWorldEngine::WorldMap::GetWorldMapData(float x, float y) -> MapData *
{
	auto index = GetWorldMapDataIndex(x, y);

	if (index == WorldMap::InvalidLocation()) return nullptr;

	return GetMapData(index.first, index.second);
}

auto PixelWorldEngine::WorldMap::Intersect(Collider collider) -> bool
{
	for (auto it = colliders.begin(); it != colliders.end(); it++) 
		if (it->second.Intersect(collider) == true) return true;
	return false;
}

auto PixelWorldEngine::WorldMap::GetMapName() -> std::string
{
	return mapName;
}

auto PixelWorldEngine::WorldMap::InvalidLocation() -> std::pair<int, int>
{
	return std::pair<int, int>(-1, -1);
}

