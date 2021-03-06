#include "PixelWorld.hpp"

#include "Application.hpp"
#include "EngineDefaultResource.hpp"

PixelWorldEngine::TextureManager* PixelWorldEngine::PixelWorld::TextTextureManager = nullptr;

void PixelWorldEngine::PixelWorld::OnMouseMove(void * sender, Events::MouseMoveEvent * eventArg)
{
	Camera* usedCamera[(int)Layer::Count];

	usedCamera[(int)Layer::World] = camera;
	usedCamera[(int)Layer::UI] = &UICamera;

	for (size_t i = 0; i < layerRoots.size(); i++) {
		
		//如果摄像机不存在，那么事件也就没必要触发了
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
	Camera* usedCamera[(int)Layer::Count];

	usedCamera[(int)Layer::World] = camera;
	usedCamera[(int)Layer::UI] = &UICamera;

	for (size_t i = 0; i < layerRoots.size(); i++) {

		//如果摄像机不存在，那么事件也就没必要触发了
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
	Camera* usedCamera[(int)Layer::Count];

	usedCamera[(int)Layer::World] = camera;
	usedCamera[(int)Layer::UI] = &UICamera;

	for (size_t i = 0; i < layerRoots.size(); i++) {

		//如果摄像机不存在，那么事件也就没必要触发了
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

	//处理碰撞事件
	collideSolver.SolveCollide(layerRoots[(int)Layer::World]);

	for (auto it = layerRoots.begin(); it != layerRoots.end(); it++)
		Internal::PixelObjectProcess::ProcessAfterUpdate(*it);
}

PixelWorldEngine::PixelWorld::PixelWorld(std::string Name, Application * Application)
{
	DebugReturn(DebugLayer::Assert(Application == nullptr, Error::TheObjectIsNull, "Application", FunctionName));

	name = Name;
	graphics = Application->GetGraphics();

	renderObject = new Graphics::RectangleF(-0.5f, -0.5f, 0.5f, 0.5f, graphics);
	
	buffers.resize((int)BufferIndex::Count);
	bufferArrays.resize((int)BufferArrayIndex::Count);
	layerRoots.resize((int)Layer::Count);

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

	layerRoots[(int)Layer::World] = new PixelObject("WorldLayer");
	layerRoots[(int)Layer::UI] = new PixelObject("UILayer");

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

	defaultSampler = new Graphics::StaticSampler(graphics, Graphics::TextureAddressMode::Clamp);

	worldMap = nullptr;
	textureManager = nullptr;

	//静态变量，因此只有在未初始化的时候才初始化
	if (TextTextureManager == nullptr)
		TextTextureManager = new TextureManager(Application, Graphics::PixelFormat::A8);

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
	Utility::Delete(defaultSampler);
	Utility::Delete(renderBuffer);
	Utility::Delete(renderTarget);
	Utility::Delete(renderObject);
	Utility::Delete(renderCanvas);
}

void PixelWorldEngine::PixelWorld::SetResolution(int width, int height, float ssaa)
{
	DebugReturn(DebugLayer::Assert(width <= 0 || height <= 0, Error::WidthOrHeightLessThanZero, name, FunctionName));
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
	DebugReturn(DebugLayer::Assert(Camera == nullptr, Error::TheObjectIsNull, "Camera", FunctionName));

	camera = Camera;
}

void PixelWorldEngine::PixelWorld::SetShader(Graphics::GraphicsShader * Shader)
{
	DebugReturn(DebugLayer::Assert(Shader == nullptr, Error::TheObjectIsNull, "Shader", FunctionName));

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
	DebugReturn(DebugLayer::Assert(TextureManager == nullptr, Error::TheObjectIsNull, "TextureManager", FunctionName));

	textureManager = TextureManager;
}

void PixelWorldEngine::PixelWorld::SetPixelObject(PixelObject * pixelObject, Layer layer)
{
	layerRoots[(int)layer]->SetChild(pixelObject);
}

void PixelWorldEngine::PixelWorld::CancelPixelObject(std::string name, Layer layer)
{
	layerRoots[(int)layer]->CancelChild(name);
}

auto PixelWorldEngine::PixelWorld::GetWorldMap() -> WorldMap *
{
	return worldMap;
}

auto PixelWorldEngine::PixelWorld::GetPixelObject(std::string name, Layer layer) -> PixelObject*
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

	//如果地图不存在或者纹理管理器不存在，那么没有必要进行渲染
	if (worldMap != nullptr && textureManager != nullptr) {
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

				auto mapData = worldMap->mapData[y * worldMap->GetWidth() + x];
				auto arrayIndex = textureManager->GetArrayIndex(mapData.RenderObjectID);

				if (mapData.RenderObjectID == 0) continue;

				data.worldTransform = glm::translate(glm::mat4(1), glm::vec3(x * mapBlockSize + mapBlockSize * 0.5f,
					y * mapBlockSize + mapBlockSize * 0.5f, 0.f)) * scaleMatrix;
				data.texcoordTransform = textureManager->GetTexCoordTransform(mapData.RenderObjectID);
				data.setting[0] = mapData.RenderObjectID;
				data.setting[1] = arrayIndex;
				data.setting[2] = 0;
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
	if (pixelObject->IsEnableVisual == false) return;

	Collider cameraCollider = Collider(camera->GetRectangle());

	auto transformMatrix = baseTransformMatrix * pixelObject->Transform.GetMatrix();
	auto opacity = baseOpacity * pixelObject->Opacity;

	//如果摄像机无法看到物体，那么没必要进行渲染
	if (cameraCollider.Intersect(pixelObject->collider, glm::mat4(1), transformMatrix) == true) {

		if (pixelObject->RenderObjectID != 0 && textureManager != nullptr) {
			InstanceData data;

			auto matrix = glm::scale(transformMatrix, glm::vec3(pixelObject->width, pixelObject->height, 1.0f));
			auto arrayIndex = textureManager->GetArrayIndex(pixelObject->RenderObjectID);

			data.renderCoor = glm::vec4(1, 1, 1, opacity);
			data.setting[0] = pixelObject->RenderObjectID;
			data.setting[1] = arrayIndex;
			data.setting[2] = 0;
			data.worldTransform = matrix;
			data.texcoordTransform = textureManager->GetTexCoordTransform(pixelObject->RenderObjectID);

			instanceData->push_back(data);

			if (graphics->GetGraphicsMode() == Graphics::GraphicsMode::Low && instanceData->size() == LOW_MAX_INSTANCE_DATA)
				LowDrawObject(instanceData);
		}

		//渲染文字
		if (pixelObject->textInstance != nullptr) {
			InstanceData data;

			float scaleX = (float)pixelObject->textInstance->GetWidth();
			float scaleY = (float)pixelObject->textInstance->GetHeight();
			
			auto matrix = glm::scale(transformMatrix, glm::vec3(scaleX, scaleY, 1.0f));
			int arrayIndex = TextTextureManager->GetArrayIndex(pixelObject->objectID);

			data.renderCoor = glm::vec4(pixelObject->TextColor, opacity);
			data.setting[0] = pixelObject->objectID;
			data.setting[1] = arrayIndex;
			data.setting[2] = 1;
			data.worldTransform = matrix;
			data.texcoordTransform = TextTextureManager->GetTexCoordTransform(pixelObject->objectID);

			instanceData->push_back(data);

			if (graphics->GetGraphicsMode() == Graphics::GraphicsMode::Low && instanceData->size() == LOW_MAX_INSTANCE_DATA)
				LowDrawObject(instanceData);
		}
	}

	for (auto it = pixelObject->childrenDepthSort.begin(); it != pixelObject->childrenDepthSort.end(); it++)
		RenderPixelObject(transformMatrix, opacity, *it, instanceData, camera);
}

void PixelWorldEngine::PixelWorld::RenderPixelObjects()
{
	std::vector<Camera*> usedCamera((int)Layer::Count);
	std::vector<BufferArrayIndex> arrayIndex((int)Layer::Count);

	usedCamera[(int)Layer::World] = camera;
	usedCamera[(int)Layer::UI] = &UICamera;

	arrayIndex[(int)Layer::World] = BufferArrayIndex::PixelObjectInstanceData;
	arrayIndex[(int)Layer::UI] = BufferArrayIndex::UIObjectInstanceData;

	for (size_t i = 0; i < layerRoots.size(); i++) {
		std::vector<InstanceData> instanceData;

		//如果摄像机不存在那么没有必要进行渲染
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
	DebugReturnWithValue(DebugLayer::Assert(shader == nullptr, Error::TheShaderIsNull, name, "PixelWorld"), renderBuffer);
	DebugNone(DebugLayer::Assert(camera == nullptr, Error::TheCameraIsNull, name, "PixelWorld"));
	DebugNone(DebugLayer::Assert(textureManager == nullptr, Error::TheTextureManagerIsNull, name, "PixelWorld"));

	renderTarget->Clear(backGroundColor[0], backGroundColor[1], backGroundColor[2], backGroundColor[3]);

	graphics->ClearState();

	graphics->SetRenderTarget(renderTarget);

	graphics->SetViewPort(RectangleF(0.f, 0.f, (float)resolutionWidth * ssaaLevel, (float)resolutionHeight * ssaaLevel));

	graphics->SetShader(shader);

	graphics->SetVertexBuffer(renderObject->GetVertexBuffer());
	graphics->SetIndexBuffer(renderObject->GetIndexBuffer());

	graphics->SetBlendState(true);

	graphics->SetStaticSampler(defaultSampler, 0);

	if (textureManager != nullptr)
		graphics->SetShaderResource(textureManager->GetTextureArray(), 1);

	graphics->SetShaderResource(TextTextureManager->GetTextureArray(), 2);
	
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
