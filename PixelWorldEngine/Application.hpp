#pragma once

#include "pch.hpp"

#include "KeyCode.hpp"
#include "Events.hpp"

#include "Graphics.hpp"
#include "GraphicsRenderTarget.hpp"

#include "Timer.hpp"
#include "PixelWorld.hpp"

#include "Animation.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief 主要应用程序
	 */
	class Application {
	private:
		std::string applicationName; //应用程序的名字

		int width = 0; //窗口的宽度
		int height = 0; //窗口的高度

		std::string windowName; //窗口的名字
		std::string iconName; //窗口使用的图标文件路径

		bool isWindowCreated = false; //是否已经创建了窗口
		bool isFocused = false; //窗口是否获取了焦点

		int mousePositionX; //鼠标位置Y
		int mousePositionY; //鼠标位置X

		int mousePositionXRelative; //鼠标位置X
		int mousePositionYRelative; //鼠标位置Y

		float deltaTime; //每次更新经过的时间

		Timer timer; //计时器
		FpsCounter fpsCounter; //fps计数器

		Graphics::Graphics* graphics; //图形接口
		Graphics::RenderTarget* renderTarget; //渲染目标

		Graphics::Buffer* cameraBuffer; //摄像机矩阵缓冲
		Graphics::GraphicsShader* defaultShader; //默认的着色器
		Graphics::StaticSampler* defaultSampler; //默认的采样器

		std::map<std::string, Animator*> animators; //动画管理类

		PixelWorld* pixelWorld; //使用的世界

#ifdef _WIN32
		HWND hwnd = nullptr; //窗口句柄

		std::wstring wideWindowName; //窗口宽字符下的名字
		std::wstring wideIconName; //宽字符下的图标文件路径

		static LRESULT CALLBACK DefaultWindowProc(HWND hWnd, UINT message,
			WPARAM wParam, LPARAM lParam); //默认消息处理函数
#endif // _WIN32

	public:

#ifdef _WIN32
		DXGI_SWAP_CHAIN_DESC swapDesc; //交换链数据信息

		IDXGISwapChain* swapChain; //交换链


#endif //_WIN32

	private:
		/**
		 * @brief 当鼠标移动的时候触发
		 * @param[in] sender 谁触发了
		 * @param[in] eventArg 鼠标移动的消息信息，这里的鼠标位置是真实位置
		 */
		void OnMouseMove(void* sender, PixelWorldEngine::Events::MouseMoveEvent* eventArg);

		/**
		* @brief 当鼠标滚轮滑动的时候触发
		* @param[in] sender 谁触发了
		* @param[in] eventArg 鼠标滚轮滑动的消息信息，这里的鼠标位置是真实位置
		*/
		void OnMouseWheel(void* sender, PixelWorldEngine::Events::MouseWheelEvent* eventArg);

		/**
		* @brief 鼠标按键信息处理
		* @param[in] sender 谁触发了
		* @param[in] eventArg 鼠标按下的消息信息，这里的鼠标位置是真实位置
		*/
		void OnMouseClick(void* sender, PixelWorldEngine::Events::MouseClickEvent* eventArg);

		/**
		* @brief 键盘按键信息处理
		* @param[in] sender 谁触发了
		* @param[in] eventArg 键盘按键信息
		*/
		void OnKeyClick(void* sender, PixelWorldEngine::Events::KeyClickEvent* eventArg);

		/**
		 * @brief 当窗口大小改变的时候触发
		 * @param[in] sender 谁触发了
		 * @param[in] eventArg 窗口大小改变信息
		 */
		void OnSizeChange(void* sender, PixelWorldEngine::Events::SizeChangeEvent* eventArg);

		/**
		 * @brief 更新函数
		 * @param[in] sender 谁触发了
		 */
		void OnUpdate(void* sender);

		/**
		 * @brief 渲染函数
		 * @param[in] sender 谁触发了
		 */
		void OnRender(void* sender);

#ifdef _WIN32
		/**
		 * @brief Windows消息处理函数
		 * @param[in] message 消息
		 */
		void OnProcessMessage(MSG message);
#endif // _WIN32

		/**
		 * @brief 计算鼠标位置转移到当前世界使用的分辨率下的位置
		 * @param[in] x 鼠标位置X
		 * @param[in] y 鼠标位置Y
		 * @return 转换后的鼠标位置
		 */
		auto ComputeMousePosition(int x, int y) -> std::pair<int, int>;

		/**
		 * @brief 根据分辨率以及窗口大小计算我们的可视范围
		 * @param[in] windowWidth 窗口宽度
		 * @param[in] windowHeight 窗口高度
		 * @param[in] resolutionWidth 分辨率宽度
		 * @param[in] resolutionHeight 分辨率高度
		 * @return 返回可视范围
		 */
		static auto ComputeViewPort(int width, int height, int resolutionWidth, int resolutionHeight) -> RectangleF;

		/**
		* @brief 计算鼠标位置转移到当前世界使用的分辨率下的位置
		* @param[in] ViewPort 视角范围
		* @param[in] resolutionWidth 分辨率宽度
		* @param[in] resolutionHeight 分辨率高度
		* @param[in] x 鼠标位置X
		* @param[in] y 鼠标位置Y
		* @return 转换后的鼠标位置
		*/
		static auto ComputeMousePosition(RectangleF ViewPort, int resolutionWidth, int resolutionHeight, int x, int y) -> std::pair<int, int>;
		
		friend class Input;
		friend class Animator;
	public:
		PixelWorldEngine::Events::MouseMoveHandlers MouseMove; //鼠标移动事件
		PixelWorldEngine::Events::MouseClickHandlers MouseClick; //鼠标按键事件
		PixelWorldEngine::Events::MouseWheelHandlers MouseWheel; //鼠标滚轮事件
		PixelWorldEngine::Events::KeyClickEventHandlers KeyClick; //键盘按键事件
		PixelWorldEngine::Events::UpdateEventHandlers Update; //更新事件
	public:
		/**
		 * @brief 构造函数
		 * @param[in] ApplicationName 应用程序名字
		 */
		Application(std::string ApplicationName);

		/**
		 * @brief 析构函数
		 */
		~Application();

		/**
		 * @brief 创建窗口，同时最多只能存在一个窗口
		 * @param[in] WindowName 窗口名字
		 * @param[in] Width 窗口宽度
		 * @param[in] Height 窗口高度
		 * @param[in] IconName 图标路径
		 */
		void MakeWindow(std::string WindowName, int Width, int Height, std::string IconName = "");

		/**
		 * @brief 设置窗口属性
		 * @param[in] WindowName 窗口的名字
		 * @param[in] Width 窗口的宽度
		 * @param[in] Height 窗口的高度
		 */
		void SetWindow(std::string WindowName, int Width, int Height);

		/**
		 * @brief 设置全屏
		 * @param[in] state 是否全屏
		 */
		void MakeFullScreen(bool state);

		/**
		 * @brief 显示窗口
		 */
		void ShowWindow();

		/**
		 * @brief 隐藏窗口
		 */
		void HideWindow();

		/**
		 * @brief 主循环
		 */
		void RunLoop();

		/**
		 * @brief 设置使用的世界
		 * @param[in] pixelWorld 世界
		 */
		void SetWorld(PixelWorld* pixelWorld);

		/**
		 * @brief 注册动画管理类型
		 * @param[in] animator 动画管理类
		 */
		void RegisterAnimator(Animator* animator);

		/**
		 * @brief 卸载动画管理类型
		 * @param[in] animator 动画管理类
		 */
		void UnRegisterAnimator(Animator* animator);

		/**
		 * @brief 卸载动画管理类
		 * @param[in] name 动画管理类的名字
		 */
		void UnRegisterAnimator(std::string name);

		/**
		 * @brief 获取窗口宽度
		 * @return 窗口宽度
		 */
		auto GetWindowWidth() -> int;

		/**
		 * @brief 获取窗口高度
		 * @return 窗口宽度
		 */
		auto GetWindowHeight() -> int;

		/**
		 * @brief 获取fps
		 * @return fps
		 */
		auto GetFramePerSecond() -> int;

		/**
		 * @brief 获取更新的时间间隔
		 * @return 时间，单位秒
		 */
		auto GetDeltaTime() -> float;

		/**
		 * @brief 获取图形接口
		 * @return 图形接口的指针
		 */
		auto GetGraphics() -> Graphics::Graphics*;

		/**
		 * @brief 窗口是否获取到了焦点
		 * @return 是否得到焦点
		 */
		auto IsFocused() -> bool;
	};

}
