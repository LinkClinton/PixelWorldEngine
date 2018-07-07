#include "UIObject.hpp"
#include "PixelWorld.hpp"

auto PixelWorldEngine::UIObject::CreateTransformMatrix(UIObject * object) -> glm::mat4x4
{
	glm::mat4x4 transformMatrix = glm::mat4(1);

	transformMatrix = glm::translate(glm::mat4(1), glm::vec3(object->positionX + object->halfWidth, object->positionY + object->halfHeight, 0.0f));
	transformMatrix = glm::rotate(transformMatrix, object->angle, glm::vec3(0.0f, 0.0f, 1.0f));
	transformMatrix = glm::translate(transformMatrix, glm::vec3(-object->halfWidth, -object->halfHeight, 0.0f));

	return transformMatrix;
}

void PixelWorldEngine::UIObject::SetPixelWorld(UIObject * object, PixelWorld * pixelWorld)
{
	object->pixelWorld = pixelWorld;

	for (auto it = object->childrenLayer.begin(); it != object->childrenLayer.end(); it++)
		SetPixelWorld(*it, pixelWorld);
}

void PixelWorldEngine::UIObject::SetFocus(UIObject * object, bool isFocused)
{
	if (object == nullptr) return;

	object->isFocused = isFocused;
}

void PixelWorldEngine::UIObject::UnRegisterFromParent(UIObject * object)
{
	if (object->parent == nullptr) return;

	object->parent->UnRegisterUIObject(object->name);
}

void PixelWorldEngine::UIObject::UnRegisterFromPixelWorld(UIObject * object)
{
	if (object->pixelWorld == nullptr || object->parent != nullptr) return;

	object->pixelWorld->UnRegisterUIObject(object->name);
}

void PixelWorldEngine::UIObject::ProcessMouseMoveEvent(UIObject * object, Events::MouseMoveEvent * eventArg, glm::mat4x4 baseTransformMatrix)
{
	baseTransformMatrix = baseTransformMatrix * object->transformMatrix;

	auto mousePosition = glm::inverse(baseTransformMatrix) * glm::vec4(eventArg->x, eventArg->y, 0, 1);

	if (IsInUIObjectRect(object, mousePosition.x, mousePosition.y) == true) {
		object->OnMouseMove(object, eventArg);

		Events::DoEventHandlers(object->MouseMove, object, eventArg);

		for (auto it = object->childrenLayer.begin(); it != object->childrenLayer.end(); it++)
			ProcessMouseMoveEvent(*it, eventArg, baseTransformMatrix);
	}
}

void PixelWorldEngine::UIObject::ProcessMouseClickEvent(UIObject * object, Events::MouseClickEvent * eventArg, glm::mat4x4 baseTransformMatrix)
{
	baseTransformMatrix = baseTransformMatrix * object->transformMatrix;
	
	auto mousePosition = glm::inverse(baseTransformMatrix) * glm::vec4(eventArg->x, eventArg->y, 0, 1);

	if (IsInUIObjectRect(object, mousePosition.x, mousePosition.y) == true) {
		object->OnMouseClick(object, eventArg);

		Events::DoEventHandlers(object->MouseClick, object, eventArg);

		if (eventArg->isDown == true && eventArg->button == Events::MouseButton::Left) {
			SetFocus(object->pixelWorld->focusUIObject, false);

			object->pixelWorld->focusUIObject = object;

			SetFocus(object->pixelWorld->focusUIObject, true);
		}

		for (auto it = object->childrenLayer.begin(); it != object->childrenLayer.end(); it++)
			ProcessMouseClickEvent(*it, eventArg, baseTransformMatrix);
	}
}

void PixelWorldEngine::UIObject::ProcessMouseWheelEvent(UIObject * object, Events::MouseWheelEvent * eventArg, glm::mat4x4 baseTransformMatrix)
{
	baseTransformMatrix = baseTransformMatrix * object->transformMatrix;

	auto mousePosition = glm::inverse(baseTransformMatrix) * glm::vec4(eventArg->x, eventArg->y, 0, 1);

	if (IsInUIObjectRect(object, mousePosition.x, mousePosition.y) == true) {
		object->OnMouseWheel(object, eventArg);

		Events::DoEventHandlers(object->MouseWheel, object, eventArg);

		for (auto it = object->childrenLayer.begin(); it != object->childrenLayer.end(); it++)
			ProcessMouseWheelEvent(*it, eventArg, baseTransformMatrix);
	}
}

void PixelWorldEngine::UIObject::ProcessKeyClickEvent(UIObject * object, Events::KeyClickEvent * eventArg)
{
	if (object->isFocused == true) {
		object->OnKeyClick(object, eventArg);

		Events::DoEventHandlers(object->KeyClick, object, eventArg);
	}

	for (auto it = object->childrenLayer.begin(); it != object->childrenLayer.end(); it++)
		ProcessKeyClickEvent(*it, eventArg);
}

void PixelWorldEngine::UIObject::OnMouseMove(void * sender, Events::MouseMoveEvent * eventArg)
{
}

void PixelWorldEngine::UIObject::OnMouseClick(void * sender, Events::MouseClickEvent * eventArg)
{
}

void PixelWorldEngine::UIObject::OnMouseWheel(void * sender, Events::MouseWheelEvent * eventArg)
{
}

void PixelWorldEngine::UIObject::OnKeyClick(void * sender, Events::KeyClickEvent * eventArg)
{
}


PixelWorldEngine::UIObject::UIObject(std::string Name, float PositionX, float PositionY, float Width, float Height)
{
	name = Name;

	positionX = PositionX;
	positionY = PositionY;

	width = Width;
	height = Height;

	halfWidth = width * 0.5f;
	halfHeight = height * 0.5f;

	parent = nullptr;
	pixelWorld = nullptr;

	opacity = 1.0f;
	angle = 0;

	borderWidth = 0;

	renderObjectID = 0;
	depthLayer = 0;

	isFocused = false;

	transformMatrix = CreateTransformMatrix(this);
}

void PixelWorldEngine::UIObject::SetBorderColor(float red, float green, float blue)
{
	borderColor[0] = red;
	borderColor[1] = green;
	borderColor[2] = blue;
}

void PixelWorldEngine::UIObject::SetPosition(float PositionX, float PositionY)
{
	transformMatrix = glm::translate(glm::mat4(1), glm::vec3(PositionX - positionX, PositionY - positionY, 0.0f)) * transformMatrix;

	positionX = PositionX;
	positionY = PositionY;
}

void PixelWorldEngine::UIObject::SetPositionX(float PositionX)
{
	transformMatrix = glm::translate(glm::mat4(1), glm::vec3(PositionX - positionX, 0.0f, 0.0f)) * transformMatrix;

	positionX = PositionX;
}

void PixelWorldEngine::UIObject::SetPositionY(float PositionY)
{
	transformMatrix = glm::translate(glm::mat4(1), glm::vec3(0.0f, PositionY - positionY, 0.0f)) * transformMatrix;

	positionY = PositionY;
}

void PixelWorldEngine::UIObject::SetSize(float Width, float Height)
{
	width = Width;
	height = Height;

	halfWidth = width * 0.5f;
	halfHeight = height * 0.5f;

	transformMatrix = CreateTransformMatrix(this);
}

void PixelWorldEngine::UIObject::SetWidth(float Width)
{
	width = Width;

	halfWidth = width * 0.5f;

	transformMatrix = CreateTransformMatrix(this);
}

void PixelWorldEngine::UIObject::SetHeight(float Height)
{
	height = Height;

	halfHeight = height * 0.5f;

	transformMatrix = CreateTransformMatrix(this);
}

void PixelWorldEngine::UIObject::SetOpacity(float Opacity)
{
	opacity = Opacity;
}

void PixelWorldEngine::UIObject::SetAngle(float Angle)
{
	angle = Angle;

	transformMatrix = CreateTransformMatrix(this);
}

void PixelWorldEngine::UIObject::SetBorderWidth(float width)
{
	borderWidth = width;
}

void PixelWorldEngine::UIObject::SetRenderObjectID(int id)
{
	renderObjectID = id;
}

void PixelWorldEngine::UIObject::SetDepthLayer(int DepthLayer)
{
	if (depthLayer != DepthLayer && parent != nullptr) {
		depthLayer = DepthLayer;

		parent->childrenLayer.erase(this);
		parent->childrenLayer.insert(this);

		return;
	}

	if (depthLayer != DepthLayer && pixelWorld != nullptr &&
		parent == nullptr) {

		depthLayer = DepthLayer;

		pixelWorld->UIObjectLayer.erase(this);
		pixelWorld->UIObjectLayer.insert(this);

		return;
	}

	depthLayer = DepthLayer;
}

void PixelWorldEngine::UIObject::RegisterUIObject(UIObject * object)
{
	UnRegisterFromParent(object);
	UnRegisterFromPixelWorld(object);

	children[object->name] = object;
	childrenLayer.insert(object);

	object->parent = this;
	
	SetPixelWorld(object, pixelWorld);
}

void PixelWorldEngine::UIObject::UnRegisterUIObject(UIObject * object)
{
	children.erase(object->name);
	childrenLayer.erase(object);

	object->parent = nullptr;
	object->isFocused = false;

	SetPixelWorld(object, nullptr);
}

void PixelWorldEngine::UIObject::UnRegisterUIObject(std::string name)
{
	auto object = children[name];

	children.erase(name);
	childrenLayer.erase(object);
	
	object->parent = nullptr;
	object->isFocused = false;

	SetPixelWorld(object, nullptr);
}

auto PixelWorldEngine::UIObject::GetBorderColor() -> float *
{
	return borderColor;
}

auto PixelWorldEngine::UIObject::GetPositionX() -> float
{
	return positionX;
}

auto PixelWorldEngine::UIObject::GetPositionY() -> float
{
	return positionY;
}

auto PixelWorldEngine::UIObject::GetWidth() -> float
{
	return width;
}

auto PixelWorldEngine::UIObject::GetHeight() -> float
{
	return height;
}

auto PixelWorldEngine::UIObject::GetOpacity() -> float
{
	return opacity;
}

auto PixelWorldEngine::UIObject::GetAngle() -> float
{
	return angle;
}

auto PixelWorldEngine::UIObject::GetBorderWidth() -> float
{
	return borderWidth;
}

auto PixelWorldEngine::UIObject::GetRenderObjectID() -> int
{
	return renderObjectID;
}

auto PixelWorldEngine::UIObject::GetDepthLayer() -> int
{
	return depthLayer;
}

auto PixelWorldEngine::UIObject::GetPixelWorld() -> PixelWorld *
{
	return pixelWorld;
}

auto PixelWorldEngine::UIObject::IsInUIObjectRect(UIObject * object, float x, float y) -> bool
{
	if (x >= 0 && x < object->width &&
		y >= 0 && y < object->height)
		return true;
	return false;
}

bool PixelWorldEngine::UIObjectCompare::operator()(UIObject * object1, UIObject * object2) const
{
	return object1->depthLayer < object2->depthLayer;
}
