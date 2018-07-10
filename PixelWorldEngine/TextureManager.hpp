#pragma once

#include "pch.hpp"

#include "Graphics.hpp"
#include "ShaderResource.hpp"
#include "WorldMap.hpp"
#include "MergeTexture.hpp"

#define MAX_MERGETEXTURE_COUNT 4

namespace PixelWorldEngine {

	/**
	 * @brief 纹理管理类型，将纹理合并成一块大纹理，纹理的格式必须是R8G8B8A8
	 */
	class TextureManager {
	private:
		Graphics::Graphics* graphics; //..

		MergeTexture* mergeTextures[MAX_MERGETEXTURE_COUNT];

		friend class PixelWorld;
	public:
		/**
		 * @brief 构造函数
		 * @param[in] application 应用程序
		 */
		TextureManager(Application* application);
	
		void AddMergeTexture(int id, MergeTexture* mergeTexture);

		void RemoveMergeTexture(int id);

		auto GetWhich(int renderObjectID) -> int;
	};

}