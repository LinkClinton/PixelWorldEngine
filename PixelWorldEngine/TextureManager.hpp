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

	/**
	 * @brief ��������
	 */
	enum TextureType {
		Size128 = 128, //��Ⱥ͸߶ȴ�СΪ128
		Size256 = 256, //��Ⱥ͸߶ȴ�СΪ256
		Size512 = 512, //��Ⱥ͸߶ȴ�СΪ512
		Size1024 = 1024, //��Ⱥ͸߶ȴ�СΪ1024
		Size2048 = 2048, //��Ⱥ͸߶ȴ�СΪ2048
		Count = 5
	};

	/**
	 * @brief ������
	 */
	struct SubTexture {
		int arrayIndex; //�������ڵ�����

		int startPositionX; //�������ʼXλ��
		int startPositionY; //�������ʼYλ��
		int width; //����Ŀ��
		int height; //����ĸ߶�

		glm::mat4x4 transform; //��������ı任����

		void MakeTexCoordPosition(int maxWidth, int maxHeight);
	};

	/**
	 * @brief ���������
	 */
	class TextureManager {
	private:
		Graphics::Graphics* graphics; //..

		Graphics::Texture2DArray* textureArray; //�洢�������������

		Graphics::PixelFormat pixelFormat; //�����ʽ

		std::map<int, SubTexture> subTextures; //������

		std::queue<SubTexture> freeSubTextures[(int)TextureType::Count]; //���е�������

		int textureCount; //����ʹ����
		int textureSpace; //�����ܹ���

		static int textureSizes[(int)TextureType::Count]; //����֧�ֵĴ�С
		static int textureSizeIndex[MAX_TEXTURE_SIZE + 1]; //��ͬ��С�������Ӧ����������
		static bool isInitialized; //�Ƿ��ʼ��

		/**
		 * @brief ���������Ƶ�����������ȥ
		 * @param[in] id ���������Ⱦ���
		 * @param[in] subTexture ������
		 * @param[in] texture ����������
		 */
		void CopySubTexture(int id, SubTexture subTexture, Graphics::Texture2D* texture);
	public:
		/**
		 * @brief ���캯��
		 * @param[in] application Ӧ�ó���
		 * @param[in] pixelFormat �����ʽ
		 */
		TextureManager(Application* application, Graphics::PixelFormat pixelFormat = Graphics::PixelFormat::Unknown);

		/** 
		 * @brief ��������
		 */
		~TextureManager();

		/**
		 * @brief ע������
		 * @param[in] id ��Ⱦ���
		 * @param[in] texture ����
		 */
		void RegisterTexture(int id, Graphics::Texture2D* texture);

		/**
		 * @brief ȡ��ע������
		 * @param[in] id ��Ⱦ���
		 */
		void UnRegisterTexture(int id);

		/**
		 * @brief ���
		 */
		void Clear();

		/**
		 * @brief ͨ����Ⱦ��Ż�ȡ����任
		 * @param[in] id ��Ⱦ���
		 * @return ����任
		 */
		auto GetTexCoordTransform(int id) -> glm::mat4x4;

		/**
		 * @brief ��ȡ��������
		 * @return ��������
		 */
		auto GetTextureArray() -> Graphics::Texture2DArray*;

		/**
		 * @brief ��ȡ��Ӧ��Ⱦ������ڵ�������������
		 * @return ��������
		 */
		auto GetArrayIndex(int id) -> int;

		/**
		 * @brief ��������Ⱦ����Ƿ����
		 * @return ����ֵΪtrue������ڣ����򲻴���
		 */
		auto IsExist(int id) -> bool;
	};
	
}