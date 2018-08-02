#include <Application.hpp>
#include <PixelWorld.hpp>
#include <Input.hpp>

#include <cstring>

using namespace PixelWorldEngine;

const std::string applicationName = u8"Application";
const std::string pixelWorldName = u8"PixelWorld";
const std::string windowName = u8"PixelWorld";
const std::string worldMapName = u8"WorldMap";

const int width = 1920;
const int height = 1080;

const int mapWidth = 100;
const int mapHeight = 100;

Application* app = new Application(applicationName); //构建Application

PixelWorld* world = new PixelWorld(pixelWorldName, app); //构建世界
WorldMap* worldMap = new WorldMap(worldMapName, mapWidth, mapHeight); //构建地图

DataManager* dataManager = new DataManager(app); //构建数据管理器
TextureManager* textureManager = new TextureManager(app, PixelWorldEngine::Graphics::PixelFormat::R8G8B8A8); //构建纹理管理器

Camera camera = Camera(RectangleF(0, 0, 1280, 720)); //构建摄像机

PixelObject* object = new PixelObject("1");
PixelObject* objectSon1 = new PixelObject("son1");

PixelObject* targetObject = new PixelObject("TargetObject");

Graphics::Font* font = dataManager->RegisterFont("Consola.ttf", "Consola", 20);

void MakeTextureManager() {
	auto texture = dataManager->RegisterTexture(u8"MapBlock.jpg"); //读取纹理

	int renderObjectID = 0; //渲染编号，即将纹理映射成整数，方便我们使用

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			auto subTexture = new Graphics::Texture2D(texture, PixelWorldEngine::Rectangle(j * 64, i * 64, j * 64 + 64, i * 64 + 64)); //创建纹理，从原有纹理中复制出一部分出来

			textureManager->RegisterTexture(++renderObjectID, subTexture); //将这一块纹理添加到合并纹理中去，并为其设置好渲染编号

			Utility::Delete(subTexture); //释放资源
		}
	}
}

void OnCollide(PixelObject* A, PixelObject* B) {
	//std::cout << A->GetName() << " Collide " << B->GetName() << std::endl;
}

void OnEnter(void* sender) {
	auto object = (PixelObject*)sender;

	//std::cout << "MouseEnter: " + object->GetName() << std::endl;
}

void OnLeave(void* sender) {
	auto object = (PixelObject*)sender;

	//std::cout << "MouseLeave: " + object->GetName() << std::endl;
}

void OnMouseMove(void* sender, Events::MouseMoveEvent* e) {
	
	//auto position = Utility::ConvertPosition(&camera, glm::vec2((float)e->x / width, (float)e->y / height));

	//object.Transform.SetPosition(position);
}

void OnUpdate(void* sender) {
	app->SetWindow(windowName + " FPS: " + Utility::ToString(app->GetFramePerSecond()), width, height);

	//object.Transform.SetRotate(object.Transform.GetRotate() + glm::pi<float>() * app->GetDeltaTime() * 0.2f);

	auto deltaTime = app->GetDeltaTime(); //获取经过的时间

	glm::vec2 translate = glm::vec2(0, 0); //位移

	float speed = 200; //速度

					   //根据WSAD移动
	if (Input::GetKeyCodeDown(KeyCode::A) == true)
		translate.x -= 1;
	if (Input::GetKeyCodeDown(KeyCode::D) == true)
		translate.x += 1;
	if (Input::GetKeyCodeDown(KeyCode::S) == true)
		translate.y += 1;
	if (Input::GetKeyCodeDown(KeyCode::W) == true)
		translate.y -= 1;
	
	//位移
	if (translate != glm::vec2(0, 0)) {

		translate = glm::normalize(translate) * speed * deltaTime;

		object->Transform.SetPosition(object->Transform.GetPosition() + translate);
	}

	
}

/**
* @brief 构建世界
*/
void MakeWorld() {
	
	//将地图使用渲染编号为18的纹理覆盖
	for (int x = 0; x < mapWidth; x++) {//地图宽度
		for (int y = 0; y < mapHeight; y++) {//地图高度
			auto data = MapData(); //创建地图块数据，记住最后释放资源

			data.RenderObjectID = 18; //设置这一块地图渲染的时候使用的纹理

			worldMap->SetMapData(x, y, data); //设置好地图数据
		}
	}

	object->RenderObjectID = 2;
	object->SetSize(100, 100);
	object->Opacity = 0.7f;
	object->Transform.SetPosition(glm::vec2(0, 0));
	//object.Transform.SetForward(glm::vec2(1, 1));
	
	objectSon1->RenderObjectID = 3;
	objectSon1->Transform.SetPosition(glm::vec2(10, 10));
	objectSon1->SetSize(50, 50);
	
	targetObject->RenderObjectID = 4;
	targetObject->SetSize(200, 150);
	targetObject->Opacity = 1.0f;
	targetObject->Transform.SetPosition(glm::vec2(200, 200));
	targetObject->Font = font;
	targetObject->Text = "cccc\ncccc";
	targetObject->TextColor = glm::vec3(1, 0, 0);
	//targetObject->Transform.SetRotate(glm::pi<float>() * 0.25f);

	object->ObjectCollide.push_back(OnCollide);

	object->SetChild(objectSon1);

	world->SetPixelObject(object, Layer::World);
	world->SetPixelObject(targetObject, Layer::World);

	world->SetResolution(width, height); //设置分辨率

	world->SetCamera(&camera); //设置好摄像机
	world->SetWorldMap(worldMap); //设置好地图

	world->SetTextureManager(textureManager); //设置好纹理管理器
}

int main() {

	MakeTextureManager(); //构建好纹理管理器

	MakeWorld(); //构建世界

	app->SetWorld(world); //设置世界

	app->MouseMove.push_back(OnMouseMove);
	app->Update.push_back(OnUpdate);

	app->MakeWindow(windowName, width, height); //创建窗口
	app->ShowWindow(); //显示窗口
	app->RunLoop(); //主循环

	Utility::Delete(worldMap);
	Utility::Delete(world);
	Utility::Delete(dataManager);
	Utility::Delete(textureManager);
	Utility::Delete(app);
}