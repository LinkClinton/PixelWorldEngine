# PixelWorldEngine

版本: 1.00

## 介绍

PixelWorldEngine是由C++编写的用于开发2D像素类型的游戏引擎，主要目的是用于开发开放世界。不知道简单也不知道是否好用QAQ。

## 使用注意

- 请使用UTF-8编码

## 配置需求

- 目前支持的系统：Windows 7, Windows 8, Windows 10
- 最低内存需求：2GB
- 最低显卡需求：支持DirectX 10.1
- 环境需求：Visual Studio 2017(Windows版本), Python3

## 如何配置

首先打开`gitbash`，移动到想要放置引擎的路径，然后键入以下指令

```
git clone https://github.com/LinkClinton/PixelWorldEngine.git
```

由于考虑到配置方便问题，因此也将使用的第三方库上传了，因此文件可能有点大。

然后将[这里的](https://github.com/LinkClinton/PixelWorldEngineSample)`ThirdParty`拷贝到你的C++项目的文件目录。

然后打开你自己的C++项目，添加已有项目，选择引擎项目，然后在你的C++项目中添加引用。

最后设置你的C++项目的包含路径(Include Directories)以及静态库路径(Library Directories)。以下为路径参考，当然你也可以参考TestApp以及Sample的设置(注意选择x86而不是x64)。

- 包含路径，注意引擎的头文件目录也需要设置

```
$(SolutionDir)ThirdParty\freetype\include
$(SolutionDir)ThirdParty
```

- 静态库路径

```
$(SolutionDir)ThirdParty\freetype\win32
$(SolutionDir)ThirdParty\SDL2\x86
```

## 第三方库

- [SDL2](http://www.libsdl.org)
- [FreeType](https://www.freetype.org)
- [glm](https://glm.g-truc.net)

## 教程以及样例

- **[教程](../Documents/Tutorial)**
- **[样例](https://github.com/LinkClinton/PixelWorldEngineSample)**