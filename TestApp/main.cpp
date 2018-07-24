#include <Application.hpp>
#include <PixelWorld.hpp>

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

PixelObject object = PixelObject("1");

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

	object.RenderObjectID = 2;

	world->SetPixelObject(&object, PixelObjectLayer::WorldLayer);

	world->SetResolution(width, height); //���÷ֱ���

	world->SetCamera(&camera); //���ú������
	world->SetWorldMap(worldMap); //���úõ�ͼ

	world->SetTextureManager(textureManager); //���ú����������
}

int main() {

	MakeTextureManager(); //���������������

	MakeWorld(); //��������

	app->SetWorld(world); //��������

	app->MakeWindow(windowName, width, height); //��������
	app->ShowWindow(); //��ʾ����
	app->RunLoop(); //��ѭ��

	Utility::Delete(worldMap);
	Utility::Delete(world);
	Utility::Delete(dataManager);
	Utility::Delete(textureManager);
	Utility::Delete(app);
}