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
	worldName = WorldName;
	graphics = Application->GetGraphics();

	renderObject = new Graphics::RectangleF(0, 0, 1, 1, graphics);
	
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
	DebugLayer::Assert(pixelObjects.count(pixelObject->name) == 0, Error::NoChildObject, pixelObject->name, worldName);

	pixelObjects.erase(pixelObject->name);
	pixelObjectLayer.erase(pixelObject);

	pixelObject->pixelWorld = nullptr;
}

void PixelWorldEngine::PixelWorld::UnRegisterPixelObject(std::string objectName)
{
	DebugLayer::Assert(pixelObjects.count(objectName) == 0, Error::NoChildObject, objectName, worldName);

	auto pixelObject = pixelObjects[objectName];

	pixelObjects.erase(objectName);
	pixelObjectLayer.erase(pixelObject);

	pixelObject->pixelWorld = nullptr;
}

void PixelWorldEngine::PixelWorld::RegisterUIObject(UIObject * object)
{
	UIObject::UnRegisterFromParent(object);
	UIObject::UnRegisterFromPixelWorld(object);

	UIObjects[object->name] = object;
	UIObjectLayer.insert(object);

	UIObject::SetPixelWorld(object, this);
}

void PixelWorldEngine::PixelWorld::UnRegisterUIObject(UIObject * object)
{
	DebugLayer::Assert(UIObjects.count(object->name) == 0, Error::NoChildObject, object->name, worldName);

	UIObjects.erase(object->name);
	UIObjectLayer.erase(object);

	object->isFocused = false;

	UIObject::SetPixelWorld(object, nullptr);
}

void PixelWorldEngine::PixelWorld::UnRegisterUIObject(std::string name)
{
	DebugLayer::Assert(UIObjects.count(name) == 0, Error::NoChildObject, name, worldName);

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

		for (int x = renderObjectRect.left; x <= renderObjectRect.right; x++) {

			for (int y = renderObjectRect.top; y <= renderObjectRect.bottom; y++) {
				if (worldMap->GetMapData(x, y) == nullptr) continue;

				auto matrix = glm::translate(glm::mat4(1), glm::vec3(x * mapBlockSize,
					y * mapBlockSize, 0.f)) * scaleMatrix;

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

		auto matrix = glm::translate(glm::mat4(1), glm::vec3(pixelObject->positionX - pixelObject->halfWidth,
			pixelObject->positionY - pixelObject->halfHeight, 0.f));

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

void PixelWorldEngine::PixelWorld::RenderUIObject(glm::mat4x4 baseTransform, float baseOpacity, UIObject* object)
{
	auto halfWidth = object->width * 0.5f;
	auto halfHeight = object->height * 0.5f;
	auto twoBorderWidth = object->borderWidth * 2.0f;

	auto opacity = baseOpacity * object->opacity;
	
	glm::mat4x4 translationMatrix = baseTransform * object->transformMatrix;

	renderConfig.renderColor = glm::vec4(object->borderColor[0], object->borderColor[1], object->borderColor[2], opacity);

	if (object->borderWidth != 0.0f) {
		auto widthScaleMatrix = glm::scale(glm::mat4(1), glm::vec3(object->width, object->borderWidth, 1.0f));
		auto heightScaleMatrix = glm::scale(glm::mat4(1), glm::vec3(object->borderWidth, object->height, 1.0f));

		auto beforeRotateMatrix = glm::translate(glm::mat4(1), glm::vec3(-halfWidth, -halfHeight, 0.0f));
		auto afterRotateMatrix = glm::translate(glm::mat4(1), glm::vec3(halfWidth, halfHeight, 0.0f));

		glm::mat4x4 borderMatrix[4];

		borderMatrix[0] = translationMatrix * widthScaleMatrix;
		borderMatrix[1] = glm::translate(translationMatrix, glm::vec3(0.0f, object->height - object->borderWidth - 1, 0.0f)) * widthScaleMatrix;
		borderMatrix[2] = translationMatrix * heightScaleMatrix;
		borderMatrix[3] = glm::translate(translationMatrix, glm::vec3(object->width - object->borderWidth - 1, 0.0f, 0.0f)) * heightScaleMatrix;

		renderConfig.renderObjectID[0] = 0;

		buffers[(int)BufferIndex::RenderConfig]->Update(&renderConfig);

		graphics->SetConstantBuffer(buffers[(int)BufferIndex::RenderConfig], (int)BufferIndex::RenderConfig);

		for (int i = 0; i < 4; i++) {
			buffers[(int)BufferIndex::TransformBuffer]->Update(&borderMatrix[i]);

			graphics->SetConstantBuffer(buffers[(int)BufferIndex::TransformBuffer], (int)BufferIndex::TransformBuffer);

			graphics->DrawIndexed(renderObject->GetIndexBuffer()->GetCount(), 0, 0);
		}
	}

	if (object->renderObjectID != 0) {
		glm::mat4 matrix = glm::mat4(1);

		matrix = glm::translate(translationMatrix, glm::vec3(object->borderWidth, object->borderWidth, 0.0f));
		matrix = glm::scale(matrix, glm::vec3(object->width - twoBorderWidth, object->height - twoBorderWidth, 1.0f));

		renderConfig.renderObjectID[0] = object->renderObjectID;

		buffers[(int)BufferIndex::TransformBuffer]->Update(&matrix);
		buffers[(int)BufferIndex::RenderConfig]->Update(&renderConfig);

		graphics->SetConstantBuffer(buffers[(int)BufferIndex::TransformBuffer], (int)BufferIndex::TransformBuffer);
		graphics->SetConstantBuffer(buffers[(int)BufferIndex::RenderConfig], (int)BufferIndex::RenderConfig);

		graphics->SetShaderResource(renderObjectIDGroup[object->renderObjectID], 0);

		graphics->DrawIndexed(renderObject->GetIndexBuffer()->GetCount(), 0, 0);
	}

	for (auto it = object->childrenLayer.begin(); it != object->childrenLayer.end(); it++)
		RenderUIObject(translationMatrix, opacity, *it);
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

		RenderUIObject(glm::mat4(1), 1.0f, object);
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
	
	graphics->SetStaticSampler(defaultSampler, 0);

	graphics->SetBlendState(true);

	RenderWorldMap();

	RenderPixelObjects();

	RenderUIObjects();

	return renderBuffer;
}




