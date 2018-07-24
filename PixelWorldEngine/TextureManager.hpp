#pragma once

#include "pch.hpp"

#include "Graphics.hpp"
#include "ShaderResource.hpp"
#include "WorldMap.hpp"

#define TEXTURE_WIDTH 2048
#define TEXTURE_HEIGHT 2048
#define TEXTURE_ARRAY_ADD 8
#define MAX_TEXTURE_SIZE 2048

namespace PixelWorldEngine {

	enum TextureType {
		Size128 = 128,
		Size256 = 256,
		Size512 = 512,
		Size1024 = 1024,
		Size2048 = 2048,
		Count = 5
	};

	struct SubTexture {
		int arrayIndex;

		int startPositionX;
		int startPositionY;
		int width;
		int height;

		glm::mat4x4 transform;

		void MakeTexCoordPosition(int maxWidth, int maxHeight);
	};

	class TextureManager {
	private:
		Graphics::Graphics* graphics;

		Graphics::Texture2DArray* textureArray;

		Graphics::PixelFormat pixelFormat;

		std::map<int, SubTexture> subTextures;

		std::queue<SubTexture> freeSubTextures[(int)TextureType::Count];

		int textureCount;
		int textureSpace;

		static int textureSizes[(int)TextureType::Count];
		static int textureSizeIndex[MAX_TEXTURE_SIZE + 1];
		static bool isInitialized;

		void CopySubTexture(int id, SubTexture subTexture, Graphics::Texture2D* texture);
	public:
		TextureManager(Application* application, Graphics::PixelFormat pixelFormat = Graphics::PixelFormat::Unknown);

		~TextureManager();

		void RegisterTexture(int id, Graphics::Texture2D* texture);

		void UnRegisterTexture(int id);

		void Clear();

		auto GetTexCoordTransform(int id) -> glm::mat4x4;

		auto GetTextureArray() -> Graphics::Texture2DArray*;

		auto GetArrayIndex(int id) -> int;

		auto IsExist(int id) -> bool;
	};
	
}