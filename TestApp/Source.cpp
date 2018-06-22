#include "..\PixelWorldEngine\Application.hpp"
#include "..\PixelWorldEngine\DebugLayer.hpp"
#include "..\PixelWorldEngine\PixelWorld.hpp"

using namespace PixelWorldEngine;
using namespace PixelWorldEngine::Graphics;

#ifdef _WIN32

#ifndef _DEBUG

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#endif // _DEBUG

#endif // !_WIN32


Application application = Application(L"Application");
PixelWorld pixelWorld = PixelWorld(L"PixelWorld", &application);
DataManager dataManger = DataManager(&application);
WorldMap worldMap = WorldMap(L"Map1", 100, 100);

int main() {
	auto texture = dataManger.RegisterTexture(L"C:/Users/LinkC/Pictures/T1.jpg");

	for (int i = 0; i < worldMap.GetWidth(); i++)
		for (int j = 0; j < worldMap.GetHeight(); j++) {
			auto mapData = new MapData();
			mapData->RenderObjectID[0] = 1;
			worldMap.SetMapData(i, j, mapData);
		}

	pixelWorld.SetResolution(1920, 1080);
	pixelWorld.SetCamera(Camera(PixelWorldEngine::RectangleF(0, 0, 1280, 720)));
	pixelWorld.SetRenderObjectSize(32);
	pixelWorld.RegisterRenderObjectID(1, texture);

	pixelWorld.SetWorldMap(&worldMap);

	application.MakeWindow(L"TestApp", 1920, 1080);
	application.SetWorld(&pixelWorld);
	application.ShowWindow();
	application.RunLoop();
}