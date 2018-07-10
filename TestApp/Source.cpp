#include "..\PixelWorldEngine\Application.hpp"
#include "..\PixelWorldEngine\DebugLayer.hpp"
#include "..\PixelWorldEngine\PixelWorld.hpp"
#include "..\PixelWorldEngine\Input.hpp"
#include "..\PixelWorldEngine\Animation.hpp"

#include <iostream>
#include <random>

using namespace PixelWorldEngine;
using namespace PixelWorldEngine::Graphics;

std::default_random_engine randomEngine(0);

#ifdef _WIN32

#ifndef _DEBUG

//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#endif // _DEBUG

#endif // _WIN32

class Player :public PixelObject {
protected:
	virtual void OnCollide(PixelObject* pixelObject)override {
		std::cout << "Collidde Object: " + pixelObject->GetName() << std::endl;
	}
	virtual void OnEnter(PixelObject* pixelObject)override {
		std::cout << "Enter Object: " + pixelObject->GetName() << std::endl;
	}
	virtual void OnLeave(PixelObject* pixelObject)override {
		std::cout << "Leave Object: " + pixelObject->GetName() << std::endl;
	}
	virtual void OnUpdate(float deltaTime)override {

	}
public:
	Player(std::string Name, float PositionX = 0.f, float PositionY = 0.f, float Width = 1.f, float Height = 1)
		:PixelObject(Name, PositionX, PositionY, Width, Height){}
};

int cameraWidth = 1280;
int cameraHeight = 720;

Application application = Application("Application");
PixelWorld pixelWorld = PixelWorld("PixelWorld", &application);
DataManager dataManager = DataManager(&application);
TextureManager textureManager = TextureManager(&application, 64, 64);
WorldMap worldMap = WorldMap("Map1", 100, 100);
Camera camera = Camera(PixelWorldEngine::RectangleF(-cameraWidth * 0.5f, -cameraHeight * 0.5f, cameraWidth * 0.5f, cameraHeight * 0.5f));
PixelObject pixelObject = PixelObject("Player", 32, 32, 64, 64);
UIObject object = UIObject("UIObject", 100, 100, 720, 720);
UIObject object2 = UIObject("UIObject2", 100, 100, 100, 100);
Animator animator = Animator("Animator");

void OnCollide(PixelObject* pixelObject) {
	
}
void OnEnter(PixelObject* pixelObject){
	pixelObject->SetOpacity(0.5f);
}
void OnLeave(PixelObject* pixelObject) {
	pixelObject->SetOpacity(1.f);
}

void SetRenderObjectID(void* Which, void* Data) {
	auto which = (PixelObject*)Which;
	auto data = *(int*)Data;

	which->SetRenderObjectID(data);
}

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
	auto object = (UIObject*)sender;

	
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

		auto mapData = worldMap.GetWorldMapData(x, y);

		if (mapData != nullptr) {
			mapData->MoveEnable = false;
			mapData->RenderObjectID[0] = 22;
		}
	}

	float speed = 200 * deltaTime;
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
			PixelWorldEngine::RectangleF(cameraWidth * 0.5f, cameraHeight * 0.5f, cameraWidth * 0.5f, cameraHeight * 0.5f));

	}
}

int main() {
	auto texture = dataManager.RegisterTexture("T.jpg");
	auto texture1 = dataManager.RegisterTexture("P.png");

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			textures.push_back(new Texture2D(texture, PixelWorldEngine::Rectangle(j * 64, i * 64, j * 64 + 64, i * 64 + 64)));

			textureManager.AddTexture(textures.size(), textures[textures.size() - 1]);
		}
	}

	textureManager.AddTexture(33, texture1);

	textureManager.MergeTextures();

	std::uniform_int_distribution<> dis(1, textures.size()); 

	for (int i = 0; i < worldMap.GetWidth(); i++)
		for (int j = 0; j < worldMap.GetHeight(); j++) {
			auto mapData = new MapData();
			mapData->MoveEnable = true;
			mapData->RenderObjectID[0] = 18;
			mapData->Opacity = 1.f;
			worldMap.SetMapData(i, j, mapData);
		}

	worldMap.SetMapBlockSize(64);

	camera.SetFocus(pixelObject.GetPositionX(), pixelObject.GetPositionY(),
		PixelWorldEngine::RectangleF(cameraWidth * 0.5f, cameraHeight * 0.5f, cameraWidth * 0.5f, cameraHeight * 0.5f));

	pixelObject.SetRenderObjectID(33);
	pixelObject.Collide.push_back(OnCollide);
	pixelObject.Enter.push_back(OnEnter);
	pixelObject.Leave.push_back(OnLeave);

	object.SetPosition(0, 0);
	object.SetBorderColor(1, 0, 0);
	object.SetBorderWidth(4);
	object.SetRenderObjectID(1);
	object.SetOpacity(0.5f);
	object.SetAngle(glm::pi<float>());
	
	object2.SetBorderColor(0, 1, 0);
	object2.SetBorderWidth(1);
	object2.SetAngle(glm::pi<float>()*0.25f);

	object.RegisterUIObject(&object2);

	pixelWorld.SetTextureManager(&textureManager);

	pixelWorld.SetResolution(1920, 1080);
	pixelWorld.SetCamera(&camera);

	pixelWorld.RegisterUIObject(&object);


	pixelWorld.RegisterPixelObject(&pixelObject);

	for (int i = 5; i <= 15; i++) {
		auto object = new PixelObject(IntToString(i), i * 50.0f, i * 50.0f, 50, 50);
		object->EnablePhysicsCollision(false);
		object->SetDepthLayer(0);
		object->SetRenderObjectID(20);
		pixelWorld.RegisterPixelObject(object);
	}

	pixelWorld.SetWorldMap(&worldMap);
	
	application.Update.push_back(OnUpdate);

	application.RegisterAnimator(&animator);

	application.MakeWindow("TestApp", 1920, 1080);
	application.SetWorld(&pixelWorld);
	application.ShowWindow();
	application.RunLoop();
}