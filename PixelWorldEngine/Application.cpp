#include "Application.hpp"

#include "EngineDefaultResource.hpp"

#ifdef _DEBUG

#include <iostream>

#endif // _DEBUG


#ifdef _WIN32

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

PixelWorldEngine::Application* self;

LRESULT PixelWorldEngine::Application::DefaultWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE: {

		auto eventArg = &Events::SizeChangeEvent();

		eventArg->width = LOWORD(lParam);
		eventArg->height = HIWORD(lParam);

		self->OnSizeChange(self, eventArg);

		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);;
}

#endif // _WIN32

void PixelWorldEngine::Application::OnMouseMove(void * sender, PixelWorldEngine::Events::MouseMoveEvent * eventArg)
{
	mousePositionX = eventArg->x;
	mousePositionY = eventArg->y;
}

void PixelWorldEngine::Application::OnMouseWheel(void * sender, PixelWorldEngine::Events::MouseWheelEvent * eventArg)
{
}

void PixelWorldEngine::Application::OnMouseClick(void * sender, PixelWorldEngine::Events::MouseClickEvent * eventArg)
{

}

void PixelWorldEngine::Application::OnKeyClick(void * sender, PixelWorldEngine::Events::KeyClickEvent * eventArg)
{

}

void PixelWorldEngine::Application::OnSizeChange(void * sender, PixelWorldEngine::Events::SizeChangeEvent * eventArg)
{
	windowWidth = eventArg->width;
	windowHeight = eventArg->height;
	
	Utility::Delete(renderTarget);

#ifdef _WIN32
	swapChain->ResizeBuffers(1, windowWidth, windowHeight, (DXGI_FORMAT)Graphics::PixelFormat::Unknown, 0);
#endif // _WIN32

	renderTarget = new Graphics::RenderTarget(graphics, this);
}

void PixelWorldEngine::Application::OnUpdate(void * sender)
{
	OnRender(sender);
}

void PixelWorldEngine::Application::OnRender(void * sender)
{

	auto worldTexture = pixelWorld->GetCurrentWorld();

	auto resolutionWidth = worldTexture->GetWidth();
	auto resolutionHeight = worldTexture->GetHeight();

	auto matrix = Camera(Rectangle(0.f, 0.f, (float)resolutionWidth, (float)resolutionHeight)).GetMatrix();

	cameraBuffer->Update(&matrix);

	renderTarget->Clear(0, 0, 0);

	graphics->ClearState();

	graphics->SetRenderTarget(renderTarget);

	graphics->SetViewPort(Application::ComputeViewPort(windowWidth, windowHeight,
		resolutionWidth, resolutionHeight));

	graphics->SetShader(defaultShader);

	graphics->SetVertexBuffer(pixelWorld->renderObject->GetVertexBuffer());
	graphics->SetIndexBuffer(pixelWorld->renderObject->GetIndexBuffer());

	graphics->SetConstantBuffer(cameraBuffer, 0);
	graphics->SetShaderResource(worldTexture, 0);
	graphics->SetStaticSampler(defaultSampler, 0);

	graphics->DrawIndexed(pixelWorld->renderObject->GetIndexBuffer()->GetCount());

#ifdef _WIN32

	auto result = swapChain->Present(0, 0);

#endif // _WIN32

#ifdef LIUNX
	//交换后台缓冲
#endif // LIUNX


}

#ifdef _WIN32

void PixelWorldEngine::Application::OnProcessMessage(MSG message)
{
	switch (message.message)
	{
	case WM_MOUSEMOVE: {
		auto eventArg = &Events::MouseMoveEvent();

		eventArg->x = LOWORD(message.lParam);
		eventArg->y = HIWORD(message.lParam);

		OnMouseMove(this, eventArg);

		break;
	}

	case WM_LBUTTONDOWN: {
		auto eventArg = &Events::MouseClickEvent();

		eventArg->x = LOWORD(message.lParam);
		eventArg->y = HIWORD(message.lParam);

		eventArg->button = Events::MouseButton::Left;
		eventArg->isDown = true;

		OnMouseClick(this, eventArg);

		break;
	}

	case WM_LBUTTONUP: {
		auto eventArg = &Events::MouseClickEvent();

		eventArg->x = LOWORD(message.lParam);
		eventArg->y = HIWORD(message.lParam);

		eventArg->button = Events::MouseButton::Left;
		eventArg->isDown = false;

		OnMouseClick(this, eventArg);

		break;
	}

	case WM_MBUTTONDOWN: {
		auto eventArg = &Events::MouseClickEvent();

		eventArg->x = LOWORD(message.lParam);
		eventArg->y = HIWORD(message.lParam);
		eventArg->button = Events::MouseButton::Middle;
		eventArg->isDown = true;

		OnMouseClick(this, eventArg);

		break;
	}

	case WM_MBUTTONUP: {
		auto eventArg = &Events::MouseClickEvent();

		eventArg->x = LOWORD(message.lParam);
		eventArg->y = HIWORD(message.lParam);
		eventArg->button = Events::MouseButton::Middle;
		eventArg->isDown = false;

		OnMouseClick(this, eventArg);

		break;
	}

	case WM_RBUTTONDOWN: {
		auto eventArg = &Events::MouseClickEvent();

		eventArg->x = LOWORD(message.lParam);
		eventArg->y = HIWORD(message.lParam);
		eventArg->button = Events::MouseButton::Right;
		eventArg->isDown = true;

		OnMouseClick(this, eventArg);

		break;
	}

	case WM_RBUTTONUP: {
		auto eventArg = &Events::MouseClickEvent();

		eventArg->x = LOWORD(message.lParam);
		eventArg->y = HIWORD(message.lParam);
		eventArg->button = Events::MouseButton::Right;
		eventArg->isDown = false;

		OnMouseClick(this, eventArg);

		break;
	}

	case WM_KEYDOWN: {
		auto eventArg = &Events::KeyClickEvent();

		eventArg->isDown = true;
		eventArg->keyCode = (KeyCode)message.wParam;

		OnKeyClick(this, eventArg);

		break;
	}

	case WM_KEYUP: {
		auto eventArg = &Events::KeyClickEvent();

		eventArg->isDown = false;
		eventArg->keyCode = (KeyCode)message.wParam;

		OnKeyClick(this, eventArg);

		break;
	}

	case WM_MOUSEWHEEL: {
		auto eventArg = &Events::MouseWheelEvent();

		eventArg->x = GET_X_LPARAM(message.lParam);
		eventArg->y = GET_Y_LPARAM(message.lParam);
		eventArg->offset = HIWORD(message.wParam);

		OnMouseWheel(this, eventArg);

		break;
	}

	default:
		break;
	}
}

#endif // _WIN32


auto PixelWorldEngine::Application::ComputeViewPort(int windowWidth, int windowHeight, int resolutionWidth, int resolutionHeight) -> Rectangle
{
	if (windowWidth * resolutionHeight == windowHeight * resolutionWidth) return Rectangle(0, 0, (float)windowWidth, (float)windowHeight);

	float Tx = (float)resolutionWidth;
	float Ty = (float)resolutionHeight;

	float x = (float)windowWidth;
	float  y = (float)windowHeight;

	float scaleWidth = 0;
	float scaleHeight = 0;

	float offX = 0;
	float offY = 0;

	if (Ty * x > Tx * y) {
		//for width
		scaleWidth = windowWidth / ((Ty * x) / (Tx * y));
		scaleHeight = (float)windowHeight;

		offX = (windowWidth - scaleWidth) / 2.f;
	}
	else {
		//for height
		scaleWidth = (float)windowWidth;
		scaleHeight = windowHeight * ((Ty * x) / (Tx * y));

		offY = (windowHeight - scaleHeight) / 2.f;
	}

	return Rectangle(offX, offY, offX + scaleWidth, offY + scaleHeight);
}

auto PixelWorldEngine::Application::ComputeMousePosition(Rectangle ViewPort, int resolutionWidth, int resolutionHeight, int x, int y) -> std::pair<int, int>
{
	float width = ViewPort.right - ViewPort.left;
	float height = ViewPort.bottom - ViewPort.top;

	std::pair<int, int> result;

	result.first = (int)(resolutionWidth * (x - ViewPort.left) / width);
	result.second = (int)(resolutionHeight * (y - ViewPort.top) / height);

	return result;
}

PixelWorldEngine::Application::Application(const wchar_t * ApplicationName)
{
	applicationName = ApplicationName;

	graphics = new Graphics::Graphics();

	cameraBuffer = new Graphics::Buffer(graphics, nullptr, sizeof(glm::mat4x4));
	
	defaultShader = new Graphics::GraphicsShader(graphics,
		Utility::CharArrayToVector((char*)vsApplicationDefaultShaderCode),
		Utility::CharArrayToVector((char*)psApplicationDefaultShaderCode));

	defaultSampler = new Graphics::StaticSampler(graphics);

#ifdef _WIN32
	ImmDisableIME(0);
#endif // _WIN32

#ifdef LIUNX
	//禁用输入法
#endif // LIUNX
}

PixelWorldEngine::Application::~Application()
{
	Utility::Delete(cameraBuffer);

	Utility::Delete(defaultShader);
	Utility::Delete(defaultSampler);

	Utility::Delete(renderTarget);
	Utility::Delete(graphics);
}

void PixelWorldEngine::Application::MakeWindow(const wchar_t * WindowName, int Width, int Height, const wchar_t * IconName)
{
	windowName = WindowName;
	windowWidth = Width;
	windowHeight = Height;
	iconName = IconName;

	if (isWindowCreated == true) {

#ifdef _WIN32
		SetWindowTextW(hwnd, &windowName[0]);

		RECT rect;

		rect.top = 0;
		rect.left = 0;
		rect.right = windowWidth;
		rect.bottom = windowHeight;

		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		SetWindowPos(hwnd, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
			SWP_NOZORDER ^ SWP_NOMOVE);

#endif // _WIN32

#ifdef LIUNX
		//当窗口已经创建的话，修改他的标题以及宽度和高度
#endif // LIUNX
	}
	else {

#ifdef _WIN32

		auto hInstance = GetModuleHandle(0);

		WNDCLASS appInfo;

		appInfo.style = CS_DBLCLKS;
		appInfo.lpfnWndProc = DefaultWindowProc;
		appInfo.cbClsExtra = 0;
		appInfo.cbWndExtra = 0;
		appInfo.hInstance = hInstance;
		appInfo.hIcon = (HICON)LoadImage(0, &iconName[0], IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
		appInfo.hCursor = LoadCursor(nullptr, IDC_ARROW);
		appInfo.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		appInfo.lpszMenuName = NULL;
		appInfo.lpszClassName = &windowName[0];

		RegisterClass(&appInfo);

		RECT rect;

		rect.top = 0;
		rect.left = 0;
		rect.right = windowWidth;
		rect.bottom = windowHeight;

		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		hwnd = CreateWindowW(&windowName[0], &windowName[0], WS_OVERLAPPEDWINDOW ^
			WS_SIZEBOX ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
			rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr);

		swapDesc.BufferCount = 1;
		swapDesc.BufferDesc.Format = (DXGI_FORMAT)Graphics::PixelFormat::R8G8B8A8;
		swapDesc.BufferDesc.Height = windowHeight;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
		swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapDesc.BufferDesc.Width = windowWidth;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.Flags = 0;
		swapDesc.OutputWindow = hwnd;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
		swapDesc.Windowed = true;

		IDXGIDevice* device = nullptr;
		IDXGIAdapter* adapter = nullptr;
		IDXGIFactory* factory = nullptr;

		graphics->device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&device));
		device->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&adapter));
		adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));

		factory->CreateSwapChain(graphics->device, &swapDesc, &swapChain);

		Utility::Dispose(device);
		Utility::Dispose(adapter);
		Utility::Dispose(factory);

		self = this;
#endif // _WIN32

#ifdef LIUNX
		//创建了一个窗口，请禁用放大以及使用鼠标改变其大小。窗口默认不显示
		//创建了交换链，请注意不使用多重采样，且后台缓冲格式是R8G8B8A8。
#endif // LIUNX

		renderTarget = new Graphics::RenderTarget(graphics, this);

		isWindowCreated = true;
	}
}

void PixelWorldEngine::Application::MakeFullScreen(bool state)
{
#ifdef _WIN32

	swapChain->SetFullscreenState(state, nullptr);

#endif // _WIN32

}

void PixelWorldEngine::Application::ShowWindow()
{

#ifdef _WIN32

	::ShowWindow(hwnd, SW_SHOW);

#endif // _WIN32

#ifdef LIUNX
	//显示窗口
#endif // LIUNX

}

void PixelWorldEngine::Application::HideWindow()
{

#ifdef _WIN32

	::ShowWindow(hwnd, SW_HIDE);

#endif // _WIN32

#ifdef LIUNX
	//隐藏窗口
#endif // LIUNX


}

void PixelWorldEngine::Application::RunLoop()
{
	while (isWindowCreated == true) {

#ifdef _WIN32
		MSG message;
		message.hwnd = hwnd;

		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);

			OnProcessMessage(message);
		}

		if (message.message == WM_QUIT) isWindowCreated = false;
#endif // _WIN32

#ifdef LIUNX
		//处理消息循环
#endif // LIUNX


		OnUpdate(this);
	}
}

void PixelWorldEngine::Application::SetWorld(PixelWorld * PixelWorld)
{
	pixelWorld = PixelWorld;
}

auto PixelWorldEngine::Application::GetWindowWidth() -> int
{
	return windowWidth;
}

auto PixelWorldEngine::Application::GetWindowHeight() -> int
{
	return windowHeight;
}

auto PixelWorldEngine::Application::GetGraphics() -> Graphics::Graphics *
{
	return graphics;
}


