#pragma once

#include "pch.hpp"

#include "Graphics.hpp"
#include "ShaderResource.hpp"
#include "WorldMap.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief ����������ͣ�������ϲ���һ�����������ĸ�ʽ������R8G8B8A8
	 */
	class TextureManager {
	private:
		Graphics::Graphics* graphics; //..

		Graphics::Texture2D* finalTexture; //�����洢��������

		int widthRequirement; //���Ҫ��
		int heightRequirement; //�߶�Ҫ��

		std::map<int, Graphics::Texture2D*> textures; //�洢��Ҫ�ϲ��������Լ���ID

		int maxRenderObjectID; //�ϲ���������ʹ�õĵ�����ID

		friend class PixelWorld;
	public:
		/**
		 * @brief ���캯��
		 * @param[in] application Ӧ�ó���
		 * @param[in] widthRequirement �ϳɵ�������Ҫ��
		 * @param[in] heightRequirement �ϳɵ�����߶�Ҫ��
		 */
		TextureManager(Application* application, int widthRequirement = DEFAULT_BLOCK_SIZE, int heightRequirement = DEFAULT_BLOCK_SIZE);

		/**
		 * @brief �����Ҫ�ϲ����������Ը��ǣ�ע������Ĵ�СҪ�������Լ���ʽ������R8G8B8A8
		 * @param[in] id ����Ҫʹ�õı�ţ�����ȾID������Ϊ������
		 * @param[in] texture ����
		 */
		void AddTexture(int id, Graphics::Texture2D* texture);

		/*
		 * @brief �Ƴ����ID������
		 * @param[in] id �����ID������еĻ���ô���Ƴ���û�еĻ�������
		 */
		void RemoveTexture(int id);

		/**
		 * @brief �ϲ�������������������Ҫ������������������ϲ��������
		 */
		void MergeTextures();

		/**
		 * @brief ��ȡ�ϲ��������
		 * @return �ϲ��������
		 */
		auto GetFinalTexture() -> Graphics::Texture2D*;

		/**
		 * @brief ��ȡ���Ҫ��
		 * @return ���Ҫ��
		 */
		auto GetWidthRequirement() -> int;

		/**
		 * @brief ��ȡ�߶�Ҫ��
		 * @return �߶�Ҫ��
		 */
		auto GetHeightRequirement() -> int;

		/**
		 * @brief ��ȡ���õ�������ȾID
		 * @param[in] ������ȾID
		 */
		auto GetMaxRenderObjectID() -> int;
	};

}