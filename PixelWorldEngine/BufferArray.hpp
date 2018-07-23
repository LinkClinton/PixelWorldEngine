#pragma once

#include "pch.hpp"

#include "Utility.hpp"
#include "GraphicsEnum.hpp"
#include "BaseStruct.hpp"
#include "ShaderResource.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		/**
		* @brief �������飬����������������ɫ����Դ�ķ�ʽ�洢
		*/
		class BufferArray :public ShaderResource {
		protected:
			Graphics * graphics; //..

			int size; //��С����λbyte
			int count; //����ĳ���

#ifdef _WIN32
		public:
			D3D11_BUFFER_DESC desc; //��������ṹ

			ID3D11Buffer* buffer; //Direct3D �ӿ�
#endif // _WIN32

		public:
			/**
			* @brief ���캯��
			* @param[in] graphics ͼ�νӿ�
			* @param[in] data ������Ϣ
			* @param[in] dataSize ���ݴ�С
			* @param[in] dataCount ������������飬��ô�䳤�ȣ�����������Ϊ1
			*/
			BufferArray(Graphics* graphics, void* data, int dataSize, int dataCount = 1);

			/**
			* @brief ��������
			*/
			~BufferArray();

			/**
			* @brief ��������
			* @param[in] ���ݣ�ע�����ݴ�Сһ��
			*/
			void Update(void* data);

			/**
			* @brief ��ȡ�����С
			* @return �����С����λbyte
			*/
			auto GetSize() -> int;

			/**
			* @brief ��ȡ����
			* @return ����
			*/
			auto GetCount() -> int;
		};
	}

}