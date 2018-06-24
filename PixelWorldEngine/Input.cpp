#include "Input.hpp"

#ifdef _WIN32

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0) 
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#endif // _WIN32


bool PixelWorldEngine::Input::GetMouseButtonDown(Events::MouseButton mouseButton)
{

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
#ifdef _WIN32
	return KEYDOWN((int)keyCode);
#endif // _WIN32


	return false;
}

bool PixelWorldEngine::Input::GetKeyCodeUp(KeyCode keyCode)
{
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