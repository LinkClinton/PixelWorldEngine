#pragma once

#include "pch.hpp"

#include "Graphics.hpp"
#include "ShaderResource.hpp"

namespace PixelWorldEngine {

	/**
	 * @brief ����������ʹ��
	 */
	struct SubTexture {
		glm::vec2 startPosition; //��������Ŀ�ʼλ��
		glm::vec2 endPosition; //��������Ľ���λ��
		
		int startPositionX; //����ʼ��Xλ��
		int startPositionY; //����ʼ��Yλ��
		int width; //����Ŀ��
		int height; //����ĸ߶�

		glm::mat4x4 transform; //��������任����

		/**
		 * @brief ������������
		 * @param[in] maxWidth ���Ŀ��
		 * @param[in] maxHeight ���ĸ߶�
		 */
		void MakeTexCoordPosition(int maxWidth, int maxHeight);

		/**
		 * @brief �Ƿ��ཻ
		 * @param[in] texture1 ��һ��������
		 * @param[in] texture2 �ڶ���������
		 */
		static bool IsEnter(SubTexture texture1, SubTexture texture2);
	};

	/**
	 * @brief �ϲ��������ͣ����ڽ�����ϲ���ע���������ͱ�����R8G8B8A8
	 */
	class MergeTexture {
	private:
		Graphics::Graphics* graphics; //..
		
		Graphics::Texture2D* finalTexture; //���ϲ�������

		std::map<int, SubTexture> subTextures; //������
	public:
		/**
		 * @brief ���캯��
		 * @param[in] width ���
		 * @param[in] height �߶�
		 */
		MergeTexture(Application* application, int width, int height);

		/**
		 * @brief ��������
		 */
		~MergeTexture();

		/**
		 * @brief ��������������Ƶ�MergeTexture��ȥ��ע����θ��Ƶ���λ�ú����������н�������ô�Ͳ��Ḵ��
		 * @param[in] id �������ȾID
		 * @param[in] positionX ������MergeTexture����ʼX����
		 * @param[in] positionY ������MergeTexture����ʼY����
		 * @param[in] texture Ҫ���Ƶ�����
		 */
		void AddTexture(int id, int positionX, int positionY, Graphics::Texture2D* texture);

		/**
		 * @brief �Ƴ�����ע�Ⲣ���Ὣ��������������������㣬�����ǲ������򽫻��ɿ�������
		 * @param[in] id �����ID
		 */
		void RemoveTexture(int id);

		/**
		 * @brief ��ȡ��������ı任���󣬼�����Ӧԭ���������������任��MergeTexture����������ϵ��ȥ
		 * @param[in] id �������ȾID
		 * @return �任����
		 */
		auto GetTexCoordTransform(int id) -> glm::mat4x4;

		/**
		 * @brief ��ȡMergeTexture������
		 * @return ��������
		 */
		auto GetFinalTexture() -> Graphics::Texture2D*;

		/**
		 * @brief ������ID�Ƿ��Ѿ���ʹ����
		 * @return �Ƿ�ʹ��
		 */
		auto IsExistID(int id) -> bool;
	};

}