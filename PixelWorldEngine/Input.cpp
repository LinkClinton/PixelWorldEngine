#include "Input.hpp"
#include "Application.hpp"

#ifdef _WIN32

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0) 
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#endif // _WIN32

extern PixelWorldEngine::Application* self;

bool PixelWorldEngine::Input::GetMouseButtonDown(Events::MouseButton mouseButton)
{
	if (self->isFocused == false) return false;

#ifdef _WIN32
	switch (mouseButton)
	{
	case PixelWorldEngine::Events::Left:
		return KEYDOWN((int)KeyCode::LButton);
	case PixelWorldEngine::Events::Middle:
		return KEYDOWN((int)KeyCode::MButton);
	case PixelWorldEngine::Events::Right:
		return KEYDOWN((int)KeyCode::RButton);
	}
#endif // _WIN32

	return false;
}

bool PixelWorldEngine::Input::GetMouseButtonUp(Events::MouseButton mouseButton)
{
	if (self->isFocused == false) return true;

#ifdef _WIN32
	switch (mouseButton)
	{
	case PixelWorldEngine::Events::Left:
		return KEYUP((int)KeyCode::LButton);
	case PixelWorldEngine::Events::Middle:
		return KEYUP((int)KeyCode::MButton);
	case PixelWorldEngine::Events::Right:
		return KEYUP((int)KeyCode::RButton);
	}
#endif // _WIN32


	return false;
}

bool PixelWorldEngine::Input::GetKeyCodeDown(KeyCode keyCode)
{
	if (self->isFocused == false) return false;

#ifdef _WIN32
	return KEYDOWN((int)keyCode);
#endif // _WIN32


	return false;
}

bool PixelWorldEngine::Input::GetKeyCodeUp(KeyCode keyCode)
{
	if (self->isFocused == false) return true;

#ifdef _WIN32
	return KEYUP((int)keyCode);
#endif // _WIN32

	return false;
}

bool PixelWorldEngine::Input::IsCapsLock()
{
#ifdef _WIN32
	return GetKeyState((int)KeyCode::CapsLock) & 1;
#endif // _WIN32

	return false;
}

bool PixelWorldEngine::Input::IsNumLock()
{
#ifdef _WIN32
	return GetKeyState((int)KeyCode::NumLock) & 1;
#endif // _WIN32

	return false;
}

bool PixelWorldEngine::Input::ScrollLock()
{
#ifdef _WIN32
	return GetKeyState((int)KeyCode::Scroll) & 1;
#endif // _WIN32

	return false;
}

int PixelWorldEngine::Input::GetMousePositionX()
{
#ifdef _WIN32
	POINT point;

	GetCursorPos(&point);
	ScreenToClient(self->hwnd, &point);
	
	return self->ComputeMousePosition(point.x, point.y).first;
#endif // _WIN32

	return 0;
}

int PixelWorldEngine::Input::GetMousePositionY()
{
#ifdef _WIN32
	POINT point;

	GetCursorPos(&point);
	ScreenToClient(self->hwnd, &point);

	return self->ComputeMousePosition(point.x, point.y).second;
#endif // _WIN32

	return 0;
}

auto PixelWorldEngine::Input::GetMousePosition() -> std::pair<int, int>
{
#ifdef _WIN32
	POINT point;

	GetCursorPos(&point);
	ScreenToClient(self->hwnd, &point);

	return self->ComputeMousePosition(point.x, point.y);
#endif // _WIN32

	return std::pair<int, int>();
}