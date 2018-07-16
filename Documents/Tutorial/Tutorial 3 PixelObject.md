# Tutorial 3 PixelObject

## 目标

- 了解什么是物体(PixelObject)。
- 了解物体的移动以及碰撞检测。
- 了解物体的继承。

## 物体

对于我们的游戏世界来说，物体就是游戏中的元素，无论是人物还是物件，都可以认为是物体。物体同样也有其渲染编号，用于告知我们要如何渲染我们的物体，同时他也有他的位置信息以及大小信息。如果我们需要进行一些碰撞检测的话，那么我们也可以开启碰撞检测，来获取碰撞信息。

### 创建物体

对于一个物体，我们可以使用构造函数创建它，但是需要注意的是，在使用它的时候要保持其生命周期。

```C++
    PixelObject(std::string Name, float PositionX = 0.f, float PositionY = 0.f, float Width = 1.f, float Height = 1);
```

- `Name`: 表示物体的名字，请注意不要重复。
- `PositionX`: 表示物体的左上角的X坐标。
- `PositionY`: 表示物体的左上角的Y坐标。
- `Width`: 表示物体的宽度。
- `Height`: 表示物体的高度。

而通常我们创建一个物体是这样的。

```C++
    player = new PixelObject("Player");

    player->SetSize(64, 64);
    player->SetPosition(0, 0);
    player->SetRenderObjectID(1);

    world->RegisterPixelObject(player);
```

以上代码就是我们创建一个名字为`Player`，大小为64，位置为(0, 0)且渲染编号为1的物体。如果我们想要使用我们的物体，那么我们就必须将物体注册到世界中去。**但是需要注意的是，一个物体只能存在于一个世界中，如果被注册的物体已经在一个世界中了，那么我们就会先调用卸载函数，然后再调用注册函数。**

### 移动物体

当物体被加载到世界中去后，我们可以对物体进行移动，移动方式有两种，第一种是单纯的修改其位置，第二种则是考虑世界中的物体碰撞以及禁止移动区域。我们在前面的内容中知道`MapData`有一个`MoveEnable`属性，则是用来限制一个物体是否能够移动到这一个地图块的。以及如果移动到的位置已经有一个物体的话，那么我们是否还需要移动，同样也是需要考虑的问题。

所幸的是，引擎已经提供了这样的处理函数。

```C++
    void PixelObject::SetPosition(float x, float y); //表示单纯的修改物体的位置
    void PixelObject::Move(float translationX, float translationY); //移动的时候考虑其他物体以及地图块属性
```

上面两个函数是我们修改位置信息的两个方法，前者是单纯修改位置信息，后者则是通过位移来进行移动，并判断我们的移动是否合法，也就是说即便我们提供了位移数据，物体也未必会位移那么多，因为可能被其他物体阻挡住了。

而如何实现物体的位移，这里我们提供一部分参考。

```C++
void OnUpdate(void* sender) {

	auto deltaTime = app->GetDeltaTime(); //获取经过的时间

	glm::vec2 translate = glm::vec2(0, 0); //位移

	float speed = 200; //速度

	//根据WSAD移动
	if (Input::GetKeyCodeDown(KeyCode::A) == true)
		translate.x -= 1;
	if (Input::GetKeyCodeDown(KeyCode::D) == true)
		translate.x += 1;
	if (Input::GetKeyCodeDown(KeyCode::S) == true)
		translate.y += 1;
	if (Input::GetKeyCodeDown(KeyCode::W) == true)
		translate.y -= 1;

    //根据方向键来调整高度
	if (Input::GetKeyCodeDown(KeyCode::Up) == true)
		cameraTimes += deltaTime * cameraSpeed;
	if (Input::GetKeyCodeDown(KeyCode::Down) == true)
		cameraTimes -= deltaTime * cameraSpeed;

	cameraTimes = Utility::Limit(cameraTimes, 0.5f, 2.0f);

	//位移
	if (translate != glm::vec2(0, 0)) {

		translate = glm::normalize(translate) * speed * deltaTime;

		player->SetNeedTranslate(translate);
		player->Move(translate.x, translate.y);
	}

	//移动摄像机，将物体作为中心点
	camera.SetFocus(player->GetPositionX() + player->GetWidth() * 0.5f,
		player->GetPositionY() + player->GetHeight() * 0.5f,
		RectangleF(cameraWidth * 0.5f * cameraTimes, cameraHeight * 0.5f * cameraTimes, cameraWidth * 0.5f * cameraTimes, cameraHeight * 0.5f * cameraTimes));
}
```

上面的代码中，我们记录每一帧之间经过的时间，然后通过时间我们可以计算出位移的量，然后再去调用`PixelObject::Move`即可完成我们的位移。当然我们还需要注意的是，有些时候我们位移物体的话，还需要考虑摄像机的位移。

### 物体的碰撞

我们提到过，我们的物体是支持进行碰撞检测的，但是仅限于同类物体(包括子类)。我们默认是开启了物体实体碰撞的，即我们不能够穿过物体，但是我们同样可以取消。

```C++
    void PixelObject::EnablePhysicsCollision(bool enable);
```

以上函数就是用来设置我们的碰撞检测的状态的，当设置为`true`的时候，我们的物体移动的时候将会考虑其他物体，而如果设置为`false`那么就不会考虑， 而会直接穿过去。

而我们的物体碰撞到其他物体的话，就会产生`OnCollide`事件，我们可以通过`OnCollide`事件来进行一些简单效果实现。例如下面的例子。

```C++
void GameObject::OnCollide(void * sender, PixelObject * object)
{
	GameObject* self = (GameObject*)sender;
	GameObject* gameObject = (GameObject*)object;

	gameObject->SetNeedTranslate(needTranslate);
	gameObject->Move(needTranslate.x, needTranslate.y);
}

```

`GameObject`是`PixelObject`的子类，我们重新编写了OnCollide函数，以上函数作用是当我们碰撞到一个物体的时候，将碰撞的物体也同样移动，使得我们可以简单的实现一个物体碰撞移动的过程。

### 物体的继承

如果对类熟悉的话，那么继承机制就是非常好用的一个东西，引擎提倡也推荐我们继承`PixelObject`来实现我们自己的物体类型。以下就是一个简单的继承例子。

```C++
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
```

我们继承后，重载虚函数，用来处理事件。同样也可以增加一些函数，或者变量等等，原本的`PixelObject`类里面的所有变量和函数也都存在。

## 样例

- [EngineObject](https://github.com/LinkClinton/PixelWorldEngineSample/tree/master/EngineObject
)