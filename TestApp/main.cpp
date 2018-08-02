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

Application* app = new Application(applicationName); //����Application

PixelWorld* world = new PixelWorld(pixelWorldName, app); //��������
WorldMap* worldMap = new WorldMap(worldMapName, mapWidth, mapHeight); //������ͼ

DataManager* dataManager = new DataManager(app); //�������ݹ�����
TextureManager* textureManager = new TextureManager(app, PixelWorldEngine::Graphics::PixelFormat::R8G8B8A8); //�������������

Camera camera = Camera(RectangleF(0, 0, 1280, 720)); //���������

PixelObject* object = new PixelObject("1");
PixelObject* objectSon1 = new PixelObject("son1");

PixelObject* targetObject = new PixelObject("TargetObject");

Graphics::Font* font = dataManager->RegisterFont("Consola.ttf", "Consola", 20);

void MakeTextureManager() {
	auto texture = dataManager->RegisterTexture(u8"MapBlock.jpg"); //��ȡ����

	int renderObjectID = 0; //��Ⱦ��ţ���������ӳ�����������������ʹ��

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 8; j++) {
			auto subTexture = new Graphics::Texture2D(texture, PixelWorldEngine::Rectangle(j * 64, i * 64, j * 64 + 64, i * 64 + 64)); //����������ԭ�������и��Ƴ�һ���ֳ���

			textureManager->RegisterTexture(++renderObjectID, subTexture); //����һ��������ӵ��ϲ�������ȥ����Ϊ�����ú���Ⱦ���

			Utility::Delete(subTexture); //�ͷ���Դ
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

	auto deltaTime = app->GetDeltaTime(); //��ȡ������ʱ��

	glm::vec2 translate = glm::vec2(0, 0); //λ��

	float speed = 200; //�ٶ�

					   //����WSAD�ƶ�
	if (Input::GetKeyCodeDown(KeyCode::A) == true)
		translate.x -= 1;
	if (Input::GetKeyCodeDown(KeyCode::D) == true)
		translate.x += 1;
	if (Input::GetKeyCodeDown(KeyCode::S) == true)
		translate.y += 1;
	if (Input::GetKeyCodeDown(KeyCode::W) == true)
		translate.y -= 1;
	
	//λ��
	if (translate != glm::vec2(0, 0)) {

		translate = glm::normalize(translate) * speed * deltaTime;

		object->Transform.SetPosition(object->Transform.GetPosition() + translate);
	}

	
}

/**
* @brief ��������
*/
void MakeWorld() {
	
	//����ͼʹ����Ⱦ���Ϊ18��������
	for (int x = 0; x < mapWidth; x++) {//��ͼ���
		for (int y = 0; y < mapHeight; y++) {//��ͼ�߶�
			auto data = MapData(); //������ͼ�����ݣ���ס����ͷ���Դ

			data.RenderObjectID = 18; //������һ���ͼ��Ⱦ��ʱ��ʹ�õ�����

			worldMap->SetMapData(x, y, data); //���úõ�ͼ����
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

	world->SetResolution(width, height); //���÷ֱ���

	world->SetCamera(&camera); //���ú������
	world->SetWorldMap(worldMap); //���úõ�ͼ

	world->SetTextureManager(textureManager); //���ú����������
}

int main() {

	MakeTextureManager(); //���������������

	MakeWorld(); //��������

	app->SetWorld(world); //��������

	app->MouseMove.push_back(OnMouseMove);
	app->Update.push_back(OnUpdate);

	app->MakeWindow(windowName, width, height); //��������
	app->ShowWindow(); //��ʾ����
	app->RunLoop(); //��ѭ��

	Utility::Delete(worldMap);
	Utility::Delete(world);
	Utility::Delete(dataManager);
	Utility::Delete(textureManager);
	Utility::Delete(app);
}