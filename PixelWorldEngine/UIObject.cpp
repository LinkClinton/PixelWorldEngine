#include "UIObject.hpp"
#include "PixelWorld.hpp"

void PixelWorldEngine::UIObject::UnRegisterFromParent(UIObject * object)
{
	if (object->parent == nullptr) return;

	object->parent->UnRegisterUIObject(object->name);
}

void PixelWorldEngine::UIObject::UnRegisterFromPixelWorld(UIObject * object)
{
	if (object->pixelWorld == nullptr) return;

	object->pixelWorld->UnRegisterUIObject(object->name);
}

PixelWorldEngine::UIObject::UIObject(std::string Name, float PositionX, float PositionY, float Width, float Height)
{
	name = Name;

	positionX = PositionX;
	positionY = PositionY;

	width = Width;
	height = Height;

	parent = nullptr;
	pixelWorld = nullptr;

	opacity = 1.0f;
	angle = 0;

	borderWidth = 0;

	renderObjectID = 0;
	depthLayer = 0;
}

void PixelWorldEngine::UIObject::SetBorderColor(float red, float green, float blue)
{
	borderColor[0] = red;
	borderColor[1] = green;
	borderColor[2] = blue;
}

void PixelWorldEngine::UIObject::SetPosition(float PositionX, float PositionY)
{
	positionX = PositionX;
	positionY = PositionY;
}

void PixelWorldEngine::UIObject::SetPositionX(float PositionX)
{
	positionX = PositionX;
}

void PixelWorldEngine::UIObject::SetPositionY(float PositionY)
{
	positionY = PositionY;
}

void PixelWorldEngine::UIObject::SetSize(float Width, float Height)
{
	width = Width;
	height = Height;
}

void PixelWorldEngine::UIObject::SetWidth(float Width)
{
	width = Width;
}

void PixelWorldEngine::UIObject::SetHeight(float Height)
{
	height = Height;
}

void PixelWorldEngine::UIObject::SetOpacity(float Opacity)
{
	opacity = Opacity;
}

void PixelWorldEngine::UIObject::SetAngle(float Angle)
{
	angle = Angle;
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

	if (depthLayer != DepthLayer && pixelWorld != nullptr) {
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
}

void PixelWorldEngine::UIObject::UnRegisterUIObject(UIObject * object)
{
	children.erase(object->name);
	childrenLayer.erase(object);

	object->parent = nullptr;
}

void PixelWorldEngine::UIObject::UnRegisterUIObject(std::string name)
{
	auto object = children[name];

	children.erase(name);
	childrenLayer.erase(object);
	
	object->parent = nullptr;
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

bool PixelWorldEngine::UIObjectCompare::operator()(UIObject * object1, UIObject * object2) const
{
	return object1->depthLayer < object2->depthLayer;
}
