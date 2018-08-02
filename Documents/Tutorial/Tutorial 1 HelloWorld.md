# Tutorial 1 HelloWorld

- Version 1.03beta

## 目标

- 学会如何使用引擎创建一个窗口。
- 了解引擎中的Application的作用和定位。
- 了解事件。

## 应用程序

Application是引擎中管理窗口以及具体呈现的类型，他的主要用途是创建窗口，将我们渲染的世界呈现到窗口上去，同时他也管理者我们整个图形渲染接口。

### 创建

创建Application是一个非常简单的工作，我们只需要使用其构造函数即可。其只有一个参数，即应用程序的名字。

```C++
    Application* app = new Application(u8"Application"); 
```

### 创建窗口

我们想要将我们的游戏内容呈现出来，那么一个窗口是不可避免的。我们可以使用Application来创建我们的游戏窗口，但是需要注意的是我们只能够创建一个窗口。

```C++
    app->MakeWindow(u8"Hello, World!", 800, 600, "");
```

以上我们创建了一个宽度为800，高度为600，窗口标题为`Hello, World!`的窗口。需要注意的是，其大小是指我们去除了标题后的大小，因此实际的高度会比我们设置的要大。

而如果我们想修改窗口信息，可以使用`Application::SetWindow`来修改。具体可以参见文档。

而在我们创建完窗口后，窗口并不是可见的，我们需要显示他。

```C++
    app->ShowWindow();
```

当然对应的还有`Application::HideWinodw`。

### 事件

游戏中我们经常使用鼠标以及键盘，当我们按下的时候，程序会做出一些反应，例如人物移动，摄像机移动等等情况，又或者我们需要在每一帧之间更新一些东西。那么我们就需要使用事件来做到。这里我们以鼠标按钮事件为例。

首先，我们需要编写一个函数，用来处理事件，但是需要注意的是，函数的参数必须按照规定来编写，具体要求可以参见文档。

```C++
void OnMouseClick(void* sender, Events::MouseClickEvent* eventArg) {

	if (eventArg->isDown == true) {
		app->SetWindow(windowName + u8"    MouseButton was down", 800, 600);
	}
	else app->SetWindow(windowName + u8"    MouseButton was up", 800, 600);

}
```

- `sender`: 触发消息的主体，对于现在，我们可以认为是Application。
- `eventArg`: 事件触发的时候的数据消息，例如鼠标的位置，是否按下按钮等等。

我们编写好处理事件的函数后，我们还需要将其加入到事件数组中去，从而才能够在事件发生的时候调用函数。

```C++
    app->MouseClick.push_back(OnMouseClick);
```

MouseClick是一个动态数组，用来存储所有的关于鼠标按钮的处理函数。而我们可以发现，我们的函数并没有加上`()`，这意味着我们返回的是函数的地址，而不是调用这个函数。

### 主循环

当我们一切预处理完成后，我们就将进入主循环了，在主循环中，除非窗口被销毁，不然我们将会一直处于主循环过程中。并且在主循环过程中，我们将会处理各种事件，以及进行渲染等等操作。

```C++
    app->RunLoop();
```

### 样例

- [EngineHelloWorld](https://github.com/LinkClinton/PixelWorldEngineSample/tree/master/EngineHelloWorld)
