#include "..\PixelWorldEngine\Application.hpp"
#include "..\PixelWorldEngine\DebugLayer.hpp"
#include "..\PixelWorldEngine\PixelWorld.hpp"
#include "..\PixelWorldEngine\Input.hpp"
#include "..\PixelWorldEngine\Animation.hpp"
#include "..\PixelWorldEngine\Encoding.hpp"
#include "..\PixelWorldEngine\GraphicsFont.hpp"
#include "..\PixelWorldEngine\Text.hpp"

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
		std::cout << u8"Collidde Object: " + pixelObject->GetName() << std::endl;
	}
	virtual void OnEnter(PixelObject* pixelObject)override {
		std::cout << u8"Enter Object: " + pixelObject->GetName() << std::endl;
	}
	virtual void OnLeave(PixelObject* pixelObject)override {
		std::cout << u8"Leave Object: " + pixelObject->GetName() << std::endl;
	}
	virtual void OnUpdate(float deltaTime)override {

	}
public:
	Player(std::string Name, float PositionX = 0.f, float PositionY = 0.f, float Width = 1.f, float Height = 1)
		:PixelObject(Name, PositionX, PositionY, Width, Height){}
};

int cameraWidth = 1920;
int cameraHeight = 1080;

Application application = Application(u8"Application");
PixelWorld pixelWorld = PixelWorld(u8"PixelWorld", &application);
DataManager dataManager = DataManager(&application);
TextureManager textureManager = TextureManager(&application);
MergeTexture mergeTexture = MergeTexture(&application, 640, 640, PixelFormat::R8G8B8A8);
MergeTexture mergeTexture1 = MergeTexture(&application, 640, 640, PixelFormat::A8);
WorldMap worldMap = WorldMap(u8"Map1", 100, 100);
Camera camera = Camera(PixelWorldEngine::RectangleF(-cameraWidth * 0.5f, -cameraHeight * 0.5f, cameraWidth * 0.5f, cameraHeight * 0.5f));
PixelObject pixelObject = PixelObject(u8"Player", 0, 0, 64, 64);
UIObject object = UIObject(u8"UIObject", 100, 100, 720, 720);
UIObject object2 = UIObject(u8"UIObject2", 100, 100, 100, 100);
UIObject object3 = UIObject(u8"UIObject3", 300, 100, 100, 100);
Animator animator = Animator(u8"Animator");

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
	std::string result = u8"";
	
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

	app->SetWindow((std::string)u8"TestApp Fps:" + IntToString(app->GetFramePerSecond()), 1920, 1080);

	if (Input::GetMouseButtonDown(Events::MouseButton::Left) == true) {
		auto rect = camera.GetRectangle();

		auto x = rect.left + (float)Input::GetMousePositionX() / (float)resolutionX * (rect.right - rect.left);
		auto y = rect.top + (float)Input::GetMousePositionY() / (float)resolutionY * (rect.bottom - rect.top);

		auto mapData = worldMap.GetWorldMapData(x, y);

		if (mapData != nullptr) {
			mapData->MoveEnable = false;
			mapData->RenderObjectID = 22;
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
	auto texture = dataManager.RegisterTexture(u8"T.jpg");
	auto texture1 = dataManager.RegisterTexture(u8"P.png");
	auto font = dataManager.RegisterFont("msyh.ttc", "Consola32", 32);

	auto text = new Text(u8"泥好呀，这是中文字体测试版本", font, 256, 0);
	auto text1 = new Text(u8"中文字体旋转测试。", font, 100, 0);

	auto charTexture1 = text->GetTexture();
	auto charTexture2 = text1->GetTexture();
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			textures.push_back(new Texture2D(texture, PixelWorldEngine::Rectangle(j * 64, i * 64, j * 64 + 64, i * 64 + 64)));

			mergeTexture.AddTexture(textures.size(), j * 64, i * 64, textures[textures.size() - 1]);
		}
	}

	mergeTexture.AddTexture(33, 9 * 64, 9 * 64, texture1);
	mergeTexture1.AddTexture(34, 0, 0, charTexture1);
	mergeTexture1.AddTexture(35, 256, 0, charTexture2);

	textureManager.AddMergeTexture(0, &mergeTexture);
	textureManager.AddMergeTexture(1, &mergeTexture1);

	std::uniform_int_distribution<> dis(1, textures.size()); 

	for (int i = 0; i < worldMap.GetWidth(); i++)
		for (int j = 0; j < worldMap.GetHeight(); j++) {
			auto mapData = new MapData();
			mapData->MoveEnable = true;
			mapData->RenderObjectID = 18;
			mapData->Opacity = 1.f;
			worldMap.SetMapData(i, j, mapData);
		}

	worldMap.SetMapBlockSize(64);

	camera.SetFocus(pixelObject.GetPositionX(), pixelObject.GetPositionY(),
		PixelWorldEngine::RectangleF(cameraWidth * 0.5f, cameraHeight * 0.5f, cameraWidth * 0.5f, cameraHeight * 0.5f));
	
	pixelObject.SetEffectColor(1, 0, 0);
	pixelObject.SetSize((float)charTexture1->GetWidth(), (float)charTexture1->GetHeight());
	pixelObject.SetRenderObjectID(34);
	pixelObject.Collide.push_back(OnCollide);
	pixelObject.Enter.push_back(OnEnter);
	pixelObject.Leave.push_back(OnLeave);

	object.SetPosition(0, 0);
	object.SetBorderColor(1, 0, 0);
	object.SetBorderWidth(4);
	object.SetRenderObjectID(1);
	object.SetOpacity(0.5f);
	object.SetAngle(glm::pi<float>() * 0);
	
	object2.SetBorderColor(0, 1, 0);
	object2.SetBorderWidth(1);
	object2.SetAngle(glm::pi<float>()*0.25f);

	object3.SetRenderObjectID(35);
	object3.SetAngle(glm::pi<float>()*0.25f);
	object3.SetEffectColor(0.5f, 0.5f, 0.5f);
	object3.SetSize((float)charTexture2->GetWidth(), (float)charTexture2->GetHeight());

	object.RegisterUIObject(&object2);
	object.RegisterUIObject(&object3);

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

	application.MakeWindow(u8"TestApp", 1920, 1080);
	application.SetWorld(&pixelWorld);
	application.ShowWindow();
	application.RunLoop();
}