# Tutorial 4 Text

## 目标

- 了解字体(Font)类。
- 了解如何通过字体构建文本纹理(Text)。
- 了解UI物体(UIObject)并学会使用。
- 了解UI物体的父子关系。

## 字体

我们知道，同样的文字因为书写风格的不同，其渲染出来的样子也不会相同，而字体就是方便我们呈现不同风格的文字而产生的。文字本身是靠纹理来进行渲染，但是我们不可能将所有的文字还存储各个大小版本的纹理，因此字体的存储通常都是矢量图，然后在我们创建的时候需要指定大小然后在需要的时候创建出对应的纹理。而如果我们想要渲染文本，即一串文字，则不能将纹理进行简单的拼凑，因为不同的文字可能其间距不同，以及基准线的原因。引擎本身是提供了一个简单的文本渲染方案，但是如果觉得不满意你也可以使用自己的方案。

### 创建字体

和创建纹理类似，我们可以通过内存数据创建出纹理，也可以通过文件来创建纹理，这里我们介绍使用文件来创建纹理。同样我们需要使用到`DataManager`来进行创建。

```C++
    auto font = dataManager->RegisterFont(fileName, fontName, fontSize);
```

- `fileName`: 即字体文件的路径。
- `fontName`: 用于标识不同的字体，注意不能重复。
- `fontSize`: 创建的字体的大小。

### 获取文字的纹理

我们可以通过提供文字获取其对应的纹理，但是需要注意的是，纹理只有关于这个文字的数据信息，而没有排版的数据信息，因此不同的文字对应的纹理其宽度或者高度都有可能不同。

```C++
    auto characterCodeMetrics = font->GetCharacterCodeMetrics(code);
```

以上函数的参数是一个UTF-16编码的字符，为什么是UTF-16？因为如果使用UTF8的话，那么单个字符需要多个byte才能够描述，因此我们使用的是UTF-16，既可以将所有常用的文字通过一个变量表达。而返回值则是关于这个文字的纹理其对应的数据。具体可以去参见文档或者源码。

## 文本

因为文本的排版比较的麻烦，因此引擎提供了一个简单的排版类型。

### 创建文本

```C++
    auto text = new Text(str, font, width, height);
```

- `str`: 文本内容，注意编码为UTF-8。
- `font`：使用的字体。
- `width`: 文本的宽度限制。
- `height`: 文本的高度限制，如果设置为0的话，那么就由引擎来计算出合适的高度。

如果我们的文本内容太长，导致在限制内无法渲染完成，那么超出部分将不会被渲染。

### 使用文本

我们创建文本后，如果想要使用文本的话，只需要获取其对应的纹理，然后将其加入到合并纹理中去，当作普通的纹理使用就可以了。但是有两点需要注意。

- 文本纹理的格式是A8类型，因为我们没有存储颜色数据，因此我们的合并纹理的格式也必须是A8格式。
- 文本的颜色在具体渲染的时候才会决定，由`Object::SetEffectColor`决定。

```C++
    auto texture = text->GetTexture();
```

## UI物体

UI物体和像素物体(PixelObject)两者都继承自Object，但是和像素物体不同的是，UI物体的渲染并不和摄像机有关系，并且UI物体也多了一些特性。不过UI物体还是需要加入到世界中去才能渲染，以及没有了物理属性。

### 创建UI物体

```C++
void MakeObject() {
	object = new UIObject("UIObject");  //构建UI物体
	objectParent = new UIObject("UIObjectParent");

	float centerX = width * 0.5f; //计算中心位置
	float centerY = height * 0.5f;

	objectParent->SetSize((float)text->GetWidth() * 2, (float)text->GetWidth() * 2); //设置父亲物体的大小为文本宽度的两倍
	objectParent->SetPosition(centerX - objectParent->GetWidth() * 0.5f, centerY - objectParent->GetHeight() * 0.5f); //设置父亲物体的位置为中心
	objectParent->SetAngle(glm::pi<float>() * 0.25f); //设置旋转角度，旋转中心为物体中心，会对子物体造成影响

	objectParent->SetRenderObjectID(0); //由于不渲染纹理，因此设置为0
	objectParent->SetOpacity(0.7f); //设置不透明度，会对子物体造成影响

	objectParent->SetBorderWidth(1); //设置边框宽度
	objectParent->SetBorderColor(0, 1, 0); //设置边框颜色
	

	object->SetSize((float)text->GetWidth(), (float)text->GetHeight()); //设置大小和文本纹理一样大小
	object->SetPosition(objectParent->GetWidth() * 0.5f - object->GetWidth() * 0.5f, objectParent->GetHeight() * 0.5f - object->GetHeight() * 0.5f); //设置位置为中心
	object->SetAngle(-glm::pi<float>() * 0.25f); //设置旋转角度
	
	object->SetRenderObjectID(1); //将渲染编号设置为文本纹理的
	object->SetEffectColor(1, 0, 0); //设置文本颜色，默认为(1, 1, 1)，其本质是对纹理颜色进行一个乘法
	
	object->SetBorderWidth(1); //设置边框宽度，默认为0
	object->SetBorderColor(1, 0, 0); //设置边框颜色



	objectParent->RegisterUIObject(object); //注册子物体

	world->RegisterUIObject(objectParent); //注册UI物体
}

```

以上是一个创建UI物体的例子，一个UI物体同样可以有其子物体，简单的认为就是我们将对UI物体进行的变换其子物体也会继承，或者这样想，我们将子物体放到了UI物体上，那么UI物体进行移动或者旋转的话，子物体也会因此而进行移动和旋转(注意旋转中心还是UI物体的)。

### 关于EffectColor

对于继承自Object的物体都有一个EffectColor属性，其本身是一个额外的拓展，主要是用于渲染文本的时候设置文本的颜色。其计算公式是将我们对纹理进行采样得到的颜色最后和EffectColor进行相乘。由于文本纹理只有Alpha属性，因此其返回值是(1, 1, 1, A)，所以乘起来我们就可以得到对应的颜色了。

## 样例

- [EngineText](https://github.com/LinkClinton/PixelWorldEngineSample/tree/master/EngineText)