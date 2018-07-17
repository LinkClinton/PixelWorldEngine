#include "PixelWorld.hpp"

#include "Application.hpp"
#include "EngineDefaultResource.hpp"

void PixelWorldEngine::PixelWorld::OnMouseMove(void * sender, Events::MouseMoveEvent * eventArg)
{
	for (auto it = UIObjectLayer.begin(); it != UIObjectLayer.end(); it++)
		(*it)->ProcessMouseMoveEvent(*it, eventArg, glm::mat4(1));
}

void PixelWorldEngine::PixelWorld::OnMouseClick(void * sender, Events::MouseClickEvent * eventArg)
{
	for (auto it = UIObjectLayer.begin(); it != UIObjectLayer.end(); it++)
		(*it)->ProcessMouseClickEvent(*it, eventArg, glm::mat4(1));
}

void PixelWorldEngine::PixelWorld::OnMouseWheel(void * sender, Events::MouseWheelEvent * eventArg)
{
	for (auto it = UIObjectLayer.begin(); it != UIObjectLayer.end(); it++)
		(*it)->ProcessMouseWheelEvent(*it, eventArg, glm::mat4(1));
}

void PixelWorldEngine::PixelWorld::OnKeyClick(void * sender, Events::KeyClickEvent * eventArg)
{
	for (auto it = UIObjectLayer.begin(); it != UIObjectLayer.end(); it++)
		(*it)->ProcessKeyClickEvent(*it, eventArg);
}

void PixelWorldEngine::PixelWorld::OnUpdate(float deltaTime)
{
	for (auto it = pixelObjects.begin(); it != pixelObjects.end(); it++) {
		it->second->OnUpdate(deltaTime);
		Events::DoEventHandlers(it->second->Update, deltaTime);
	}
}

PixelWorldEngine::PixelWorld::PixelWorld(std::string WorldName, Application * Application)
{
	DebugReturn(DebugLayer::Assert(Application == nullptr, Error::TheObjectIsNull, "Application", FunctionName));

	worldName = WorldName;
	graphics = Application->GetGraphics();

	renderObject = new Graphics::RectangleF(0, 0, 1, 1, graphics);
	
	buffers.resize((int)BufferIndex::Count);
	bufferArrays.resize((int)BufferArrayIndex::Count);

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

	focusUIObject = nullptr;

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

void PixelWorldEngine::PixelWorld::SetWorldMap(std::string worldMapName)
{
	worldMap = worldMaps[worldMapName];
}

void PixelWorldEngine::PixelWorld::SetWorldMap(WorldMap * WorldMap)
{
	DebugReturn(DebugLayer::Assert(WorldMap == nullptr, Error::TheObjectIsNull, "WorldMap", FunctionName));

	if (worldMaps[WorldMap->GetMapName()] == nullptr)
		RegisterWorldMap(WorldMap);

	worldMap = WorldMap;
}

void PixelWorldEngine::PixelWorld::SetTextureManager(TextureManager * TextureManager)
{
	textureManager = TextureManager;
}

void PixelWorldEngine::PixelWorld::RegisterWorldMap(WorldMap * worldMap)
{
	DebugReturn(DebugLayer::Assert(worldMap == nullptr, Error::TheObjectIsNull, "worldMap", FunctionName));

	worldMaps[worldMap->GetMapName()] = worldMap;
}

void PixelWorldEngine::PixelWorld::RegisterPixelObject(PixelObject * pixelObject)
{
	DebugReturn(DebugLayer::Assert(pixelObject == nullptr, Error::TheObjectIsNull, "pixelObject", FunctionName));

	if (pixelObject->pixelWorld != nullptr) 
		pixelObject->pixelWorld->UnRegisterPixelObject(pixelObject);

	pixelObjects[pixelObject->name] = pixelObject;
	pixelObjectLayer.insert(pixelObject);
	
	pixelObject->pixelWorld = this;
}

void PixelWorldEngine::PixelWorld::UnRegisterPixelObject(PixelObject * pixelObject)
{
	DebugReturn(DebugLayer::Assert(pixelObject == nullptr, Error::TheObjectIsNull, "pixelObject", FunctionName));
	DebugReturn(DebugLayer::Assert(pixelObjects.count(pixelObject->name) == 0, Error::TheNameIsNotExist, pixelObject->name, FunctionName));

	pixelObjects.erase(pixelObject->name);
	pixelObjectLayer.erase(pixelObject);

	pixelObject->pixelWorld = nullptr;
}

void PixelWorldEngine::PixelWorld::UnRegisterPixelObject(std::string objectName)
{
	DebugReturn(DebugLayer::Assert(pixelObjects.count(objectName) == 0, Error::TheNameIsNotExist, objectName, FunctionName));
	
	auto pixelObject = pixelObjects[objectName];

	pixelObjects.erase(objectName);
	pixelObjectLayer.erase(pixelObject);

	pixelObject->pixelWorld = nullptr;
}

void PixelWorldEngine::PixelWorld::RegisterUIObject(UIObject * object)
{
	DebugReturn(DebugLayer::Assert(object == nullptr, Error::TheObjectIsNull, "object", FunctionName));

	UIObject::UnRegisterFromParent(object);
	UIObject::UnRegisterFromPixelWorld(object);

	UIObjects[object->name] = object;
	UIObjectLayer.insert(object);

	UIObject::SetPixelWorld(object, this);
}

void PixelWorldEngine::PixelWorld::UnRegisterUIObject(UIObject * object)
{
	DebugReturn(DebugLayer::Assert(object == nullptr, Error::TheObjectIsNull, "object", FunctionName));
	DebugReturn(DebugLayer::Assert(UIObjects.count(object->name) == 0, Error::TheNameIsNotExist, object->name, FunctionName));
	
	UIObjects.erase(object->name);
	UIObjectLayer.erase(object);

	object->isFocused = false;

	UIObject::SetPixelWorld(object, nullptr);
}

void PixelWorldEngine::PixelWorld::UnRegisterUIObject(std::string name)
{
	DebugReturn(DebugLayer::Assert(UIObjects.count(name) == 0, Error::TheNameIsNotExist, name, FunctionName));

	auto object = UIObjects[name];

	UIObjects.erase(name);
	UIObjectLayer.erase(object);

	object->isFocused = false;

	UIObject::SetPixelWorld(object, nullptr);
}

auto PixelWorldEngine::PixelWorld::GetWorldMap() -> WorldMap *
{
	return worldMap;
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
	auto matrix = camera->GetMatrix();
	auto viewRect = camera->GetRectangle();
	auto renderObjectRect = Rectangle();

	buffers[(int)BufferIndex::CameraBuffer]->Update(&matrix);

	graphics->SetConstantBuffer(buffers[(int)BufferIndex::CameraBuffer], (int)BufferIndex::CameraBuffer);

	if (worldMap != nullptr) {
		auto mapBlockSize = worldMap->GetMapBlockSize();
		
		renderObjectRect.left = Utility::Max((int)(viewRect.left / mapBlockSize), 0);
		renderObjectRect.top = Utility::Max((int)(viewRect.top / mapBlockSize), 0);
		renderObjectRect.right = Utility::Min((int)(viewRect.right / mapBlockSize) + 1, worldMap->GetHeight() - 1);
		renderObjectRect.bottom = Utility::Min((int)(viewRect.bottom / mapBlockSize) + 1, worldMap->GetHeight() - 1);

		auto scaleMatrix = glm::scale(glm::mat4(1), glm::vec3(mapBlockSize, mapBlockSize, 1.f));

		std::vector<InstanceData> instanceData;

		for (int x = renderObjectRect.left; x <= renderObjectRect.right; x++) {

			for (int y = renderObjectRect.top; y <= renderObjectRect.bottom; y++) {
				if (worldMap->GetMapData(x, y) == nullptr) continue;

				InstanceData data;

				auto mapData = worldMap->GetMapData(x, y);
				auto whichID = textureManager->GetWhich(mapData->RenderObjectID);

				if (whichID == MAX_MERGETEXTURE_COUNT) continue;

				data.worldTransform = glm::translate(glm::mat4(1), glm::vec3(x * mapBlockSize, y * mapBlockSize, 0.f)) * scaleMatrix;
				data.texcoordTransform = textureManager->mergeTextures[whichID]->GetTexCoordTransform(mapData->RenderObjectID);
				data.setting[0] = mapData->RenderObjectID;
				data.setting[1] = whichID;
				data.renderCoor = glm::vec4(1.0f, 1.0f, 1.0f, mapData->Opacity);

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

void PixelWorldEngine::PixelWorld::RenderPixelObjects()
{
	std::vector<InstanceData> instanceData;

	for (auto it = pixelObjectLayer.begin(); it != pixelObjectLayer.end(); it++) {
		auto pixelObject = *it;
		InstanceData data;

		if (pixelObject->renderObjectID == 0) continue;

		auto matrix = glm::translate(glm::mat4(1), glm::vec3(pixelObject->positionX,
			pixelObject->positionY, 0.f));
		auto whichID = textureManager->GetWhich(pixelObject->renderObjectID);

		if (whichID == MAX_MERGETEXTURE_COUNT) continue;

		matrix = glm::scale(matrix, glm::vec3(pixelObject->width, pixelObject->height, 1.f));

		data.renderCoor = glm::vec4(pixelObject->effectColor[0], pixelObject->effectColor[1], pixelObject->effectColor[2], pixelObject->opacity);
		data.setting[0] = pixelObject->renderObjectID;
		data.setting[1] = whichID;
		data.worldTransform = matrix;
		data.texcoordTransform = textureManager->mergeTextures[whichID]->GetTexCoordTransform(pixelObject->renderObjectID);

		instanceData.push_back(data);

		if (graphics->GetGraphicsMode() == Graphics::GraphicsMode::Low && instanceData.size() == LOW_MAX_INSTANCE_DATA)
			LowDrawObject(&instanceData);
	}

	if (instanceData.size() != 0) {
		if (graphics->GetGraphicsMode() == Graphics::GraphicsMode::High)
			HighDrawObject(&instanceData, BufferArrayIndex::PixelObjectInstanceData);
		else LowDrawObject(&instanceData);
	}
}

void PixelWorldEngine::PixelWorld::RenderUIObject(glm::mat4x4 baseTransform, float baseOpacity, UIObject* object, std::vector<InstanceData>* instanceData)
{
	auto halfWidth = object->width * 0.5f;
	auto halfHeight = object->height * 0.5f;
	auto twoBorderWidth = object->borderWidth * 2.0f;

	auto opacity = baseOpacity * object->opacity;
	
	glm::mat4x4 translationMatrix = baseTransform * object->transformMatrix;

	if (object->borderWidth != 0.0f) {
		auto widthScaleMatrix = glm::scale(glm::mat4(1), glm::vec3(object->width, object->borderWidth, 1.0f));
		auto heightScaleMatrix = glm::scale(glm::mat4(1), glm::vec3(object->borderWidth, object->height, 1.0f));

		auto beforeRotateMatrix = glm::translate(glm::mat4(1), glm::vec3(-halfWidth, -halfHeight, 0.0f));
		auto afterRotateMatrix = glm::translate(glm::mat4(1), glm::vec3(halfWidth, halfHeight, 0.0f));

		InstanceData data[4];

		data[0].worldTransform = translationMatrix * widthScaleMatrix;
		data[1].worldTransform = glm::translate(translationMatrix, glm::vec3(0.0f, object->height - object->borderWidth, 0.0f)) * widthScaleMatrix;
		data[2].worldTransform = translationMatrix * heightScaleMatrix;
		data[3].worldTransform = glm::translate(translationMatrix, glm::vec3(object->width - object->borderWidth, 0.0f, 0.0f)) * heightScaleMatrix;

		auto color = glm::vec4(object->borderColor[0], object->borderColor[1], object->borderColor[2], object->opacity);

		data[0].renderCoor = color;
		data[1].renderCoor = color;
		data[2].renderCoor = color;
		data[3].renderCoor = color;

		for (int i = 0; i < 4; i++) {
			instanceData[i].push_back(data[i]);

			if (graphics->GetGraphicsMode() == Graphics::GraphicsMode::Low && instanceData[i].size() == LOW_MAX_INSTANCE_DATA)
				LowDrawObject(&instanceData[i]);
		}
	}

	if (object->renderObjectID != 0) {
		glm::mat4 matrix = glm::mat4(1);

		matrix = glm::translate(translationMatrix, glm::vec3(object->borderWidth, object->borderWidth, 0.0f));
		matrix = glm::scale(matrix, glm::vec3(object->width - twoBorderWidth, object->height - twoBorderWidth, 1.0f));

		InstanceData data;

		int whichID = textureManager->GetWhich(object->renderObjectID);

		if (whichID != MAX_MERGETEXTURE_COUNT) {

			data.renderCoor = glm::vec4(object->effectColor[0], object->effectColor[1], object->effectColor[2], object->opacity);
			data.setting[0] = object->renderObjectID;
			data.setting[1] = whichID;
			data.worldTransform = matrix;
			data.texcoordTransform = textureManager->mergeTextures[whichID]->GetTexCoordTransform(object->renderObjectID);

			instanceData[4].push_back(data);

			if (graphics->GetGraphicsMode() == Graphics::GraphicsMode::Low && instanceData[4].size() == LOW_MAX_INSTANCE_DATA)
				LowDrawObject(&instanceData[4]);
		}
	}

	for (auto it = object->childrenLayer.begin(); it != object->childrenLayer.end(); it++)
		RenderUIObject(translationMatrix, opacity, *it, instanceData);
}

void PixelWorldEngine::PixelWorld::RenderUIObjects()
{
	auto cameraMatrix = UICamera.GetMatrix();

	buffers[(int)BufferIndex::CameraBuffer]->Update(&cameraMatrix);

	graphics->SetConstantBuffer(buffers[(int)BufferIndex::CameraBuffer], (int)BufferIndex::CameraBuffer);

	std::vector<InstanceData> instanceData[5];

	for (auto it = UIObjectLayer.begin(); it != UIObjectLayer.end(); it++) {
		auto object = *it;

		RenderUIObject(glm::mat4(1), 1.0f, object, instanceData);
	}

	for (int i = 0; i < 5; i++) {
		if (instanceData[i].size() != 0) {

			if (graphics->GetGraphicsMode() == Graphics::GraphicsMode::High)
				HighDrawObject(&instanceData[i], BufferArrayIndex::UIObjectInstanceData);
			else LowDrawObject(&instanceData[i]);
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

	for (int i = 0; i < MAX_MERGETEXTURE_COUNT; i++) {
		if (textureManager == nullptr) break;

		if (textureManager->mergeTextures[i] == nullptr) continue;
		
		renderConfig.mergeTextureFormat[i] = textureManager->mergeTextures[i]->GetPixelFormat();

		graphics->SetShaderResource(textureManager->mergeTextures[i]->GetFinalTexture(), i + 1);
	}
	
	buffers[(int)BufferIndex::RenderConfig]->Update(&renderConfig);
	
	graphics->SetConstantBuffer(buffers[(int)BufferIndex::RenderConfig], (int)BufferIndex::RenderConfig);

	if (camera != nullptr) {

		RenderWorldMap();

		RenderPixelObjects();
	}

	RenderUIObjects();

	return renderBuffer;
}

PixelWorldEngine::InstanceData::InstanceData()
{
	memset(setting, 0, sizeof(setting));
	worldTransform = glm::mat4(1);
	texcoordTransform = glm::mat4(1);
	renderCoor = glm::vec4(0, 0, 0, 0);
}
