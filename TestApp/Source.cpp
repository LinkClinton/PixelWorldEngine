#include "..\PixelWorldEngine\Application.hpp"
#include "..\PixelWorldEngine\DebugLayer.hpp"
#include "..\PixelWorldEngine\PixelWorld.hpp"
#include "..\PixelWorldEngine\Input.hpp"

#include <iostream>
#include <random>

using namespace PixelWorldEngine;
using namespace PixelWorldEngine::Graphics;

std::default_random_engine randomEngine(0);

#ifdef _WIN32

#ifndef _DEBUG

//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#endif // _DEBUG

#endif // !_WIN32


Application application = Application("Application");
PixelWorld pixelWorld = PixelWorld("PixelWorld", &application);
DataManager dataManger = DataManager(&application);
WorldMap worldMap = WorldMap("Map1", 100, 100);
Camera camera = Camera(PixelWorldEngine::RectangleF(-640, -360, 640, 360));
PixelObject pixelObject = PixelObject("Player", 50, 50, 100, 100);

int resolutionX = 1920;
int resolutionY = 1080;

std::vector<Texture2D*> textures;

auto IntToString(int Int) -> std::string {
	std::string result = "";
	
	while (Int > 0) {
		char c = (Int % 10) + '0';
		result = c + result;

		Int /= 10;
	}

	return result;
}

void OnMouseClick(void* sender, Events::MouseClickEvent* eventArg) {
	
}

void OnKeyEvent(void* sender, Events::KeyClickEvent* eventArg) {

}

void OnUpdate(void* sender) {
	Application* app = (Application*)sender;

	float deltaTime = app->GetDeltaTime();

	app->SetWindow((std::string)"TestApp Fps:" + IntToString(app->GetFramePerSecond()), 1920, 1080);

	if (Input::GetMouseButtonDown(Events::MouseButton::Left) == true) {
		auto rect = camera.GetRectangle();

		auto x = rect.left + (float)Input::GetMousePositionX() / (float)resolutionX * (rect.right - rect.left);
		auto y = rect.top + (float)Input::GetMousePositionY() / (float)resolutionY * (rect.bottom - rect.top);

		auto position = worldMap.GetWorldMapDataIndex(x, y);

		if (position != worldMap.InvalidLocation()) {

			worldMap.EnableMovement(false, position.first, position.second);

			auto mapData = worldMap.GetMapData(position.first, position.second);

			if (mapData != nullptr) {
				mapData->RenderObjectID[0] = 22;
			}
		}
	}

	float speed = 100 * deltaTime;
	bool isKeyDown = false;

	glm::vec2 transform(0, 0);

	if (Input::GetKeyCodeDown(KeyCode::A))
		transform.x -= speed, isKeyDown = true;
	if (Input::GetKeyCodeDown(KeyCode::D))
		transform.x += speed, isKeyDown = true;
	if (Input::GetKeyCodeDown(KeyCode::S))
		transform.y += speed, isKeyDown = true;
	if (Input::GetKeyCodeDown(KeyCode::W))
		transform.y -= speed, isKeyDown = true;

	if (isKeyDown == true) {
		pixelObject.Move(transform.x, transform.y);
		camera.SetFocus(pixelObject.GetPositionX(), pixelObject.GetPositionY(),
			PixelWorldEngine::RectangleF(640, 360, 640, 360));

	}
}

int main() {
	auto texture = dataManger.RegisterTexture("T.jpg");
	
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

			mapData->RenderObjectID[0] = 18;
			worldMap.SetMapData(i, j, mapData);
		}

	worldMap.SetBlockSize(64);

	camera.SetFocus(pixelObject.GetPositionX(), pixelObject.GetPositionY(),
		PixelWorldEngine::RectangleF(640, 360, 640, 360));

	pixelObject.SetRenderObjectID(23);

	pixelWorld.SetResolution(1920, 1080);
	pixelWorld.SetCamera(&camera);
	pixelWorld.RegisterPixelObject(&pixelObject);

	pixelWorld.SetWorldMap(&worldMap);
	
	application.KeyClick.push_back(OnKeyEvent);
	application.Update.push_back(OnUpdate);
	application.MouseClick.push_back(OnMouseClick);

	application.MakeWindow("TestApp", 1920, 1080);
	application.SetWorld(&pixelWorld);
	application.ShowWindow();
	application.RunLoop();
}