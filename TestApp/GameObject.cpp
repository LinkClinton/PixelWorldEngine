#include "GameObject.hpp"

void GameObject::OnMove(float translationX, float translationY)
{
}

void GameObject::OnCollide(void * sender, PixelObject * object)
{
	GameObject* self = (GameObject*)sender;
	GameObject* gameObject = (GameObject*)object;

	gameObject->SetNeedTranslate(needTranslate);
	gameObject->Move(needTranslate.x, needTranslate.y);
}

GameObject::GameObject(std::string name): PixelObject(name)
{
	needTranslate = glm::vec2(0, 0);
}

void GameObject::SetNeedTranslate(glm::vec2 translate)
{
	needTranslate = translate;
}
