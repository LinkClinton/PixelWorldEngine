#include "PixelWorld.hpp"

#include "Application.hpp"
#include "EngineDefaultResource.hpp"

PixelWorldEngine::PixelWorld::PixelWorld(std::string WorldName, Application * Application)
{
	worldName = WorldName;
	graphics = Application->GetGraphics();

	renderObjectSize = 32;

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

void PixelWorldEngine::PixelWorld::SetRenderObjectSize(float size)
{
	renderObjectSize = size;
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

	pixelObject->pixelWorld = this;
}

void PixelWorldEngine::PixelWorld::UnRegisterPixelObject(PixelObject * pixelObject)
{
	pixelObjects.erase(pixelObject->name);

	pixelObject->pixelWorld = nullptr;
}

void PixelWorldEngine::PixelWorld::UnRegisterPixelObject(std::string objectName)
{
	pixelObjects[objectName]->pixelWorld = nullptr;

	pixelObjects.erase(objectName);
}

auto PixelWorldEngine::PixelWorld::GetRenderObjectSize() -> float
{
	return renderObjectSize;
}

auto PixelWorldEngine::PixelWorld::GetWorldMap() -> WorldMap *
{
	return worldMap;
}

auto PixelWorldEngine::PixelWorld::GetWorldMapDataIndex(float x, float y) -> std::pair<int, int>
{
	auto result = std::pair<int, int>();

	result.first = (int)(x / renderObjectSize);
	result.second = (int)(y / renderObjectSize);

	if (Utility::IsLimit(result.first, 0, worldMap->GetWidth() - 1) == true &&
		Utility::IsLimit(result.second, 0, worldMap->GetHeight() - 1) == true)
		return result;
	else return std::pair<int, int>(-1, -1);
}

auto PixelWorldEngine::PixelWorld::GetWorldMapData(float x, float y) -> MapData *
{
	if (worldMap == nullptr) return nullptr;

	auto index = GetWorldMapDataIndex(x, y);

	if (index == WorldMap::InvalidLocation()) return nullptr;

	return worldMap->GetMapData(index.first, index.second);
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

	if (worldMap == nullptr) return renderBuffer;

	auto viewRect = camera->GetRectangle();
	auto renderObjectRect = Rectangle();
	auto matrix = camera->GetMatrix();

	buffers[(int)BufferIndex::CameraBuffer]->Update(&matrix);

	renderObjectRect.left = Utility::Max((int)(viewRect.left / renderObjectSize), 0);
	renderObjectRect.top = Utility::Max((int)(viewRect.top / renderObjectSize), 0);
	renderObjectRect.right = Utility::Min((int)(viewRect.right / renderObjectSize) + 1, worldMap->GetHeight() - 1);
	renderObjectRect.bottom = Utility::Min((int)(viewRect.bottom / renderObjectSize) + 1, worldMap->GetHeight() - 1);
	
	graphics->SetStaticSampler(defaultSampler, 0);
	graphics->SetConstantBuffer(buffers[(int)BufferIndex::CameraBuffer], (int)BufferIndex::CameraBuffer);
	
	auto scaleMatrix = glm::scale(glm::mat4(1), glm::vec3(renderObjectSize, renderObjectSize, 1.f));

	for (int x = renderObjectRect.left; x <= renderObjectRect.right; x++) {

		for (int y = renderObjectRect.top; y <= renderObjectRect.bottom; y++) {
			if (worldMap->GetMapData(x, y) == nullptr) continue;

			auto matrix = glm::translate(glm::mat4(1), glm::vec3(x * renderObjectSize,
				y * renderObjectSize, 0.f)) * scaleMatrix;

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

	memset(renderConfig.currentRenderObjectID, 0, sizeof(renderConfig.currentRenderObjectID));

	for (auto it = pixelObjects.begin(); it != pixelObjects.end(); it++) {
		auto pixelObject = it->second;

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





