#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		class Graphics;

		/**
		 * @brief ���������
		 */
		enum class BufferType {
			VertexBuffer = 1, //���㻺�壬���ڴ洢��������
			IndexBuffer = 2, //�������壬���ڴ洢������������
			ConstantBuffer = 4 //�������壬���ڴ洢�ύ����ɫ��������
		};

		/**
		 * @brief ��������
		 */
		class Buffer {
		private:
			Graphics* graphics; //..

			int size; //����Ĵ�С����λbyte
			int count; //Ĭ��Ϊ1

			BufferType type; //���������

#ifdef _WIN32
		public:
			D3D11_BUFFER_DESC desc; //��������Ľṹ

			ID3D11Buffer* buffer; //������Դ
#endif // _WIN32

		public:
			/**
			 * @brief ���캯��
			 * @param[in] data Ҫ���뻺���е�����
			 * @param[in] dataSize ����Ĵ�С����λbyte
			 * @param[in] dataCount Ԫ�ظ��������뻺���е������һ�����飬��ô�뽫������Ϊ����ĳ��ȷ�������Ϊ1
			 * @param[in] bufferType ���������
			 */
			Buffer(Graphics* graphics, void* data, int dataSize, int dataCount = 1, BufferType bufferType = BufferType::ConstantBuffer);
			
			/**
			 * @brief ��������
			 */
			~Buffer();

			/**
			 * @brief �����ݸ��µ�������ȥ
			 * @param[in] data ���ݣ��뱣֤���ݴ�С�ͻ����Сһ��
			 */
			void Update(void* data);

			/** 
			 * @brief ��ȡ�����С
			 * @return �����С����λbyte
			 */
			auto GetSize() -> int;

			/**
			 * @brief ���ػ����е�Ԫ�ظ���
			 * @return Ԫ�ظ���
			 */
			auto GetCount() -> int;

			/**
			 * @brief ���ػ��������
			 * @return ���������
			 */
			auto GetType() -> BufferType;
		};
	}
}
