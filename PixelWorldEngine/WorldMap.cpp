#include "WorldMap.hpp"
#include "DebugLayer.hpp"

PixelWorldEngine::MapData::MapData()
{
	RenderObjectID = 0;

	Opacity = 1.f;
}

PixelWorldEngine::WorldMap::WorldMap(std::string name, int Width, int Height, float MapBlockSize, MapData * data)
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

void PixelWorldEngine::WorldMap::SetMapData(int x, int y, MapData data)
{
	int id = y * width + x;

	DebugReturn(DebugLayer::Assert(Utility::IsLimit(id, 0, width * height - 1) == false, Error::TheValueIsNotRight, "x and y", FunctionName));

	mapData[id] = data;
}

void PixelWorldEngine::WorldMap::SetMapBlockSize(float size)
{
	DebugReturn(DebugLayer::Assert(size < 0, Error::TheValueIsNotRight, "size", FunctionName));

	mapBlockSize = size;
}

auto PixelWorldEngine::WorldMap::GetMapData(int x, int y) -> MapData 
{
	int id = y * width + x;

	DebugReturnWithValue(DebugLayer::Assert(id > (int)mapData.size(), Error::TheValueIsNotRight, "x and y", FunctionName), MapData());

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

auto PixelWorldEngine::WorldMap::GetWorldMapData(float x, float y) -> MapData 
{
	auto index = GetWorldMapDataIndex(x, y);

	if (index == WorldMap::InvalidLocation()) return MapData();

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

