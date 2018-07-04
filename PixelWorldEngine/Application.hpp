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

	class Application {
	private:
		std::string applicationName;

		int windowWidth = 0;
		int windowHeight = 0;

		std::string windowName;
		std::string iconName;

		bool isWindowCreated = false;
		bool isFocused = false;

		int mousePositionX;
		int mousePositionY;

		int mousePositionXRelative;
		int mousePositionYRelative;

		float deltaTime;

		Timer timer;
		FpsCounter fpsCounter;

		Graphics::Graphics* graphics;
		Graphics::RenderTarget* renderTarget;

		Graphics::Buffer* cameraBuffer;
		Graphics::GraphicsShader* defaultShader;
		Graphics::StaticSampler* defaultSampler;

		std::map<std::string, Animator*> animators;

		PixelWorld* pixelWorld;

#ifdef _WIN32
		HWND hwnd = nullptr;

		static LRESULT CALLBACK DefaultWindowProc(HWND hWnd, UINT message,
			WPARAM wParam, LPARAM lParam);
#endif // _WIN32

	public:

#ifdef _WIN32
		DXGI_SWAP_CHAIN_DESC swapDesc;

		IDXGISwapChain* swapChain;


#endif //_WIN32

	private:
		void OnMouseMove(void* sender, PixelWorldEngine::Events::MouseMoveEvent* eventArg);

		void OnMouseWheel(void* sender, PixelWorldEngine::Events::MouseWheelEvent* eventArg);

		void OnMouseClick(void* sender, PixelWorldEngine::Events::MouseClickEvent* eventArg);

		void OnKeyClick(void* sender, PixelWorldEngine::Events::KeyClickEvent* eventArg);

		void OnSizeChange(void* sender, PixelWorldEngine::Events::SizeChangeEvent* eventArg);

		void OnUpdate(void* sender);

		void OnRender(void* sender);

#ifdef _WIN32
		void OnProcessMessage(MSG message);
#endif // _WIN32

		auto ComputeMousePosition(int x, int y) -> std::pair<int, int>;

		static auto ComputeViewPort(int windowWidth, int windowHeight, int resolutionWidth, int resolutionHeight) -> RectangleF;

		static auto ComputeMousePosition(RectangleF ViewPort, int resolutionWidth, int resolutionHeight, int x, int y) -> std::pair<int, int>;
		
		friend class Input;
		friend class Animator;
	public:
		PixelWorldEngine::Events::MouseMoveHandlers MouseMove;
		PixelWorldEngine::Events::MouseClickHandlers MouseClick;
		PixelWorldEngine::Events::MouseWheelHandlers MouseWheel;
		PixelWorldEngine::Events::KeyClickEventHandlers KeyClick;
		PixelWorldEngine::Events::UpdateEventHandlers Update;
	public:
		Application(std::string ApplicationName);

		~Application();

		void MakeWindow(std::string WindowName, int Width, int Height, std::string IconName = "");

		void SetWindow(std::string WindowName, int Width, int Height);

		void MakeFullScreen(bool state);

		void ShowWindow();

		void HideWindow();

		void RunLoop();

		void SetWorld(PixelWorld* pixelWorld);

		void RegisterAnimator(Animator* animator);

		void UnRegisterAnimator(Animator* animator);

		void UnRegisterAnimator(std::string name);

		auto GetWindowWidth() -> int;

		auto GetWindowHeight() -> int;

		auto GetFramePerSecond() -> int;

		auto GetDeltaTime() -> float;

		auto GetGraphics() -> Graphics::Graphics*;

		auto IsFocused() -> bool;
	};

}
