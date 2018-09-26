#pragma once

#include "pch.hpp"

#include "Utility.hpp"
#include "GraphicsEnum.hpp"
#include "BaseStruct.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		class Graphics;

		/**
		 * @brief 着色器资源
		 */
		class ShaderResource {
#ifdef _WIN32
		public:
			ID3D11Resource * resource; //Direct3D 接口
			ID3D11ShaderResourceView* resourceView; //Direct3D 接口
#endif // _WIN32
		public:
			/**
			 * @brief 析构函数
			 */
			~ShaderResource();

		};
	}
}

#include "Texture2D.hpp"
#include "BufferArray.hpp"
#include "Texture2DArray.hpp"
