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

		MergeTexture* mergeTextures[MAX_MERGETEXTURE_COUNT]; //MergeTexture页

		friend class PixelWorld;
	public:
		/**
		 * @brief 构造函数
		 * @param[in] application 应用程序
		 */
		TextureManager(Application* application);
	
		/**
		 * @brief 添加MergeTexture
		 * @param[in] id 标记ID，注意重复的话将会覆盖原本的，并且范围在[0, MAX_MERGETEXTURE_COUNT - 1]之间
		 * @param[in] mergeTexture MergexTexture，如果有重合的渲染ID的话，那么优先id在前的
		 */
		void AddMergeTexture(int id, MergeTexture* mergeTexture);

		/**
		 * @移除MergeTexture
		 * @param[in] id 标记ID
		 */
		void RemoveMergeTexture(int id);

		/**
		 * @brief 获取这个渲染ID在属于哪个MergeTexture，如果有重复的话，优先id在前的，如果没有的话，返回MAX_MERGETEXTURE_COUNT
		 * @return 渲染ID所在的MergeTexture的编号
		 */
		auto GetWhich(int renderObjectID) -> int;
	};

}