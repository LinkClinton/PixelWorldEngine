#include <Application.hpp>
#include <Text.hpp>

using namespace PixelWorldEngine;
using namespace Graphics;

const std::string applicationName = u8"Application";
const std::string worldName = u8"PixelWorld";
const std::string windowName = u8"Text";

const std::string fontName = u8"Consola";

const int width = 1280;
const int height = 720;

const int textWidth = 200;
const int textHeight = 0;

Application* app = new Application(applicationName); //构建应用程序
PixelWorld* world = new PixelWorld(worldName, app); //构建世界
DataManager* dataManager = new DataManager(app); //构建数据管理器
TextureManager* textureManager = new TextureManager(app); //构建纹理管理器
MergeTexture* mergeTexture = new MergeTexture(app, 400, 400, PixelFormat::A8); //构建合并纹理，因为存储文字贴图

Font* font = nullptr;
Text* text = nullptr;

UIObject* object = nullptr;
UIObject* objectParent = nullptr;

Camera camera = Camera(PixelWorldEngine::RectangleF(0, 0, width, height)); //摄像机

																		   /**
																		   * @brief 构建资源
																		   */
void MakeResource() {

	font = dataManager->RegisterFont("Consola.ttf", fontName, 32); //读取字体文件

	text = new Text(u8"This is an example about render Text!", font, textWidth, textHeight); //构建文字纹理，高度设置为0的话表示由系统决定

	auto texture = text->GetTexture(); //获取纹理，纹理的管理不需要担心

	mergeTexture->AddTexture(1, 0, 0, texture); //将纹理放入合并纹理中去

	textureManager->AddMergeTexture(0, mergeTexture); //添加合并纹理到纹理管理器
}

/**
* @brief 构建物体
*/
void MakeObject() {
	object = new UIObject("UIObject");  //构建UI物体
	objectParent = new UIObject("UIObjectParent");

	float centerX = width * 0.5f; //计算中心位置
	float centerY = height * 0.5f;

	objectParent->SetSize((float)text->GetWidth() * 2, (float)text->GetWidth() * 2); //设置父亲物体的大小为文本宽度的两倍
	objectParent->SetPosition(centerX - objectParent->GetWidth() * 0.5f, centerY - objectParent->GetHeight() * 0.5f); //设置父亲物体的位置为中心
	objectParent->SetAngle(glm::pi<float>() * 0.25f); //设置旋转角度，旋转中心为物体中心，会对子物体造成影响

	objectParent->SetRenderObjectID(0); //由于不渲染纹理，因此设置为0
	objectParent->SetOpacity(0.7f); //设置不透明度，会对子物体造成影响

	objectParent->SetBorderWidth(1); //设置边框宽度
	objectParent->SetBorderColor(0, 1, 0); //设置边框颜色


	object->SetSize((float)text->GetWidth(), (float)text->GetHeight()); //设置大小和文本纹理一样大小
	object->SetPosition(objectParent->GetWidth() * 0.5f - object->GetWidth() * 0.5f, objectParent->GetHeight() * 0.5f - object->GetHeight() * 0.5f); //设置位置为中心
	object->SetAngle(-glm::pi<float>() * 0.25f); //设置旋转角度

	object->SetRenderObjectID(1); //将渲染编号设置为文本纹理的
	object->SetEffectColor(1, 0, 0); //设置文本颜色，默认为(1, 1, 1)，其本质是对纹理颜色进行一个乘法

	object->SetBorderWidth(1); //设置边框宽度，默认为0
	object->SetBorderColor(1, 0, 0); //设置边框颜色



	objectParent->RegisterUIObject(object); //注册子物体

	world->RegisterUIObject(objectParent); //注册UI物体

}

/**
* @brief 构建世界
*/
void MakeWorld() {
	world->SetResolution(width, height); //设置分辨率

	world->SetCamera(&camera); //设置摄像机，但是这个并不会影响到UI物体

	world->SetTextureManager(textureManager); //设置纹理管理器

	app->SetWorld(world); //将世界和应用程序关联
}

/**
* @brief 销毁资源释放内存
*/
void DestoryResource() {
	dataManager->UnRegisterFont(fontName); //字体要进行卸载

	Utility::Delete(text);
	Utility::Delete(object);
	Utility::Delete(objectParent);
	Utility::Delete(world);
	Utility::Delete(dataManager);
	Utility::Delete(textureManager);
	Utility::Delete(mergeTexture);
	Utility::Delete(app);
}

int main() {
	MakeResource(); //构建资源

	MakeObject(); //构建物体

	MakeWorld(); //构建世界

				 //构建窗口并显示
	app->MakeWindow(windowName, width, height);
	app->ShowWindow();

	app->RunLoop(); //主循环
}