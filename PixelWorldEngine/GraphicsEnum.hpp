#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		/**
		 * @brief ���ظ�ʽ
		 */
		enum class PixelFormat {
			Unknown = 0, //δ֪��ʽ
			R32G32B32A32_Float = 2, //RGBAÿ����ռһ��float
			R8G8B8A8 = 28, //RGBAÿ����ռһ��byte
			A8 = 65 //����ֻ��Aֵռһ��byte
		};
	}
}
