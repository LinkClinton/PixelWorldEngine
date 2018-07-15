#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		/**
		 * @brief 像素格式
		 */
		enum class PixelFormat {
			Unknown = 0, //未知格式
			R32G32B32A32_Float = 2, //RGBA每个都占一个float
			R8G8B8A8 = 28, //RGBA每个都占一个byte
			A8 = 65 //仅仅只有A值占一个byte
		};
	}
}
