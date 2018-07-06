#include "PixelWorld.hpp"

#include "Application.hpp"
#include "EngineDefaultResource.hpp"

void PixelWorldEngine::PixelWorld::OnUpdate(float deltaTime)
{
	for (auto it = pixelObjects.begin(); it != pixelObjects.end(); it++) {
		it->second->OnUpdate(deltaTime);
		Events::DoEventHandlers(it->second->Update, deltaTime);
	}
}

PixelWorldEngine::PixelWorld::PixelWorld(std::string WorldName, Application * Application)
{
	worldName = WorldName;
	graphics = Application->GetGraphics();

	renderObject = new Graphics::RectangleF(-0.5f, -0.5f, 0.5f, 0.5f, graphics);
	
	buffers.resize((int)BufferIndex::Count);

	auto matrix = glm::mat4(1);

	buffers[(int)BufferIndex::CameraBuffer] = new Graphics::Buffer(graphics, &matrix , sizeof(glm::mat4x4));
	buffers[(int)BufferIndex::TransformBuffer] = new Graphics::Buffer(graphics, &matrix, sizeof(glm::mat4x4));
	buffers[(int)BufferIndex::RenderConfig] = new Graphics::Buffer(graphics, &renderConfig, sizeof(PixelWorldRenderConfig));

	defaultShader = new Graphics::GraphicsShader(graphics, 
		Utility::CharArrayToVector((char*)vsPixelWorldDefaultShaderCode),
		Utility::CharArrayToVector((char*)psPixelWorldDefaultShaderCode));
	
	defaultSampler = new Graphics::StaticSampler(graphics);

	worldMap = nullptr;

	SetShader();

	SetBackGroundColor(0, 0, 0, 1);
}

PixelWorldEngine::PixelWorld::~PixelWorld()
{
	for (size_t i = 0; i < buffers.size(); i++)
		Utility::Delete(buffers[i]);

	Utility::Delete(defaultShader);
	Utility::Delete(renderBuffer);
	Utility::Delete(renderTarget);
	Utility::Delete(renderCanvas);
}

void PixelWorldEngine::PixelWorld::SetResolution(int width, int height)
{
	if (width == resolutionWidth || height == resolutionHeight) return;

	UICamera.SetRectangle(PixelWorldEngine::RectangleF(0, 0, (float)width, (float)height));

	Utility::Delete(renderBuffer);
	Utility::Delete(renderTarget);
	Utility::Delete(renderCanvas);

	resolutionWidth = width;
	resolutionHeight = height;

	renderBuffer = new Graphics::Texture2D(graphics, nullptr, resolutionWidth, resolutionHeight,
		Graphics::PixelFormat::R8G8B8A8);

	renderTarget = new Graphics::RenderTarget(graphics, renderBuffer);

	renderCanvas = new Graphics::RectangleF(0, 0, (float)width, (float)height, graphics);
}

void PixelWorldEngine::PixelWorld::SetBackGroundColor(float red, float green, float blue, float alpha)
{
	backGroundColor[0] = red;
	backGroundColor[1] = green;
	backGroundColor[2] = blue;
	backGroundColor[3] = alpha;
}

void PixelWorldEngine::PixelWorld::SetCamera(Camera* Camera)
{
	camera = Camera;
}

void PixelWorldEngine::PixelWorld::SetShader(Graphics::GraphicsShader * Shader)
{
	shader = Shader;
}

void PixelWorldEngine::PixelWorld::SetShader()
{
	shader = defaultShader;
}

void PixelWorldEngine::PixelWorld::SetWorldMap(std::string worldMapName)
{
	worldMap = worldMaps[worldMapName];
}

void PixelWorldEngine::PixelWorld::SetWorldMap(WorldMap * WorldMap)
{
	if (worldMaps[WorldMap->GetMapName()] == nullptr)
		RegisterWorldMap(WorldMap);

	worldMap = WorldMap;
}

void PixelWorldEngine::PixelWorld::RegisterRenderObjectID(int id, Graphics::Texture2D* texture)
{
	renderObjectIDGroup.insert(std::pair<int, Graphics::Texture2D*>(id, texture));
}

void PixelWorldEngine::PixelWorld::UnRegisterRenderObjectID(int id)
{
	renderObjectIDGroup.erase(id);
}

void PixelWorldEngine::PixelWorld::RegisterWorldMap(WorldMap * worldMap)
{
	worldMaps[worldMap->GetMapName()] = worldMap;
}

void PixelWorldEngine::PixelWorld::RegisterPixelObject(PixelObject * pixelObject)
{
	if (pixelObject->pixelWorld != nullptr) 
		pixelObject->pixelWorld->UnRegisterPixelObject(pixelObject);

	pixelObjects[pixelObject->name] = pixelObject;
	pixelObjectLayer.insert(pixelObject);
	
	pixelObject->pixelWorld = this;
}

void PixelWorldEngine::PixelWorld::UnRegisterPixelObject(PixelObject * pixelObject)
{
	pixelObjects.erase(pixelObject->name);
	pixelObjectLayer.erase(pixelObject);

	pixelObject->pixelWorld = nullptr;
}

void PixelWorldEngine::PixelWorld::UnRegisterPixelObject(std::string objectName)
{
	pixelObjects[objectName]->pixelWorld = nullptr;

	pixelObjectLayer.erase(pixelObjects[objectName]);
	pixelObjects.erase(objectName);
}

void PixelWorldEngine::PixelWorld::RegisterUIObject(UIObject * object)
{
	UIObjects[object->name] = object;
	UIObjectLayer.insert(object);
}

void PixelWorldEngine::PixelWorld::UnRegisterUIObject(UIObject * object)
{
	UIObjects.erase(object->name);
	UIObjectLayer.erase(object);
}

void PixelWorldEngine::PixelWorld::UnRegisterUIObject(std::string name)
{
	UIObjectLayer.erase(UIObjects[name]);
	UIObjects.erase(name);
}

auto PixelWorldEngine::PixelWorld::GetWorldMap() -> WorldMap *
{
	return worldMap;
}

void PixelWorldEngine::PixelWorld::RenderWorldMap()
{
	auto viewRect = camera->GetRectangle();
	auto renderObjectRect = Rectangle();

	if (worldMap != nullptr) {
		auto mapBlockSize = worldMap->GetMapBlockSize();
		auto halfMapBlockSize = mapBlockSize * 0.5f;

		renderObjectRect.left = Utility::Max((int)(viewRect.left / mapBlockSize), 0);
		renderObjectRect.top = Utility::Max((int)(viewRect.top / mapBlockSize), 0);
		renderObjectRect.right = Utility::Min((int)(viewRect.right / mapBlockSize) + 1, worldMap->GetHeight() - 1);
		renderObjectRect.bottom = Utility::Min((int)(viewRect.bottom / mapBlockSize) + 1, worldMap->GetHeight() - 1);

		auto scaleMatrix = glm::scale(glm::mat4(1), glm::vec3(mapBlockSize, mapBlockSize, 1.f));

		for (int x = renderObjectRect.left; x <= renderObjectRect.right; x++) {

			for (int y = renderObjectRect.top; y <= renderObjectRect.bottom; y++) {
				if (worldMap->GetMapData(x, y) == nullptr) continue;

				auto matrix = glm::translate(glm::mat4(1), glm::vec3(x * mapBlockSize + halfMapBlockSize,
					y * mapBlockSize + halfMapBlockSize, 0.f)) * scaleMatrix;

				auto mapData = worldMap->GetMapData(x, y);

				memcpy(renderConfig.renderObjectID, mapData->RenderObjectID, sizeof(mapData->RenderObjectID));
				renderConfig.renderColor = glm::vec4(backGroundColor[0], backGroundColor[1], backGroundColor[2], mapData->Opacity);

				buffers[(int)BufferIndex::TransformBuffer]->Update(&matrix);
				buffers[(int)BufferIndex::RenderConfig]->Update(&renderConfig);

				graphics->SetConstantBuffer(buffers[(int)BufferIndex::TransformBuffer], (int)BufferIndex::TransformBuffer);
				graphics->SetConstantBuffer(buffers[(int)BufferIndex::RenderConfig], (int)BufferIndex::RenderConfig);

				for (int id = 0; id < MAX_RENDER_OBJECT; id++) {
					if (mapData->RenderObjectID[id] == 0) continue;
					graphics->SetShaderResource(renderObjectIDGroup[mapData->RenderObjectID[id]], id);
				}

				graphics->DrawIndexed(renderObject->GetIndexBuffer()->GetCount(), 0, 0);
			}
		}
	}
}

void PixelWorldEngine::PixelWorld::RenderPixelObjects()
{
	memset(renderConfig.renderObjectID, 0, sizeof(renderConfig.renderObjectID));
	renderConfig.renderColor = glm::vec4(backGroundColor[0], backGroundColor[1], backGroundColor[2], 1);

	for (auto it = pixelObjectLayer.begin(); it != pixelObjectLayer.end(); it++) {
		auto pixelObject = *it;

		if (pixelObject->renderObjectID == 0) continue;

		auto matrix = glm::translate(glm::mat4(1), glm::vec3(pixelObject->positionX,
			pixelObject->positionY, 0.f));

		matrix = glm::scale(matrix, glm::vec3(pixelObject->width, pixelObject->height, 1.f));

		renderConfig.renderObjectID[0] = pixelObject->renderObjectID;
		renderConfig.renderColor.a = pixelObject->opacity;

		buffers[(int)BufferIndex::TransformBuffer]->Update(&matrix);
		buffers[(int)BufferIndex::RenderConfig]->Update(&renderConfig);

		graphics->SetConstantBuffer(buffers[(int)BufferIndex::TransformBuffer], (int)BufferIndex::TransformBuffer);
		graphics->SetConstantBuffer(buffers[(int)BufferIndex::RenderConfig], (int)BufferIndex::RenderConfig);

		graphics->SetShaderResource(renderObjectIDGroup[pixelObject->renderObjectID], 0);

		graphics->DrawIndexed(renderObject->GetIndexBuffer()->GetCount(), 0, 0);
	}
}

void PixelWorldEngine::PixelWorld::RenderUIObject(glm::mat4x4 baseTranslation, UIObject* object)
{
	auto halfWidth = object->width * 0.5f;
	auto halfHeight = object->height * 0.5f;

	auto positionX = object->positionX + halfWidth;
	auto positionY = object->positionY + halfHeight;

	auto halfBorderWidth = object->borderWidth * 0.5f;

	if (object->borderWidth != 0.0f) {

		renderConfig.renderColor.r = object->borderColor[0];
		renderConfig.renderColor.g = object->borderColor[1];
		renderConfig.renderColor.b = object->borderColor[2];
		renderConfig.renderColor.a = object->opacity;
		renderConfig.renderObjectID[0] = 0;

		buffers[(int)BufferIndex::RenderConfig]->Update(&renderConfig);

		graphics->SetConstantBuffer(buffers[(int)BufferIndex::RenderConfig], (int)BufferIndex::RenderConfig);

		auto widthScaleMatrix = glm::scale(glm::mat4(1), glm::vec3(object->width, object->borderWidth, 1.0f));
		auto heightScaleMatrix = glm::scale(glm::mat4(1), glm::vec3(object->borderWidth, object->height, 1.0f));

		glm::mat4x4 borderMatrix[4];

		borderMatrix[0] = glm::translate(baseTranslation, glm::vec3(positionX, object->positionY + halfBorderWidth , 0))
			* widthScaleMatrix;
		borderMatrix[1] = glm::translate(baseTranslation, glm::vec3(object->positionX + halfBorderWidth, positionY, 0))
			* heightScaleMatrix;
		borderMatrix[2] = glm::translate(baseTranslation, glm::vec3(positionX, object->positionY - halfBorderWidth + object->height + 1 , 0))
			* widthScaleMatrix;
		borderMatrix[3] = glm::translate(baseTranslation, glm::vec3(object->positionX - halfBorderWidth + object->width + 1 , positionY, 0))
			* heightScaleMatrix;

		for (int i = 0; i < 4; i++) {
			buffers[(int)BufferIndex::TransformBuffer]->Update(&borderMatrix[i]);

			graphics->SetConstantBuffer(buffers[(int)BufferIndex::TransformBuffer], (int)BufferIndex::TransformBuffer);

			graphics->DrawIndexed(renderObject->GetIndexBuffer()->GetCount(), 0, 0);
		}
	}

	if (object->renderObjectID != 0) {

		renderConfig.renderObjectID[0] = object->renderObjectID;

		auto matrix = glm::translate(baseTranslation, glm::vec3(positionX + object->borderWidth, positionY + object->borderWidth, 0));
		matrix = glm::scale(matrix, glm::vec3(object->width - object->borderWidth * 2.0f, object->height - object->borderWidth * 2.0f, 1.0f));

		buffers[(int)BufferIndex::TransformBuffer]->Update(&matrix);
		buffers[(int)BufferIndex::RenderConfig]->Update(&renderConfig);

		graphics->SetConstantBuffer(buffers[(int)BufferIndex::TransformBuffer], (int)BufferIndex::TransformBuffer);
		graphics->SetConstantBuffer(buffers[(int)BufferIndex::RenderConfig], (int)BufferIndex::RenderConfig);

		graphics->DrawIndexed(renderObject->GetIndexBuffer()->GetCount(), 0, 0);
	}

	for (auto it = object->children.begin(); it != object->children.end(); it++)
		RenderUIObject(glm::translate(baseTranslation, glm::vec3(object->positionX, object->positionY, 0.0f)), it->second);
}

void PixelWorldEngine::PixelWorld::RenderUIObjects()
{
	auto cameraMatrix = UICamera.GetMatrix();

	buffers[(int)BufferIndex::CameraBuffer]->Update(&cameraMatrix);

	graphics->SetConstantBuffer(buffers[(int)BufferIndex::CameraBuffer], (int)BufferIndex::CameraBuffer);

	memset(renderConfig.renderObjectID, 0, sizeof(renderConfig.renderObjectID));
	renderConfig.renderColor = glm::vec4(backGroundColor[0], backGroundColor[1], backGroundColor[2], 1);

	for (auto it = UIObjectLayer.begin(); it != UIObjectLayer.end(); it++) {
		auto object = *it;

		RenderUIObject(glm::mat4(1), object);
	}
}

auto PixelWorldEngine::PixelWorld::GetCurrentWorld() -> Graphics::Texture2D *
{
	renderTarget->Clear(backGroundColor[0], backGroundColor[1], backGroundColor[2], backGroundColor[3]);

	graphics->ClearState();

	graphics->SetRenderTarget(renderTarget);

	graphics->SetViewPort(RectangleF(0.f, 0.f, (float)resolutionWidth, (float)resolutionHeight));

	graphics->SetShader(shader);

	graphics->SetVertexBuffer(renderObject->GetVertexBuffer());
	graphics->SetIndexBuffer(renderObject->GetIndexBuffer());
	
	auto matrix = camera->GetMatrix();

	buffers[(int)BufferIndex::CameraBuffer]->Update(&matrix);

	graphics->SetStaticSampler(defaultSampler, 0);
	graphics->SetConstantBuffer(buffers[(int)BufferIndex::CameraBuffer], (int)BufferIndex::CameraBuffer);

	graphics->SetBlendState(true);

	RenderWorldMap();

	RenderPixelObjects();

	RenderUIObjects();

	return renderBuffer;
}




