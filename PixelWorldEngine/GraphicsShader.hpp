#pragma once

#include "pch.hpp"

#include "BaseStruct.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		class Graphics;

		/**
		 * @brief ��ɫ��
		 */
		class GraphicsShader {
		private:
			Graphics* graphics; //..

			std::vector<byte> compiledVertexShaderCode; //�����Ķ�����ɫ��
			std::vector<byte> compiledPixelShaderCode; //������������ɫ��

#ifdef _WIN32
		public:
			ID3D11VertexShader* vertexShader; //Direct3D �ӿ�
			ID3D11PixelShader* pixelShader; //Direct3D �ӿ�
#endif // _WIN32

		public:
			/**
			 * @brief ���캯��������HLSL��˵����ڵ㺯��������main������GLSL��˵���Ѿ�Ĭ���������QAQ
			 * @param[in] graphics ͼ�νӿ�
			 * @param[in] VertexShaderCode ������ɫ������
			 * @param[in] PixelShaderCode ������ɫ������
			 * @param[in] isCompiled �Ƿ�Ԥ����
			 */
			GraphicsShader(Graphics* graphics, std::vector<byte> VertexShaderCode, std::vector<byte> PixelShaderCode, bool isCompiled = false);

			/**
			 * @brief ��������
			 */
			~GraphicsShader();

			/**
			 * @brief ��ȡ������ɫ������
			 * @return ������ɫ������
			 */
			auto GetVertexShaderCode() -> std::vector<byte>;

			/**
			 * @brief ��ȡ������ɫ������
			 * @return ������ɫ������
			 */
			auto GetPixelShaderCode() -> std::vector<byte>;
		};

	}

}
