#pragma once

#include "pch.hpp"

#include "BaseStruct.hpp"

#include "Graphics.hpp"
#include "BufferResource.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		/**
		 * @brief ͼ�λ������ͣ�����������ֻ���ͼ��
		 */
		class Geometry {
		protected:
			Graphics* graphics; //..

			Buffer* vertexBuffer; //���㻺��
			Buffer* indexBuffer; //��������
		public:
			/**
			 * @brief ���캯��
			 * @param[in] graphics ͼ�νӿ�
			 */
			Geometry(Graphics* graphics);

			/**
			 * @brief ��������
			 */
			~Geometry();

			/**
			 * @brief ��ȡ���㻺��
			 * @return ���㻺��ĵ�ַ
			 */
			auto GetVertexBuffer() -> Buffer*;

			/**
			 * @brief ��ȡ��������
			 * @return ��������ĵ�ַ
			 */
			auto GetIndexBuffer() -> Buffer*;
		};

		/**
		 * @brief ����ͼ�����ͣ�ע���䶥��Z�᷽���ֵΪ1
		 */
		class RectangleF :public Geometry {
		public:
			/**
			 * @brief ���캯��
			 * @param[in] left ��߽�
			 * @param[in] top �ϱ߽�
			 * @param[in] right �ұ߽�
			 * @param[in] bottom �±߽�
			 * @param[in] graphics ͼ�νӿ�
			 */
			RectangleF(float left, float top, float right, float bottom, Graphics* graphics);
		};

	}

}