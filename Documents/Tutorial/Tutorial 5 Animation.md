# Tutorial 5 Animation

- Version 1.03beta

## 目标

- 了解什么是关键帧动画
- 了解如何使用动画Animation
- 了解动画管理类Animator

## 关键帧

如果我们将动画细分的话，那么可以看作动画有一组动画帧构成，然而如果我们对于每个动画都存储如此多的帧数的话，其容量将会特别的大。为了避免这样的情况，就出现了关键帧动画。

什么是关键帧动画？简单来说，我们将一整个动画过程中较为重要的帧数记录下来，然后在我们实际播放动画的时候，通过运算来将中间过程的帧数数据计算出来，这样的话就可以减少大量的动画数据。

而关键帧在PixelWorldEngine中则是`KeyFrame`类型。

```C++
KeyFrame(T data, float time);
```

如果我们想要构建一个关键帧，那么我们就需要提供时间点，即这一帧在时间轴上的位置，以及对应的数据，我们可以看到对应数据是一个模板类型，因此其可以支持任意类型的数据，**但需要注意的是获取数据的时候指定的数据类型要和构建的时候指定的数据类型一致，而一组动画中的所有关键帧的数据类型也应该一致**。

```C++
KeyFrame::SetData(T data);
KeyFrame::GetData() -> T;
```

## 动画

动画指管理动画数据，以及如何通过关键帧计算出实际的帧。

```C++
Animation::Animation(name, FrameProcessUnit);
```

- `name`: 动画的名字
- `FrameProcessUnit`: 获取实际帧的方法。

### FrameProcessUnit

其为一个函数，定义如下。

```C++
typedef std::function<KeyFrame(float, KeyFrame, KeyFrame)> FrameProcessUnit;
```

简单来说只需要编写一个参数和返回值如何要求的函数，然后将其设置到动画中去即可。

- 第一个参数：表示的是当前的时间点。
- 第二个参数：表示的是比这个时间点小但最接近这个时间点的关键帧。
- 第三个参数：表示的是比这个时间点大但最接近这个时间点的关键帧。
- 返回值：这是这个时间点对应的帧。

以下为默认的函数。

```C++
auto PixelWorldEngine::Animation::DefaultProcessUnit(float timePos, KeyFrame lastFrame, KeyFrame nextFrame) -> KeyFrame
{
	if (timePos == nextFrame.timePos) return nextFrame;
	return lastFrame;
}
```

其就是返回比当前时间点小但是最接近当前时间点的关键帧。

### 添加关键帧

我们并不需要自己构造关键帧然后添加进去，我们只需要使用`Animation::SetKeyFrame`即可添加一个关键帧进去，当我们将所有需要的关键帧添加进去后，则需要调用`Animation::Sort`将其按照时间点排序(注意不要添加一样时间点的关键帧，以及关键帧最少要有两个)。

```C++
animation[id]->SetKeyFrame(glm::vec2(startXpos, startYpos), 0);
animation[id]->SetKeyFrame(subObject[id]->Transform.GetPosition(), 5);
animation[id]->SetKeyFrame(subObject[id]->Transform.GetPosition(), 8);
animation[id]->SetKeyFrame(glm::vec2(startXpos, startYpos), 13);
animation[id]->Sort();
```

以上就是一个例子。

## 动画管理类型

之前说过动画仅仅只是存储数据，而要真实的进行动画的播放的话，我们还需要使用管理类型。一个动画管理类型可以管理多个动画。

```C++
AddAnimation(void* target, AnimationSetFunction function, Animation* animation, float startTime = 0);
```

这是我们添加一个动画的方式。

- `target`: 表示我们的动画的作用对象。
- `function`: 表示我们要如何将动画数据作用到我们的对象，是一个函数。
- `animation`: 动画数据。
- `startTime`: 起始时间，可以简单的理解这个动画在时间轴上开始的位置。

### AnimationSetFunction

我们的动画数据要应用到具体的实例中去，而可能我们并不是简单的进行值的改变，因此我们需要使用这个函数来让我们可以做到更多的内容。其定义如下。

```C++
typedef std::function<void(void*, void*)> AnimationSetFunction;
```

- 第一个参数：表示作用对象。
- 第二个参数：则是对应的数据。

以下为一个例子。

```C++
void RotateAnimation::RotateSetFunction(void * sender, void * data)
{
	auto object = (PixelObject*)sender;
	auto angle = *(float*)data;

	object->Transform.SetRotate(angle);
}
```

### 注册然后运行

当我们将动画添加完毕后，我们只需要使用`Application::RegisterAnimator`即可将动画添加，然后调用`Animator::Run`即可运行动画。

## 样例

- [EngineAnimation](https://github.com/LinkClinton/PixelWorldEngineSample/tree/master/EngineAnimation
)