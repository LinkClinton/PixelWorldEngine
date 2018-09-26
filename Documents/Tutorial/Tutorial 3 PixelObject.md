# Tutorial 3 PixelObject

- Version 1.03beta

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
    player->Transform.SetPosition(glm::vec2());
    player->RenderObjectID = 1;

    world->SetPixelObject(player, Layer::World);
```

以上代码就是我们创建一个名字为`Player`，大小为64，位置为(0, 0)且渲染编号为1的物体。如果我们想要使用我们的物体，那么我们就必须将物体注册到世界中去。**但是需要注意的是，一个物体只能存在于一个世界中，如果被注册的物体已经在一个世界中了，那么我们就会先调用卸载函数，然后再调用注册函数。**

### 移动物体

当物体被加载到世界中去后，我们可以对物体进行移动，只需要修改Transform即可，但是要注意的是，如果物体开启了物理碰撞属性或者移动到的位置上有物体拥有这个属性，那么我们将会进行迭代(迭代次数可以自己设置，但是目前仍然没有去处理TOI问题)，使得物体能够移动到最接近目标位置的地方的同时也不会产生重叠。

```C++
	void Transform::SetPosition(glm::vec2());
```

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
	
	//位移
	if (translate != glm::vec2(0, 0)) {

		translate = glm::normalize(translate) * speed * deltaTime;

		player->Transform.SetPosition(player->Transform.GetPosition() + translate);
	}
}
```

上面的代码中，我们记录每一帧之间经过的时间，然后通过时间我们可以计算出位移的量，然后再去调用`Transform::SetPosition`即可完成我们的位移。当然我们还需要注意的是，有些时候我们位移物体的话，还需要考虑摄像机的位移(建议在AfterUpdate中考虑摄像机的位移，因为我们移动物体后因为物理碰撞的原因，在Update中考虑摄像机的话并没有办法保证摄像机位置的正确，甚至会产生抖动)。

### 物体的碰撞

我们提到过，我们的物体是支持进行碰撞检测的，但是仅限于同类物体(包括子类)。我们默认是关闭了物体实体碰撞的。

```C++
    void PixelObject::IsEnablePhysicsCollide = false;
```

以上函数就是用来设置我们的碰撞检测的状态的，当设置为`true`的时候，我们的物体移动的时候将会考虑其他物体，而如果设置为`false`那么就不会考虑， 而会直接穿过去。

而我们的物体碰撞到其他物体的话，就会产生`OnCollide`事件，我们可以通过`OnCollide`事件来进行一些简单效果实现。

```C++
void PixelObject::OnObjectCollide(PixelObject* sender, PixelObject* object);
void PixelObject::OnObjectEnter(PixelObject* sender, PixelObject* object);
void PixelObject::OnObjectLeave(PixelObject* sender, PixelObject* object);
```

### 物体的继承

如果对类熟悉的话，那么继承机制就是非常好用的一个东西，引擎提倡也推荐我们继承`PixelObject`来实现我们自己的物体类型。以下就是一个简单的继承例子。

```C++
class GameObject : public PixelObject {
private:
	glm::vec2 needTranslate; //记录需要移动的量
public:
	GameObject(std::string name); //构造函数

	void SetNeedTranslate(glm::vec2 translate); //设置需要移动的量
};
```

我们继承后，重载虚函数，用来处理事件。同样也可以增加一些函数，或者变量等等，原本的`PixelObject`类里面的所有变量和函数也都存在。

## 样例

- [EngineObject](https://github.com/LinkClinton/PixelWorldEngineSample/tree/master/EngineObject
)