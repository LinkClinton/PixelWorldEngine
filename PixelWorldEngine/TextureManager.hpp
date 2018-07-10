#pragma once

#include "pch.hpp"

#include "Graphics.hpp"
#include "ShaderResource.hpp"
#include "WorldMap.hpp"
#include "MergeTexture.hpp"

#define MAX_MERGETEXTURE_COUNT 4

namespace PixelWorldEngine {

	/**
	 * @brief ����������ͣ�������ϲ���һ�����������ĸ�ʽ������R8G8B8A8
	 */
	class TextureManager {
	private:
		Graphics::Graphics* graphics; //..

		MergeTexture* mergeTextures[MAX_MERGETEXTURE_COUNT];

		friend class PixelWorld;
	public:
		/**
		 * @brief ���캯��
		 * @param[in] application Ӧ�ó���
		 */
		TextureManager(Application* application);
	
		void AddMergeTexture(int id, MergeTexture* mergeTexture);

		void RemoveMergeTexture(int id);

		auto GetWhich(int renderObjectID) -> int;
	};

}