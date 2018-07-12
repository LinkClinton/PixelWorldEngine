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

		MergeTexture* mergeTextures[MAX_MERGETEXTURE_COUNT]; //MergeTextureҳ

		friend class PixelWorld;
	public:
		/**
		 * @brief ���캯��
		 * @param[in] application Ӧ�ó���
		 */
		TextureManager(Application* application);
	
		/**
		 * @brief ���MergeTexture
		 * @param[in] id ���ID��ע���ظ��Ļ����Ḳ��ԭ���ģ����ҷ�Χ��[0, MAX_MERGETEXTURE_COUNT - 1]֮��
		 * @param[in] mergeTexture MergexTexture��������غϵ���ȾID�Ļ�����ô����id��ǰ��
		 */
		void AddMergeTexture(int id, MergeTexture* mergeTexture);

		/**
		 * @�Ƴ�MergeTexture
		 * @param[in] id ���ID
		 */
		void RemoveMergeTexture(int id);

		/**
		 * @brief ��ȡ�����ȾID�������ĸ�MergeTexture��������ظ��Ļ�������id��ǰ�ģ����û�еĻ�������MAX_MERGETEXTURE_COUNT
		 * @return ��ȾID���ڵ�MergeTexture�ı��
		 */
		auto GetWhich(int renderObjectID) -> int;
	};

}