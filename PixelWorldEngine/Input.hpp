#pragma once

#include "pch.hpp"

#include "Events.hpp"

namespace PixelWorldEngine {

	class Input {
	public:
		Input() = delete;

		static bool GetMouseButtonDown(Events::MouseButton mouseButton);

		static bool GetMouseButtonUp(Events::MouseButton mouseButton);

		static bool GetKeyCodeDown(KeyCode keyCode);

		static bool GetKeyCodeUp(KeyCode keyCode);

		static bool IsCapsLock();

		static bool IsNumLock();

		static bool ScrollLock();

		static int GetMousePositionX();

		static int GetMousePositionY();

		static auto GetMousePosition() -> std::pair<int, int>;
	};

}