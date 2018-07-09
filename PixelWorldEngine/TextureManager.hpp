#pragma once

#include "pch.hpp"

#include "Graphics.hpp"
#include "ShaderResource.hpp"
#include "WorldMap.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief 纹理管理类型，将纹理合并成一块大纹理，纹理的格式必须是R8G8B8A8
	 */
	class TextureManager {
	private:
		Graphics::Graphics* graphics; //..

		Graphics::Texture2D* finalTexture; //用来存储最后的纹理

		int widthRequirement; //宽度要求
		int heightRequirement; //高度要求

		std::map<int, Graphics::Texture2D*> textures; //存储需要合并的纹理，以及其ID

		int maxRenderObjectID; //合并的纹理中使用的的最大的ID

		friend class PixelWorld;
	public:
		/**
		 * @brief 构造函数
		 * @param[in] application 应用程序
		 * @param[in] widthRequirement 合成的纹理宽度要求
		 * @param[in] heightRequirement 合成的纹理高度要求
		 */
		TextureManager(Application* application, int widthRequirement = DEFAULT_BLOCK_SIZE, int heightRequirement = DEFAULT_BLOCK_SIZE);

		/**
		 * @brief 添加需要合并的纹理，可以覆盖，注意纹理的大小要求满足以及格式必须是R8G8B8A8
		 * @param[in] id 纹理要使用的编号，即渲染ID，必须为正整数
		 * @param[in] texture 纹理
		 */
		void AddTexture(int id, Graphics::Texture2D* texture);

		/*
		 * @brief 移除这个ID的纹理
		 * @param[in] id 纹理的ID，如果有的话那么就移除，没有的话就算了
		 */
		void RemoveTexture(int id);

		/**
		 * @brief 合并纹理，即添加完纹理后，需要调用这个函数来创建合并后的纹理
		 */
		void MergeTextures();

		/**
		 * @brief 获取合并后的纹理
		 * @return 合并后的纹理
		 */
		auto GetFinalTexture() -> Graphics::Texture2D*;

		/**
		 * @brief 获取宽度要求
		 * @return 宽度要求
		 */
		auto GetWidthRequirement() -> int;

		/**
		 * @brief 获取高度要求
		 * @return 高度要求
		 */
		auto GetHeightRequirement() -> int;

		/**
		 * @brief 获取设置的最大的渲染ID
		 * @param[in] 最大的渲染ID
		 */
		auto GetMaxRenderObjectID() -> int;
	};

}