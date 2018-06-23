#pragma once

#include "pch.hpp"

#include "Utility.hpp"
#include "GraphicsEnum.hpp"
#include "BaseStruct.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		class Graphics;

		class ShaderResource {
#ifdef _WIN32
		public:
			ID3D11Resource* resource;
			ID3D11ShaderResourceView* resourceView;
#endif // _WIN32
		public:
			~ShaderResource();

		};

		class Texture2D :public ShaderResource {
		protected:
			Graphics* graphics;

			int width, height;

			int size;
			int rowPitch;
			int mipLevels;

			PixelFormat pixelFormat;

#ifdef _WIN32
		public:
			D3D11_TEXTURE2D_DESC desc;
#endif // _WIN32

		public:
			Texture2D(Graphics* graphics, void* Data, int Width, int Height, PixelFormat PixelFormat = PixelFormat::Unknown, int MipLevels = 1);

			Texture2D(Texture2D* srcTexture, PixelWorldEngine::Rectangle srcRect);

			~Texture2D();

			void Update(void* data);

			auto GetWidth() -> int;

			auto GetHeight() -> int;

			auto GetSize() -> int;

			auto GetMipLevels() -> int;

			auto GetPixelFormat()->PixelFormat;
		};
	}
}
