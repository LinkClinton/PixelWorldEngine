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
		 * @return ����ཻ����true�����򷵻�false
		 */
		static bool IsEnter(SubTexture texture1, SubTexture texture2);
	};

	/**
	 * @brief �ϲ��������ͣ����ڽ�����ϲ���ע����������Ҫ��һ��
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
		 * @param[in] pixelFormat �ϲ���������ͣ�
		 */
		MergeTexture(Application* application, int width, int height, Graphics::PixelFormat pixelFormat = Graphics::PixelFormat::R8G8B8A8);

		/**
		 * @brief ��������
		 */
		~MergeTexture();

		/**
		 * @brief ��������������Ƶ��ϲ�������ȥ��ע����θ��Ƶ���λ�ú����������н��������߳�����Χ����ô�Ͳ��Ḵ��
		 * @param[in] id �������Ⱦ���
		 * @param[in] positionX �����ںϲ��������ʼX����
		 * @param[in] positionY �����ںϲ��������ʼY����
		 * @param[in] texture Ҫ���Ƶ�����
		 */
		void AddTexture(int id, int positionX, int positionY, Graphics::Texture2D* texture);

		/**
		 * @brief �Ƴ�����ע�Ⲣ���Ὣ��������������������㣬�����ǲ������򽫻��ɿ�������
		 * @param[in] id �������Ⱦ���
		 */
		void RemoveTexture(int id);

		/**
		 * @brief ��ȡ��������ı任���󣬼�����Ӧԭ���������������任���ϲ��������������ϵ��ȥ
		 * @param[in] id �������Ⱦ���
		 * @return �任����
		 */
		auto GetTexCoordTransform(int id) -> glm::mat4x4;

		/**
		 * @brief ��ȡ�ϲ����������
		 * @return ��������
		 */
		auto GetFinalTexture() -> Graphics::Texture2D*;

		/**
		 * @brief ��ȡ���ظ�ʽ
		 * @return ���ظ�ʽ
		 */
		auto GetPixelFormat() -> Graphics::PixelFormat;

		/**
		 * @brief ��������Ⱦ����Ƿ��Ѿ���ʹ����
		 * @return �Ƿ�ʹ��
		 */
		auto IsExistID(int id) -> bool;
	};

}