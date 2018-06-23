#include "..\PixelWorldEngine\Application.hpp"
#include "..\PixelWorldEngine\DebugLayer.hpp"
#include "..\PixelWorldEngine\PixelWorld.hpp"

#include <iostream>
#include <random>

using namespace PixelWorldEngine;
using namespace PixelWorldEngine::Graphics;

std::default_random_engine randomEngine(0);

#ifdef _WIN32

#ifndef _DEBUG

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#endif // _DEBUG

#endif // !_WIN32


Application application = Application(L"Application");
PixelWorld pixelWorld = PixelWorld(L"PixelWorld", &application);
DataManager dataManger = DataManager(&application);
WorldMap worldMap = WorldMap(L"Map1", 100, 100);
Camera camera = Camera(PixelWorldEngine::RectangleF(0, 0, 1280, 720));

std::vector<Texture2D*> textures;

void OnKeyEvent(Events::KeyClickEvent* eventArg) {
	float speed = 10;

	if (eventArg->isDown == true) {
		switch (eventArg->keyCode)
		{
		case KeyCode::A: {
			camera.Move(glm::vec2(-speed, 0));
			break;
		}
		case KeyCode::W: {
			camera.Move(glm::vec2(0, -speed));
			break;
		}
		case KeyCode::D: {
			camera.Move(glm::vec2(speed, 0));
			break;
		}
		case KeyCode::S: {
			camera.Move(glm::vec2(0, speed));
			break;
		}
		default:
			break;
		}
	}
}

int main() {
	auto texture = dataManger.RegisterTexture(L"C:/Users/LinkC/Pictures/T.jpg");
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			textures.push_back(new Texture2D(texture, PixelWorldEngine::Rectangle(j * 64, i * 64, j * 64 + 64, i * 64 + 64)));
			pixelWorld.RegisterRenderObjectID(textures.size(), textures[textures.size() - 1]);
		}
	}

	std::uniform_int_distribution<> dis(1, textures.size());

	for (int i = 0; i < worldMap.GetWidth(); i++)
		for (int j = 0; j < worldMap.GetHeight(); j++) {
			auto mapData = new MapData();
			mapData->RenderObjectID[0] = dis(randomEngine);
			worldMap.SetMapData(i, j, mapData);
		}

	pixelWorld.SetResolution(1920, 1080);
	pixelWorld.SetCamera(&camera);
	pixelWorld.SetRenderObjectSize(64);

	pixelWorld.SetWorldMap(&worldMap);
	
	application.KeyClick.push_back(OnKeyEvent);
	application.MakeWindow(L"TestApp", 1920, 1080);
	application.SetWorld(&pixelWorld);
	application.ShowWindow();
	application.RunLoop();
}