#include "WorldMap.hpp"

PixelWorldEngine::WorldMap::WorldMap(std::wstring name, int Width, int Height, MapData ** data)
{
	mapName = name;

	width = Width;
	height = Height;

	mapData.resize(width * height);

	if (data == nullptr) return;

	for (size_t i = 0; i < mapData.size(); i++)
		mapData[i] = data[i];
}

void PixelWorldEngine::WorldMap::SetMapData(int x, int y, MapData * data)
{
	mapData[y * width + x] = data;
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

auto PixelWorldEngine::WorldMap::GetMapName() -> std::wstring
{
	return mapName;
}

PixelWorldEngine::MapData::MapData(int renderObjectID)
{
	RenderObjectID = renderObjectID;
}
