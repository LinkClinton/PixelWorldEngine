#include "PixelObject.hpp"

PixelWorldEngine::PixelObject::PixelObject(std::string Name, PixelWorldEngine::Transform transform)
{
	name = Name;

	parent = nullptr;

	depth = 0;

	Transform = transform;

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

void PixelWorldEngine::PixelObject::SetParent(PixelObject * Parent)
{
	Parent->SetChild(this);
}

void PixelWorldEngine::PixelObject::SetChild(PixelObject * child)
{
	if (child->parent != nullptr) child->parent->CancelChild(child->name);

	childrenNameIndex.insert(std::pair<std::string, PixelObject*>(child->name, child));
	childrenDepthSort.insert(child);

	child->parent = this;
}

void PixelWorldEngine::PixelObject::CancelChild(std::string name)
{
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

auto PixelWorldEngine::PixelObject::GetParent() -> PixelObject *
{
	return parent;
}

auto PixelWorldEngine::PixelObject::GetChildren(std::string name) -> PixelObject *
{
	return childrenNameIndex[name];
}

bool PixelWorldEngine::PixelObjectCompare::operator()(PixelObject * object1, PixelObject * object2) const
{
	return object1->GetDepth() < object2->GetDepth();
}
