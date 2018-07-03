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

	renderObject = new Graphics::RectangleF(0, 0, 1.f, 1.f, graphics);

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

auto PixelWorldEngine::PixelWorld::GetWorldMap() -> WorldMap *
{
	return worldMap;
}

auto PixelWorldEngine::PixelWorld::GetCurrentWorld() -> Graphics::Texture2D *
{
	renderTarget->Clear(0, 0, 0);

	graphics->ClearState();

	graphics->SetRenderTarget(renderTarget);

	graphics->SetViewPort(RectangleF(0.f, 0.f, (float)resolutionWidth, (float)resolutionHeight));

	graphics->SetShader(shader);

	graphics->SetVertexBuffer(renderObject->GetVertexBuffer());
	graphics->SetIndexBuffer(renderObject->GetIndexBuffer());


	auto viewRect = camera->GetRectangle();
	auto renderObjectRect = Rectangle();
	auto matrix = camera->GetMatrix();

	buffers[(int)BufferIndex::CameraBuffer]->Update(&matrix);

	graphics->SetStaticSampler(defaultSampler, 0);
	graphics->SetConstantBuffer(buffers[(int)BufferIndex::CameraBuffer], (int)BufferIndex::CameraBuffer);

	if (worldMap != nullptr) {
		auto mapBlockSize = worldMap->GetMapBlockSize();

		renderObjectRect.left = Utility::Max((int)(viewRect.left / mapBlockSize), 0);
		renderObjectRect.top = Utility::Max((int)(viewRect.top / mapBlockSize), 0);
		renderObjectRect.right = Utility::Min((int)(viewRect.right / mapBlockSize) + 1, worldMap->GetHeight() - 1);
		renderObjectRect.bottom = Utility::Min((int)(viewRect.bottom / mapBlockSize) + 1, worldMap->GetHeight() - 1);

		auto scaleMatrix = glm::scale(glm::mat4(1), glm::vec3(mapBlockSize, mapBlockSize, 1.f));

		for (int x = renderObjectRect.left; x <= renderObjectRect.right; x++) {

			for (int y = renderObjectRect.top; y <= renderObjectRect.bottom; y++) {
				if (worldMap->GetMapData(x, y) == nullptr) continue;

				auto matrix = glm::translate(glm::mat4(1), glm::vec3(x * mapBlockSize,
					y * mapBlockSize, 0.f)) * scaleMatrix;

				auto mapData = worldMap->GetMapData(x, y);

				memcpy(renderConfig.currentRenderObjectID, mapData->RenderObjectID, sizeof(mapData->RenderObjectID));

				buffers[(int)BufferIndex::TransformBuffer]->Update(&matrix);
				buffers[(int)BufferIndex::RenderConfig]->Update(&renderConfig);

				graphics->SetConstantBuffer(buffers[(int)BufferIndex::TransformBuffer], (int)BufferIndex::TransformBuffer);
				graphics->SetConstantBuffer(buffers[(int)BufferIndex::RenderConfig], (int)BufferIndex::RenderConfig);

				for (int id = 0; id < MAX_RENDER_OBJECT; id++) {
					if (mapData->RenderObjectID[id] == 0) continue;
					graphics->SetShaderResource(renderObjectIDGroup[mapData->RenderObjectID[id]], id);
				}

				graphics->DrawIndexed(renderObject->GetIndexBuffer()->GetCount());
			}
		}
	}

	memset(renderConfig.currentRenderObjectID, 0, sizeof(renderConfig.currentRenderObjectID));

	for (auto it = pixelObjectLayer.begin(); it != pixelObjectLayer.end(); it++) {
		auto pixelObject = *it;

		if (pixelObject->renderObjectID == 0) continue;

		auto matrix = glm::translate(glm::mat4(1), glm::vec3(pixelObject->positionX - pixelObject->halfWidth,
			pixelObject->positionY - pixelObject->halfHeight, 0.f));

		matrix = glm::scale(matrix, glm::vec3(pixelObject->width, pixelObject->height, 1.f));

		renderConfig.currentRenderObjectID[0] = pixelObject->renderObjectID;

		buffers[(int)BufferIndex::TransformBuffer]->Update(&matrix);
		buffers[(int)BufferIndex::RenderConfig]->Update(&renderConfig);

		graphics->SetConstantBuffer(buffers[(int)BufferIndex::TransformBuffer], (int)BufferIndex::TransformBuffer);
		graphics->SetConstantBuffer(buffers[(int)BufferIndex::RenderConfig], (int)BufferIndex::RenderConfig);

		graphics->SetShaderResource(renderObjectIDGroup[pixelObject->renderObjectID], 0);

		graphics->DrawIndexed(renderObject->GetIndexBuffer()->GetCount());
	}

	return renderBuffer;
}




