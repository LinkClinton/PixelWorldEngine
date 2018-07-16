#pragma once

#include <PixelWorld.hpp>

using namespace PixelWorldEngine;

class GameObject : public PixelObject {
private:
	glm::vec2 needTranslate; //记录需要移动的量
protected:
	virtual void OnMove(float translationX, float translationY)override; //移动事件

	virtual void OnCollide(void* sender, PixelObject* object)override; //碰撞事件
public:
	GameObject(std::string name); //构造函数

	void SetNeedTranslate(glm::vec2 translate); //设置需要移动的量
};