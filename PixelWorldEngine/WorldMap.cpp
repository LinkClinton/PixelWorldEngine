#include "WorldMap.hpp"

PixelWorldEngine::WorldMap::WorldMap(std::string name, int Width, int Height, float MapBlockSize, MapData ** data)
{
	mapName = name;

	width = Width;
	height = Height;

	mapBlockSize = MapBlockSize;

	mapData.resize(width * height);

	if (data == nullptr) return;

	for (size_t i = 0; i < mapData.size(); i++)
		mapData[i] = data[i];
}

void PixelWorldEngine::WorldMap::SetMapData(int x, int y, MapData * data)
{
	mapData[y * width + x] = data;
}

void PixelWorldEngine::WorldMap::SetMapBlockSize(float size)
{
	mapBlockSize = size;
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

auto PixelWorldEngine::WorldMap::GetMapBlockSize() -> float
{
	return mapBlockSize;
}

auto PixelWorldEngine::WorldMap::GetWorldMapDataIndex(float x, float y) -> std::pair<int, int>
{
	auto result = std::pair<int, int>();

	result.first = (int)(x / mapBlockSize);
	result.second = (int)(y / mapBlockSize);

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

auto PixelWorldEngine::WorldMap::GetMapName() -> std::string
{
	return mapName;
}

auto PixelWorldEngine::WorldMap::InvalidLocation() -> std::pair<int, int>
{
	return std::pair<int, int>(-1, -1);
}

