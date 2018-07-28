#include "PixelWorld.hpp"

#include "Application.hpp"
#include "EngineDefaultResource.hpp"

void PixelWorldEngine::PixelWorld::OnMouseMove(void * sender, Events::MouseMoveEvent * eventArg)
{
	Camera* usedCamera[(int)PixelObjectLayer::Count];

	usedCamera[(int)PixelObjectLayer::WorldLayer] = camera;
	usedCamera[(int)PixelObjectLayer::UILayer] = &UICamera;

	for (size_t i = 0; i < layerRoots.size(); i++) {
		if (camera == nullptr) continue;

		auto arg = Events::MouseMoveEvent(*eventArg);
		auto position = Utility::ConvertPosition(usedCamera[i],
			glm::vec2(eventArg->x / (float)resolutionWidth, (float)eventArg->y / resolutionHeight));

		arg.x = (int)position.x;
		arg.y = (int)position.y;

		Internal::PixelObjectProcess::ProcessMouseMove(layerRoots[i], &arg, glm::mat4(1));
	}
}

void PixelWorldEngine::PixelWorld::OnMouseClick(void * sender, Events::MouseClickEvent * eventArg)
{
	Camera* usedCamera[(int)PixelObjectLayer::Count];

	usedCamera[(int)PixelObjectLayer::WorldLayer] = camera;
	usedCamera[(int)PixelObjectLayer::UILayer] = &UICamera;

	for (size_t i = 0; i < layerRoots.size(); i++) {
		if (camera == nullptr) continue;

		auto arg = Events::MouseClickEvent(*eventArg);
		auto position = Utility::ConvertPosition(usedCamera[i],
			glm::vec2(eventArg->x / (float)resolutionWidth, (float)eventArg->y / resolutionHeight));

		arg.x = (int)position.x;
		arg.y = (int)position.y;

		Internal::PixelObjectProcess::ProcessMouseClick(layerRoots[i], &arg, glm::mat4(1));
	}
}

void PixelWorldEngine::PixelWorld::OnMouseWheel(void * sender, Events::MouseWheelEvent * eventArg)
{
	Camera* usedCamera[(int)PixelObjectLayer::Count];

	usedCamera[(int)PixelObjectLayer::WorldLayer] = camera;
	usedCamera[(int)PixelObjectLayer::UILayer] = &UICamera;

	for (size_t i = 0; i < layerRoots.size(); i++) {
		if (camera == nullptr) continue;

		auto arg = Events::MouseWheelEvent(*eventArg);
		auto position = Utility::ConvertPosition(usedCamera[i],
			glm::vec2(eventArg->x / (float)resolutionWidth, (float)eventArg->y / resolutionHeight));

		arg.x = (int)position.x;
		arg.y = (int)position.y;

		Internal::PixelObjectProcess::ProcessMouseWheel(layerRoots[i], &arg, glm::mat4(1));
	}
}

void PixelWorldEngine::PixelWorld::OnKeyClick(void * sender, Events::KeyClickEvent * eventArg)
{
	for (auto it = layerRoots.begin(); it != layerRoots.end(); it++)
		Internal::PixelObjectProcess::ProcessKeyClick(*it, eventArg);
}

void PixelWorldEngine::PixelWorld::OnUpdate(float deltaTime)
{
	for (auto it = layerRoots.begin(); it != layerRoots.end(); it++)
		Internal::PixelObjectProcess::ProcessUpdate(*it);

	collideSolver.SolveCollide(layerRoots[(int)PixelObjectLayer::WorldLayer]);
}

PixelWorldEngine::PixelWorld::PixelWorld(std::string WorldName, Application * Application)
{
	DebugReturn(DebugLayer::Assert(Application == nullptr, Error::TheObjectIsNull, "Application", FunctionName));

	worldName = WorldName;
	graphics = Application->GetGraphics();

	renderObject = new Graphics::RectangleF(-0.5f, -0.5f, 0.5f, 0.5f, graphics);
	
	buffers.resize((int)BufferIndex::Count);
	bufferArrays.resize((int)BufferArrayIndex::Count);
	layerRoots.resize((int)PixelObjectLayer::Count);

	auto matrix = glm::mat4(1);

	buffers[(int)BufferIndex::CameraBuffer] = new Graphics::Buffer(graphics, &matrix , sizeof(glm::mat4x4));
	buffers[(int)BufferIndex::RenderConfig] = new Graphics::Buffer(graphics, &renderConfig, sizeof(PixelWorldRenderConfig));

	if (graphics->GetGraphicsMode() == Graphics::GraphicsMode::Low)
		buffers[(int)BufferIndex::LowInstanceData] = new Graphics::Buffer(graphics, nullptr, sizeof(InstanceData) * 100);
	

	if (graphics->GetGraphicsMode() == Graphics::GraphicsMode::High) {
		bufferArrays[(int)BufferArrayIndex::WorldMapInstanceData] = new Graphics::BufferArray(graphics, nullptr, sizeof(InstanceData));
		bufferArrays[(int)BufferArrayIndex::PixelObjectInstanceData] = new Graphics::BufferArray(graphics, nullptr, sizeof(InstanceData));
		bufferArrays[(int)BufferArrayIndex::UIObjectInstanceData] = new Graphics::BufferArray(graphics, nullptr, sizeof(InstanceData));
	}

	layerRoots[(int)PixelObjectLayer::WorldLayer] = new PixelObject("WorldLayer");
	layerRoots[(int)PixelObjectLayer::UILayer] = new PixelObject("UILayer");

	if (graphics->GetGraphicsMode() == Graphics::GraphicsMode::High) {
		defaultShader = new Graphics::GraphicsShader(graphics,
			Utility::CharArrayToVector((char*)vsPixelWorldDefaultShaderCode),
			Utility::CharArrayToVector((char*)psPixelWorldDefaultShaderCode));
	}
	else {
		defaultShader = new Graphics::GraphicsShader(graphics,
			Utility::CharArrayToVector((char*)lowVsPixelWorldDefaultShaderCode),
			Utility::CharArrayToVector((char*)lowPsPixelWorldDefaultShaderCode));
	}

	defaultSampler = new Graphics::StaticSampler(graphics, Graphics::TextureAddressMode::Clamp,
		Graphics::TextureFilter::Anisotropic);

	textureManager = new TextureManager(Application);

	worldMap = nullptr;
	textureManager = nullptr;

	SetShader();

	SetBackGroundColor(0, 0, 0, 1);
}

PixelWorldEngine::PixelWorld::~PixelWorld()
{
	for (size_t i = 0; i < buffers.size(); i++)
		Utility::Delete(buffers[i]);

	for (size_t i = 0; i < bufferArrays.size(); i++)
		Utility::Delete(bufferArrays[i]);

	for (size_t i = 0; i < layerRoots.size(); i++)
		Utility::Delete(layerRoots[i]);

	Utility::Delete(defaultShader);
	Utility::Delete(renderBuffer);
	Utility::Delete(renderTarget);
	Utility::Delete(renderCanvas);
}

void PixelWorldEngine::PixelWorld::SetResolution(int width, int height, float ssaa)
{
	DebugReturn(DebugLayer::Assert(width <= 0 || height <= 0, Error::WidthOrHeightLessThanZero, worldName, FunctionName));
	DebugReturn(DebugLayer::Assert(ssaa <= 0, Error::TheValueIsNotRight, "ssaa", FunctionName));

	if (width == resolutionWidth && height == resolutionHeight && ssaaLevel == ssaa) return;

	UICamera.SetRectangle(PixelWorldEngine::RectangleF(0, 0, (float)width, (float)height));

	Utility::Delete(renderBuffer);
	Utility::Delete(renderTarget);
	Utility::Delete(renderCanvas);

	resolutionWidth = width;
	resolutionHeight = height;
	ssaaLevel = ssaa;

	renderBuffer = new Graphics::Texture2D(graphics, nullptr, (int)(resolutionWidth * ssaaLevel), (int)(resolutionHeight * ssaaLevel),
		Graphics::PixelFormat::R8G8B8A8);

	renderTarget = new Graphics::RenderTarget(graphics, renderBuffer);

	renderCanvas = new Graphics::RectangleF(0, 0, (float)resolutionWidth * ssaaLevel, (float)resolutionHeight * ssaaLevel, graphics);
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

void PixelWorldEngine::PixelWorld::SetWorldMap(WorldMap * WorldMap)
{
	DebugReturn(DebugLayer::Assert(WorldMap == nullptr, Error::TheObjectIsNull, "WorldMap", FunctionName));

	worldMap = WorldMap;
}

void PixelWorldEngine::PixelWorld::SetTextureManager(TextureManager * TextureManager)
{
	textureManager = TextureManager;
}

void PixelWorldEngine::PixelWorld::SetPixelObject(PixelObject * pixelObject, PixelObjectLayer layer)
{
	layerRoots[(int)layer]->SetChild(pixelObject);
}

void PixelWorldEngine::PixelWorld::CancelPixelObject(std::string name, PixelObjectLayer layer)
{
	layerRoots[(int)layer]->CancelChild(name);
}

auto PixelWorldEngine::PixelWorld::GetWorldMap() -> WorldMap *
{
	return worldMap;
}

auto PixelWorldEngine::PixelWorld::GetPixelObject(std::string name, PixelObjectLayer layer) -> PixelObject*
{
	return layerRoots[(int)layer]->GetChildren(name);
}

void PixelWorldEngine::PixelWorld::LowDrawObject(std::vector<InstanceData>* instanceData)
{
	if (instanceData->size() == LOW_MAX_INSTANCE_DATA) {
		buffers[(int)BufferIndex::LowInstanceData]->Update(&((*instanceData)[0]));

		graphics->SetConstantBuffer(buffers[(int)BufferIndex::LowInstanceData], (int)BufferIndex::LowInstanceData);

		graphics->DrawIndexedInstanced(renderObject->GetIndexBuffer()->GetCount(), instanceData->size(), 0, 0);
	}
	else {
		std::vector<InstanceData> newInstanceData;
		
		newInstanceData.resize(LOW_MAX_INSTANCE_DATA);

		for (size_t i = 0; i < instanceData->size(); i++)
			newInstanceData[i] = (*instanceData)[i];

		buffers[(int)BufferIndex::LowInstanceData]->Update(&newInstanceData[0]);

		graphics->SetConstantBuffer(buffers[(int)BufferIndex::LowInstanceData], (int)BufferIndex::LowInstanceData);

		graphics->DrawIndexedInstanced(renderObject->GetIndexBuffer()->GetCount(), newInstanceData.size(), 0, 0);
	}

	instanceData->clear();
}

void PixelWorldEngine::PixelWorld::HighDrawObject(std::vector<InstanceData>* instanceData, BufferArrayIndex arrayIndex)
{
	if (bufferArrays[(int)arrayIndex]->GetCount() != instanceData->size()) {

		Utility::Delete(bufferArrays[(int)arrayIndex]);

		bufferArrays[(int)arrayIndex] =
			new Graphics::BufferArray(graphics, &((*instanceData)[0]), sizeof(InstanceData) * instanceData->size(), instanceData->size());
	}
	else bufferArrays[(int)arrayIndex]->Update(&((*instanceData)[0]));

	graphics->SetShaderResource(bufferArrays[(int)arrayIndex], 0);

	graphics->DrawIndexedInstanced(renderObject->GetIndexBuffer()->GetCount(), instanceData->size(), 0, 0);
}

void PixelWorldEngine::PixelWorld::RenderWorldMap()
{
	if (camera == nullptr) return;

	auto matrix = camera->GetMatrix();
	auto viewRect = camera->GetRectangle();
	auto renderObjectRect = Rectangle();

	buffers[(int)BufferIndex::CameraBuffer]->Update(&matrix);

	graphics->SetConstantBuffer(buffers[(int)BufferIndex::CameraBuffer], (int)BufferIndex::CameraBuffer);

	if (worldMap != nullptr) {
		auto mapBlockSize = worldMap->GetMapBlockSize();
		
		renderObjectRect.left = Utility::Max((int)(viewRect.left / mapBlockSize), 0);
		renderObjectRect.top = Utility::Max((int)(viewRect.top / mapBlockSize), 0);
		renderObjectRect.right = Utility::Min((int)(viewRect.right / mapBlockSize) + 1, worldMap->GetWidth() - 1);
		renderObjectRect.bottom = Utility::Min((int)(viewRect.bottom / mapBlockSize) + 1, worldMap->GetHeight() - 1);

		auto scaleMatrix = glm::scale(glm::mat4(1), glm::vec3(mapBlockSize, mapBlockSize, 1.f));

		std::vector<InstanceData> instanceData;

		for (int x = renderObjectRect.left; x <= renderObjectRect.right; x++) {

			for (int y = renderObjectRect.top; y <= renderObjectRect.bottom; y++) {
				InstanceData data;

				auto mapData = worldMap->GetMapData(x, y);
				auto arrayIndex = textureManager->GetArrayIndex(mapData.RenderObjectID);

				if (mapData.RenderObjectID == 0) continue;

				data.worldTransform = glm::translate(glm::mat4(1), glm::vec3(x * mapBlockSize + mapBlockSize * 0.5f,
					y * mapBlockSize + mapBlockSize * 0.5f, 0.f)) * scaleMatrix;
				data.texcoordTransform = textureManager->GetTexCoordTransform(mapData.RenderObjectID);
				data.setting[0] = mapData.RenderObjectID;
				data.setting[1] = arrayIndex;
				data.renderCoor = glm::vec4(1.0f, 1.0f, 1.0f, mapData.Opacity);

				instanceData.push_back(data);

				if (graphics->GetGraphicsMode() == Graphics::GraphicsMode::Low && instanceData.size() == LOW_MAX_INSTANCE_DATA)
					LowDrawObject(&instanceData);
			}
		}

		if (instanceData.size() != 0) {
			if (graphics->GetGraphicsMode() == Graphics::GraphicsMode::High)
				HighDrawObject(&instanceData, BufferArrayIndex::WorldMapInstanceData);
			else LowDrawObject(&instanceData);
		}
	}
}

void PixelWorldEngine::PixelWorld::RenderPixelObject(glm::mat4x4 baseTransformMatrix, float baseOpacity, PixelObject * pixelObject,
	std::vector<InstanceData>* instanceData, Camera* camera)
{
	//如果摄像机无法看见，那么就跳过

	auto transformMatrix = baseTransformMatrix * pixelObject->Transform.GetMatrix();
	auto opacity = baseOpacity * pixelObject->Opacity;

	if (pixelObject->RenderObjectID != 0) {
		InstanceData data;

		auto matrix = glm::scale(transformMatrix, glm::vec3(pixelObject->width, pixelObject->height, 1.0f));

		int arrayIndex = textureManager->GetArrayIndex(pixelObject->RenderObjectID);

		data.renderCoor = glm::vec4(1, 1, 1, opacity);
		data.setting[0] = pixelObject->RenderObjectID;
		data.setting[1] = arrayIndex;
		data.worldTransform = matrix;
		data.texcoordTransform = textureManager->GetTexCoordTransform(pixelObject->RenderObjectID);
		
		instanceData->push_back(data);

		if (graphics->GetGraphicsMode() == Graphics::GraphicsMode::Low && instanceData->size() == LOW_MAX_INSTANCE_DATA)
			LowDrawObject(instanceData);
	}

	for (auto it = pixelObject->childrenDepthSort.begin(); it != pixelObject->childrenDepthSort.end(); it++)
		RenderPixelObject(transformMatrix, opacity, *it, instanceData, camera);
}

void PixelWorldEngine::PixelWorld::RenderPixelObjects()
{
	std::vector<Camera*> usedCamera((int)PixelObjectLayer::Count);
	std::vector<BufferArrayIndex> arrayIndex((int)PixelObjectLayer::Count);

	usedCamera[(int)PixelObjectLayer::WorldLayer] = camera;
	usedCamera[(int)PixelObjectLayer::UILayer] = &UICamera;

	arrayIndex[(int)PixelObjectLayer::WorldLayer] = BufferArrayIndex::PixelObjectInstanceData;
	arrayIndex[(int)PixelObjectLayer::UILayer] = BufferArrayIndex::UIObjectInstanceData;

	for (size_t i = 0; i < layerRoots.size(); i++) {
		std::vector<InstanceData> instanceData;

		if (usedCamera[i] == nullptr) continue;

		auto cameraMatrix = usedCamera[i]->GetMatrix();

		buffers[(int)BufferIndex::CameraBuffer]->Update(&cameraMatrix);

		graphics->SetConstantBuffer(buffers[(int)BufferIndex::CameraBuffer], (int)BufferIndex::CameraBuffer);

		RenderPixelObject(glm::mat4(1), 1.0f, layerRoots[i], &instanceData, usedCamera[i]);

		if (instanceData.size() != 0) {
			if (graphics->GetGraphicsMode() == Graphics::GraphicsMode::High)
				HighDrawObject(&instanceData, arrayIndex[i]);
			else LowDrawObject(&instanceData);
		}
	}
}

auto PixelWorldEngine::PixelWorld::GetCurrentWorld() -> Graphics::Texture2D *
{
	renderTarget->Clear(backGroundColor[0], backGroundColor[1], backGroundColor[2], backGroundColor[3]);

	graphics->ClearState();

	graphics->SetRenderTarget(renderTarget);

	graphics->SetViewPort(RectangleF(0.f, 0.f, (float)resolutionWidth * ssaaLevel, (float)resolutionHeight * ssaaLevel));

	graphics->SetShader(shader);

	graphics->SetVertexBuffer(renderObject->GetVertexBuffer());
	graphics->SetIndexBuffer(renderObject->GetIndexBuffer());

	graphics->SetBlendState(true);

	graphics->SetStaticSampler(defaultSampler, 0);

	graphics->SetShaderResource(textureManager->GetTextureArray(), 1);
	
	buffers[(int)BufferIndex::RenderConfig]->Update(&renderConfig);
	
	graphics->SetConstantBuffer(buffers[(int)BufferIndex::RenderConfig], (int)BufferIndex::RenderConfig);

	RenderWorldMap();

	RenderPixelObjects();

	return renderBuffer;
}

PixelWorldEngine::InstanceData::InstanceData()
{
	memset(setting, 0, sizeof(setting));
	worldTransform = glm::mat4(1);
	texcoordTransform = glm::mat4(1);
	renderCoor = glm::vec4(0, 0, 0, 0);
}
