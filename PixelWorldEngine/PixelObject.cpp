#include "PixelObject.hpp"

#include "DebugLayer.hpp"
#include "PixelWorld.hpp"

#define DEFAULT_BASE_NAME "PIXELOBJECT"

int PixelWorldEngine::PixelObject::ObjectCount = 0;
std::vector<int> PixelWorldEngine::PixelObject::FreeObjectID;

void PixelWorldEngine::PixelObject::UpdateTransform(PixelObject * object)
{
	object->oldTransform = object->Transform;

	for (auto it = object->childrenDepthSort.begin(); it != object->childrenDepthSort.end(); it++)
		UpdateTransform(*it);
}

void PixelWorldEngine::PixelObject::UpdateText(PixelObject * object)
{
	//如果文字不需要改变，那么返回
	if (object->textInstance != nullptr && object->Text == object->textInstance->GetText() &&
		object->Font == object->textInstance->GetFont() && object->isSizeChange == false) return;

	//将注册的纹理取消
	if (PixelWorld::TextTextureManager->IsExist(object->objectID) == true)
		PixelWorld::TextTextureManager->UnRegisterTexture(object->objectID);

	//释放原本的资源
	Utility::Delete(object->textInstance);

	//如果不需要渲染文字，那么返回
	if (object->Font == nullptr || object->width <= 0 || object->Text == "") return;
	
	//创建文字纹理
	object->textInstance = new PixelWorldEngine::Text(object->Text, object->Font, (int)object->width);

	//注册纹理
	PixelWorld::TextTextureManager->RegisterTexture(object->objectID, object->textInstance->GetTexture());
}

auto PixelWorldEngine::PixelObject::GetObjectID() -> int
{
	if (FreeObjectID.size() != 0) {
		int result = FreeObjectID.back();
		
		FreeObjectID.pop_back();

		return result;
	}

	return ++ObjectCount;
}

PixelWorldEngine::PixelObject::PixelObject(std::string Name, PixelWorldEngine::Transform transform)
{
	name = Name;

	parent = nullptr;

	width = 0;
	height = 0;
	depth = 0;

	isHover = false;
	isSizeChange = false;
	IsEnableVisual = true;
	IsEnableRead = false;
	IsEnablePhysicsCollide = false;

	collider.SetArea(0, 0, 0, 0);

	textInstance = nullptr;

	Transform = transform;
	oldTransform = transform;

	Text = "";
	Font = nullptr;
	
	RenderObjectID = 0;

	TextColor = glm::vec3(0.0f, 0.0f, 0.0f);

	Opacity = 1.0f;

	objectID = GetObjectID();
}

PixelWorldEngine::PixelObject::PixelObject(PixelWorldEngine::Transform transform)
	:PixelObject("", transform)
{
	name = DEFAULT_BASE_NAME + Utility::ToString(objectID);
}

PixelWorldEngine::PixelObject::~PixelObject()
{
	Utility::Delete(textInstance);

	ObjectCount--;
	FreeObjectID.push_back(objectID);
}

void PixelWorldEngine::PixelObject::SetDepth(int Depth)
{
	if (depth == Depth) return;

	depth = Depth;

	if (parent == nullptr) return;

	//更新深度后，其在父亲中的排序要改变
	parent->childrenDepthSort.erase(this);
	parent->childrenDepthSort.insert(this);
}

void PixelWorldEngine::PixelObject::SetSize(float Width, float Height)
{
	DebugReturn(DebugLayer::Assert(Width <= 0 || Height <= 0, Error::WidthOrHeightLessThanZero, name, FunctionName));

	if (width != Width || height != Height) isSizeChange = true;

	width = Width;
	height = Height;
	
	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;

	collider.SetArea(-halfWidth, -halfHeight, halfWidth, halfHeight);
}

void PixelWorldEngine::PixelObject::SetParent(PixelObject * Parent)
{
	DebugReturn(DebugLayer::Assert(Parent == nullptr, Error::TheObjectIsNull, "Parent", FunctionName));

	Parent->SetChild(this);
}

void PixelWorldEngine::PixelObject::SetChild(PixelObject * child)
{
	DebugReturn(DebugLayer::Assert(child == nullptr, Error::TheObjectIsNull, "child", FunctionName));

	if (child->parent != nullptr) child->parent->CancelChild(child->name);

	childrenNameIndex.insert(std::pair<std::string, PixelObject*>(child->name, child));
	childrenDepthSort.insert(child);

	child->parent = this;
}

void PixelWorldEngine::PixelObject::CancelChild(std::string name)
{
	DebugReturn(DebugLayer::Assert(childrenNameIndex.count(name) == 0, Error::TheObjectIsNull, name, FunctionName));

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
	DebugReturnWithValue(DebugLayer::Assert(childrenNameIndex.count(name) == 0, Error::TheNameIsNotExist, name, FunctionName), nullptr);

	return childrenNameIndex[name];
}

auto PixelWorldEngine::PixelObject::CreateFromInstance(std::string name, PixelObject * object) -> PixelObject *
{
	if (object == nullptr) return nullptr;

	auto result = new PixelObject(name);

	int id = result->objectID;

	memcpy(result, object, sizeof(PixelObject));

	result->name = name;
	result->objectID = id;

	return result;
}

bool PixelWorldEngine::Internal::PixelObjectCompare::operator()(PixelObject * object1, PixelObject * object2) const
{
	return object1->depth < object2->depth;
}

void PixelWorldEngine::Internal::PixelObjectProcess::ProcessUpdate(PixelObject * object)
{
	object->OnUpdate(object);

	Events::DoEventHandlers(object->Update, object);

	PixelObject::UpdateText(object);

	object->isSizeChange = false;

	for (auto it = object->childrenDepthSort.begin(); it != object->childrenDepthSort.end(); it++)
		ProcessUpdate(*it);
}

void PixelWorldEngine::Internal::PixelObjectProcess::ProcessAfterUpdate(PixelObject * object)
{
	object->OnAfterUpdate(object);

	Events::DoEventHandlers(object->AfterUpdate, object);

	for (auto it = object->childrenDepthSort.begin(); it != object->childrenDepthSort.end(); it++)
		ProcessAfterUpdate(*it);
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


