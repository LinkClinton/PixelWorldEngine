#pragma once

#include "pch.hpp"

#include "Utility.hpp"
#include "GraphicsEnum.hpp"
#include "BaseStruct.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		class Graphics;

		/**
		 * @brief ��ɫ����Դ
		 */
		class ShaderResource {
#ifdef _WIN32
		public:
			ID3D11Resource * resource; //Direct3D �ӿ�
			ID3D11ShaderResourceView* resourceView; //Direct3D �ӿ�
#endif // _WIN32
		public:
			/**
			 * @brief ��������
			 */
			~ShaderResource();

		};
	}
}

#include "Texture2D.hpp"
#include "BufferArray.hpp"
#include "Texture2DArray.hpp"
