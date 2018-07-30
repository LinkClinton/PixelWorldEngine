#include "PixelObject.hpp"

#include "DebugLayer.hpp"

void PixelWorldEngine::PixelObject::UpdateTransform(PixelObject * object)
{
	object->oldTransform = object->Transform;

	for (auto it = object->childrenDepthSort.begin(); it != object->childrenDepthSort.end(); it++)
		UpdateTransform(*it);
}

PixelWorldEngine::PixelObject::PixelObject(std::string Name, PixelWorldEngine::Transform transform)
{
	name = Name;

	parent = nullptr;

	width = 0;
	height = 0;
	depth = 0;

	isHover = false;
	IsEnableRead = false;
	IsEnablePhysicsCollide = true;

	collider.SetArea(0, 0, 0, 0);

	Transform = transform;
	oldTransform = transform;

	RenderObjectID = 0;

	Opacity = 1.0f;
}

void PixelWorldEngine::PixelObject::SetDepth(int Depth)
{
	if (depth == Depth) return;

	depth = Depth;

	if (parent == nullptr) return;

	parent->childrenDepthSort.erase(this);
	parent->childrenDepthSort.insert(this);
}

void PixelWorldEngine::PixelObject::SetSize(float Width, float Height)
{
	width = Width;
	height = Height;
	
	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;

	collider.SetArea(-halfWidth, -halfHeight, halfWidth, halfHeight);
}

void PixelWorldEngine::PixelObject::SetParent(PixelObject * Parent)
{
	DebugReturn(DebugLayer::Assert(Parent == nullptr, Error::TheValueIsNotRight, "Parent", FunctionName));

	Parent->SetChild(this);
}

void PixelWorldEngine::PixelObject::SetChild(PixelObject * child)
{
	DebugReturn(DebugLayer::Assert(child == nullptr, Error::TheValueIsNotRight, "child", FunctionName));

	if (child->parent != nullptr) child->parent->CancelChild(child->name);

	childrenNameIndex.insert(std::pair<std::string, PixelObject*>(child->name, child));
	childrenDepthSort.insert(child);

	child->parent = this;
}

void PixelWorldEngine::PixelObject::CancelChild(std::string name)
{
	DebugReturn(DebugLayer::Assert(childrenNameIndex.count(name) == 0, Error::TheNameIsNotExist, name, FunctionName));

	auto child = childrenNameIndex[name];

	child->parent = nullptr;
	
	childrenNameIndex.erase(name);
	childrenDepthSort.erase(child);
}

auto PixelWorldEngine::PixelObject::GetName() -> std::string
{
	return name;
}

auto PixelWorldEngine::PixelObject::GetDepth() -> int
{
	return depth;
}

auto PixelWorldEngine::PixelObject::GetSize() -> SizeF
{
	return SizeF(width, height);
}

auto PixelWorldEngine::PixelObject::GetParent() -> PixelObject *
{
	return parent;
}

auto PixelWorldEngine::PixelObject::GetChildren(std::string name) -> PixelObject *
{
	if (childrenNameIndex.count(name) == 0) return nullptr;

	return childrenNameIndex[name];
}

bool PixelWorldEngine::Internal::PixelObjectCompare::operator()(PixelObject * object1, PixelObject * object2) const
{
	return object1->depth < object2->depth;
}

void PixelWorldEngine::Internal::PixelObjectProcess::ProcessUpdate(PixelObject * object)
{
	object->OnUpdate(object);

	Events::DoEventHandlers(object->Update, object);

	for (auto it = object->childrenDepthSort.begin(); it != object->childrenDepthSort.end(); it++)
		ProcessUpdate(*it);
}

void PixelWorldEngine::Internal::PixelObjectProcess::ProcessMouseEnter(PixelObject * object)
{
	object->OnMouseEnter(object);

	Events::DoEventHandlers(object->MouseEnter, object);

	object->isHover = true;
}

void PixelWorldEngine::Internal::PixelObjectProcess::ProcessMouseLeave(PixelObject * object)
{
	object->OnMouseLeave(object);

	Events::DoEventHandlers(object->MouseLeave, object);

	object->isHover = false;
}

void PixelWorldEngine::Internal::PixelObjectProcess::ProcessMouseMove(PixelObject * object, Events::MouseMoveEvent * eventArg, glm::mat4x4 baseTransformMatrix)
{
	baseTransformMatrix = baseTransformMatrix * object->Transform.GetMatrix();

	if (object->collider.Intersect(glm::vec2(eventArg->x, eventArg->y), baseTransformMatrix) == true) {
		object->OnMouseMove(object, eventArg);

		Events::DoEventHandlers(object->MouseMove, object, eventArg);

		if (object->isHover == false) ProcessMouseEnter(object);
	}
	else if (object->isHover == true) ProcessMouseLeave(object);

	for (auto it = object->childrenDepthSort.begin(); it != object->childrenDepthSort.end(); it++)
		ProcessMouseMove(*it, eventArg, baseTransformMatrix);
}

void PixelWorldEngine::Internal::PixelObjectProcess::ProcessMouseClick(PixelObject * object, Events::MouseClickEvent * eventArg, glm::mat4x4 baseTransformMatrix)
{
	baseTransformMatrix = baseTransformMatrix * object->Transform.GetMatrix();

	if (object->collider.Intersect(glm::vec2(eventArg->x, eventArg->y), baseTransformMatrix) == true) {
		object->OnMouseClick(object, eventArg);

		Events::DoEventHandlers(object->MouseClick, object, eventArg);
	}

	for (auto it = object->childrenDepthSort.begin(); it != object->childrenDepthSort.end(); it++)
		ProcessMouseClick(*it, eventArg, baseTransformMatrix);
}

void PixelWorldEngine::Internal::PixelObjectProcess::ProcessMouseWheel(PixelObject * object, Events::MouseWheelEvent * eventArg, glm::mat4x4 baseTransformMatrix)
{
	baseTransformMatrix = baseTransformMatrix * object->Transform.GetMatrix();

	if (object->collider.Intersect(glm::vec2(eventArg->x, eventArg->y), baseTransformMatrix) == true) {
		object->OnMouseWheel(object, eventArg);

		Events::DoEventHandlers(object->MouseWheel, object, eventArg);
	}

	for (auto it = object->childrenDepthSort.begin(); it != object->childrenDepthSort.end(); it++)
		ProcessMouseWheel(*it, eventArg, baseTransformMatrix);
}

void PixelWorldEngine::Internal::PixelObjectProcess::ProcessKeyClick(PixelObject * object, Events::KeyClickEvent * eventArg)
{
	if (object->IsEnableRead == false) return;

	object->OnKeyClick(object, eventArg);

	Events::DoEventHandlers(object->KeyClick, object, eventArg);

	for (auto it = object->childrenDepthSort.begin(); it != object->childrenDepthSort.end(); it++)
		ProcessKeyClick(*it, eventArg);
}

void PixelWorldEngine::Internal::PixelObjectProcess::ProcessObjectCollide(PixelObject * objectA, PixelObject * objectB)
{
	objectA->OnObjectCollide(objectA, objectB);
	objectB->OnObjectCollide(objectB, objectA);

	Events::DoEventHandlers(objectA->ObjectCollide, objectA, objectB);
	Events::DoEventHandlers(objectB->ObjectCollide, objectB, objectA);
}

void PixelWorldEngine::Internal::PixelObjectProcess::ProcessObjectEnter(PixelObject * objectA, PixelObject * objectB)
{
	objectA->OnObjectEnter(objectA, objectB);
	objectB->OnObjectEnter(objectB, objectA);

	Events::DoEventHandlers(objectA->ObjectEnter, objectA, objectB);
	Events::DoEventHandlers(objectB->ObjectEnter, objectB, objectA);
}

void PixelWorldEngine::Internal::PixelObjectProcess::ProcessObjectLeave(PixelObject * objectA, PixelObject * objectB)
{
	objectA->OnObjectLeave(objectA, objectB);
	objectB->OnObjectLeave(objectB, objectA);

	Events::DoEventHandlers(objectA->ObjectLeave, objectA, objectB);
	Events::DoEventHandlers(objectB->ObjectLeave, objectB, objectA);
}


