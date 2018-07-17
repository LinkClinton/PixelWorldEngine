#include "Application.hpp"

#include "Encoding.hpp"

#include "EngineDefaultResource.hpp"

#ifdef _WIN32

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

PixelWorldEngine::Application* self;

bool isCreate = false;

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

	case WM_SETFOCUS: {
		self->isFocused = true;
		break;
	}

	case WM_KILLFOCUS: {
		self->isFocused = false;
		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

#endif // _WIN32

void PixelWorldEngine::Application::OnMouseMove(void * sender, PixelWorldEngine::Events::MouseMoveEvent * eventArg)
{
	mousePositionX = eventArg->x;
	mousePositionY = eventArg->y;

	if (pixelWorld != nullptr) {
		auto mousePositionRelative = ComputeMousePosition(RectangleF(0, 0, (float)width, (float)height),
			pixelWorld->resolutionWidth, pixelWorld->resolutionHeight, mousePositionX, mousePositionY);

		mousePositionXRelative = mousePositionRelative.first;
		mousePositionYRelative = mousePositionRelative.second;
	}
	else mousePositionXRelative = mousePositionX,
		mousePositionYRelative = mousePositionY;

	eventArg->x = mousePositionXRelative;
	eventArg->y = mousePositionYRelative;

	Events::DoEventHandlers(MouseMove, this, eventArg);

	if (pixelWorld != nullptr)
		pixelWorld->OnMouseMove(pixelWorld, eventArg);
}

void PixelWorldEngine::Application::OnMouseWheel(void * sender, PixelWorldEngine::Events::MouseWheelEvent * eventArg)
{
	eventArg->x = mousePositionXRelative;
	eventArg->y = mousePositionYRelative;

	Events::DoEventHandlers(MouseWheel, this, eventArg);

	if (pixelWorld != nullptr)
		pixelWorld->OnMouseWheel(pixelWorld, eventArg);
}

void PixelWorldEngine::Application::OnMouseClick(void * sender, PixelWorldEngine::Events::MouseClickEvent * eventArg)
{
	eventArg->x = mousePositionXRelative;
	eventArg->y = mousePositionYRelative;

	Events::DoEventHandlers(MouseClick, this, eventArg);

	if (pixelWorld != nullptr)
		pixelWorld->OnMouseClick(pixelWorld, eventArg);
}

void PixelWorldEngine::Application::OnKeyClick(void * sender, PixelWorldEngine::Events::KeyClickEvent * eventArg)
{
	Events::DoEventHandlers(KeyClick, this, eventArg);

	if (pixelWorld != nullptr)
		pixelWorld->OnKeyClick(pixelWorld, eventArg);
}

void PixelWorldEngine::Application::OnSizeChange(void * sender, PixelWorldEngine::Events::SizeChangeEvent * eventArg)
{
	width = eventArg->width;
	height = eventArg->height;
	
	Utility::Delete(renderTarget);

#ifdef _WIN32
	swapChain->ResizeBuffers(1, width, height, (DXGI_FORMAT)Graphics::PixelFormat::Unknown, 0);
#endif // _WIN32

	renderTarget = new Graphics::RenderTarget(graphics, this);
}

void PixelWorldEngine::Application::OnUpdate(void * sender)
{
	deltaTime = timer.GetTime();

	timer.Start();

	fpsCounter.Update(deltaTime);
	
	if (pixelWorld != nullptr)
		pixelWorld->OnUpdate(deltaTime);

	for (auto it = animators.begin(); it != animators.end(); it++)
		it->second->OnUpdate(deltaTime);

	OnRender(sender);

	Events::DoEventHandlers(Update, this);
}

void PixelWorldEngine::Application::OnRender(void * sender)
{
	if (pixelWorld == nullptr) return;

	auto worldTexture = pixelWorld->GetCurrentWorld();
	
	auto resolutionWidth = worldTexture->GetWidth();
	auto resolutionHeight = worldTexture->GetHeight();

	auto matrix = Camera(RectangleF(0.f, 0.f, (float)resolutionWidth, (float)resolutionHeight)).GetMatrix();

	cameraBuffer->Update(&matrix);

	renderTarget->Clear(0, 0, 0);

	graphics->ClearState();

	graphics->SetRenderTarget(renderTarget);

	graphics->SetViewPort(Application::ComputeViewPort(width, height,
		resolutionWidth, resolutionHeight));

	graphics->SetShader(defaultShader);

	graphics->SetVertexBuffer(pixelWorld->renderCanvas->GetVertexBuffer());
	graphics->SetIndexBuffer(pixelWorld->renderCanvas->GetIndexBuffer());

	graphics->SetConstantBuffer(cameraBuffer, 0);
	graphics->SetShaderResource(worldTexture, 0);
	graphics->SetStaticSampler(defaultSampler, 0);

	graphics->DrawIndexed(pixelWorld->renderCanvas->GetIndexBuffer()->GetCount());

#ifdef _WIN32

	auto result = swapChain->Present(0, 0);

#endif // _WIN32

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

auto PixelWorldEngine::Application::ComputeMousePosition(int x, int y) -> std::pair<int, int>
{
	if (pixelWorld != nullptr) {
		auto mousePositionRelative = ComputeMousePosition(RectangleF(0, 0, (float)width, (float)height),
			pixelWorld->resolutionWidth, pixelWorld->resolutionHeight, mousePositionX, mousePositionY);
		
		return mousePositionRelative;
	}
	else return std::pair<int, int>(x, y);
}


auto PixelWorldEngine::Application::ComputeViewPort(int width, int height, int resolutionWidth, int resolutionHeight) -> RectangleF
{
	if (width * resolutionHeight == height * resolutionWidth) return RectangleF(0, 0, (float)width, (float)height);

	float Tx = (float)resolutionWidth;
	float Ty = (float)resolutionHeight;

	float x = (float)width;
	float  y = (float)height;

	float scaleWidth = 0;
	float scaleHeight = 0;

	float offX = 0;
	float offY = 0;

	if (Ty * x > Tx * y) {
		//for width
		scaleWidth = width / ((Ty * x) / (Tx * y));
		scaleHeight = (float)height;

		offX = (width - scaleWidth) / 2.f;
	}
	else {
		//for height
		scaleWidth = (float)width;
		scaleHeight = height * ((Ty * x) / (Tx * y));

		offY = (height - scaleHeight) / 2.f;
	}

	return RectangleF(offX, offY, offX + scaleWidth, offY + scaleHeight);
}

auto PixelWorldEngine::Application::ComputeMousePosition(RectangleF ViewPort, int resolutionWidth, int resolutionHeight, int x, int y) -> std::pair<int, int>
{
	float width = ViewPort.right - ViewPort.left;
	float height = ViewPort.bottom - ViewPort.top;

	std::pair<int, int> result;

	result.first = (int)(resolutionWidth * (x - ViewPort.left) / width);
	result.second = (int)(resolutionHeight * (y - ViewPort.top) / height);

	return result;
}

PixelWorldEngine::Application::Application(std::string ApplicationName)
{
	DebugThrow(DebugLayer::Assert(isCreate, Error::MoreThanOneInstance, ApplicationName, "Application"));

	applicationName = ApplicationName;

	graphics = new Graphics::Graphics();

	cameraBuffer = new Graphics::Buffer(graphics, nullptr, sizeof(glm::mat4x4));
	
	defaultShader = new Graphics::GraphicsShader(graphics,
		Utility::CharArrayToVector((char*)vsApplicationDefaultShaderCode),
		Utility::CharArrayToVector((char*)psApplicationDefaultShaderCode));

	defaultSampler = new Graphics::StaticSampler(graphics);

	isCreate = true;

#ifdef _WIN32
	ImmDisableIME(0);
#endif // _WIN32

}

PixelWorldEngine::Application::~Application()
{
	Utility::Delete(cameraBuffer);

	Utility::Delete(defaultShader);
	Utility::Delete(defaultSampler);

	Utility::Delete(renderTarget);
	Utility::Delete(graphics);
}

void PixelWorldEngine::Application::MakeWindow(std::string WindowName, int Width, int Height, std::string IconName)
{
	if (isWindowCreated == true) return;

	windowName = WindowName;
	width = Width;
	height = Height;
	iconName = IconName;

	DebugReturn(DebugLayer::Assert(width <= 0 || height <= 0, Error::WidthOrHeightLessThanZero, WindowName, FunctionName));

#ifdef _WIN32

	wideWindowName = Encoding::ToWideChar(windowName);
	wideIconName = Encoding::ToWideChar(iconName);

	auto hInstance = GetModuleHandle(0);

	WNDCLASS appInfo;

	appInfo.style = CS_DBLCLKS;
	appInfo.lpfnWndProc = DefaultWindowProc;
	appInfo.cbClsExtra = 0;
	appInfo.cbWndExtra = 0;
	appInfo.hInstance = hInstance;
	appInfo.hIcon = (HICON)LoadImage(0, &wideIconName[0], IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	appInfo.hCursor = LoadCursor(nullptr, IDC_ARROW);
	appInfo.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	appInfo.lpszMenuName = NULL;
	appInfo.lpszClassName = &wideWindowName[0];

	RegisterClass(&appInfo);

	RECT rect;

	rect.top = 0;
	rect.left = 0;
	rect.right = width;
	rect.bottom = height;

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	hwnd = CreateWindow(&wideWindowName[0], &wideWindowName[0], WS_OVERLAPPEDWINDOW ^
		WS_SIZEBOX ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr);

	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Format = (DXGI_FORMAT)Graphics::PixelFormat::R8G8B8A8;
	swapDesc.BufferDesc.Height = height;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Width = width;
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

	renderTarget = new Graphics::RenderTarget(graphics, this);

	isWindowCreated = true;

}

void PixelWorldEngine::Application::SetWindow(std::string WindowName, int Width, int Height)
{
	windowName = WindowName;

	DebugReturn(DebugLayer::Assert(Width <= 0 || Height <= 0, Error::WidthOrHeightLessThanZero, WindowName, FunctionName));

#ifdef _WIN32
	wideWindowName = Encoding::ToWideChar(windowName);

	SetWindowText(hwnd, &wideWindowName[0]);
#endif

	if (width != Width || height != Height) {

		width = Width;
		height = Height;

#ifdef _WIN32

		RECT rect;

		rect.top = 0;
		rect.left = 0;
		rect.right = width;
		rect.bottom = height;

		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		SetWindowPos(hwnd, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
			SWP_NOZORDER ^ SWP_NOMOVE);

#endif // _WIN32
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

}

void PixelWorldEngine::Application::HideWindow()
{

#ifdef _WIN32

	::ShowWindow(hwnd, SW_HIDE);

#endif // _WIN32



}

void PixelWorldEngine::Application::RunLoop()
{
	while (isWindowCreated == true) {

#ifdef _WIN32
		MSG message;
		message.hwnd = hwnd;

		while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);

			OnProcessMessage(message);
		}

		if (message.message == WM_QUIT) isWindowCreated = false;
#endif // _WIN32

		OnUpdate(this);
	}
}

void PixelWorldEngine::Application::SetWorld(PixelWorld * PixelWorld)
{
	pixelWorld = PixelWorld;
}

void PixelWorldEngine::Application::RegisterAnimator(Animator * animator)
{
	DebugReturn(DebugLayer::Assert(animator == nullptr, Error::TheObjectIsNull, applicationName, FunctionName));
		
	animators[animator->name] = animator;
}

void PixelWorldEngine::Application::UnRegisterAnimator(Animator * animator)
{
	DebugReturn(DebugLayer::Assert(animator == nullptr, Error::TheObjectIsNull, applicationName, FunctionName));
	
	animators.erase(animator->name);
}

void PixelWorldEngine::Application::UnRegisterAnimator(std::string name)
{
	DebugReturn(DebugLayer::Assert(animators.count(name) == 0, Error::TheNameIsNotExist, name, FunctionName));
	
	animators.erase(name);
}

auto PixelWorldEngine::Application::GetWindowWidth() -> int
{
	return width;
}

auto PixelWorldEngine::Application::GetWindowHeight() -> int
{
	return height;
}

auto PixelWorldEngine::Application::GetFramePerSecond() -> int
{
	return fpsCounter.GetFramePerSecond();
}

auto PixelWorldEngine::Application::GetDeltaTime() -> float
{
	return deltaTime;
}

auto PixelWorldEngine::Application::GetGraphics() -> Graphics::Graphics *
{
	return graphics;
}

auto PixelWorldEngine::Application::IsFocused() -> bool
{
	return isFocused;
}


