#pragma once

#include "pch.hpp"

#include "Utility.hpp"
#include "DebugLayer.hpp"

#include "GraphicsEnum.hpp"
#include "StaticSampler.hpp"
#include "GraphicsShader.hpp"
#include "GraphicsFont.hpp"
#include "BufferResource.hpp"
#include "ShaderResource.hpp"
#include "GraphicsRenderTarget.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		/**
		 * @brief ͼ��ģʽ
		 */
		enum GraphicsMode {
			Low, //��
			High //��
		};

		/**
		 * @brief ��Ⱦ���ģʽ
		 */
		enum class FillMode {
			FillWireFrame = 2, //ֻ��Ⱦ�߿�
			FillSolid = 3 //��Ⱦʵ��
		};

		/**
		 * @brief ͼ�νӿ�
		 */
		class Graphics {
		private:
			FillMode fillMode; //���ģʽ��Ĭ��ΪFillSolid
			bool blendEnable; //�Ƿ�����ϣ�Ĭ��Ϊfalse

			FT_Library fontLibrary; //FreeType�����

			GraphicsMode graphicsMode; //ͼ��ģʽ����ʾ��汾�ߵ�

			friend class Font;
		public:

#ifdef _WIN32
			ID3D11Device* device; //Direct3D �豸
			ID3D11DeviceContext* deviceContext; //������

			D3D11_INPUT_ELEMENT_DESC elementDesc[3]; //���������ʽ
			ID3D11InputLayout* inputLayout; //���벼��

			D3D11_RASTERIZER_DESC rasterizerDesc; //������դ��״̬
			ID3D11RasterizerState* rasterizerState; //��դ��״̬

			D3D11_BLEND_DESC blendDesc; //�������״̬
			ID3D11BlendState* blendState; //���״̬

			D3D_FEATURE_LEVEL feature; //�����ȼ�
#endif // _WIN32

		public:
			/**
			 * @brief ���캯��
			 */
			Graphics();

			/**
			 * @brief ��������
			 */
			~Graphics();

			/**
			 * @brief ������ɫ��
			 * @param[in] shader ��ɫ����Ŀǰֻ֧�ֶ��������
			 */
			void SetShader(GraphicsShader* shader);

			/**
			 * @brief ���ö��㻺��
			 * @param[in] buffer ���㻺��
			 */
			void SetVertexBuffer(Buffer* buffer);
			
			/**
			 * @brief ������������
			 * @param[in] buffer ��������
			 */
			void SetIndexBuffer(Buffer* buffer);

			/**
			 * @brief ���ó������嵽��ɫ��
			 * @param[in] buffer ��������
			 * @param[in] id ��Ӧ��ɫ�����ݱ��
			 */
			void SetConstantBuffer(Buffer* buffer, int id);

			/**
			 * @brief ������ɫ����Դ����ɫ��
			 * @param[in] shaderResource ��ɫ����Դ
			 * @param[in] id ��Ӧ��ɫ����Դ���
			 */
			void SetShaderResource(ShaderResource* shaderResource, int id);

			/**
			 * @brief ���ò�������Դ����ɫ��
			 * @param[in] staticSampler ��������Դ
			 * @param[in] id ��Ӧ�Ĳ�������Դ���
			 */
			void SetStaticSampler(StaticSampler* staticSampler, int id);

			/**
			* @brief ���ó������嵽��ɫ��
			* @param[in] buffer ��������
			* @param[in] startID ��Ӧ��ɫ�����ݵ���ʼ��ţ�Ȼ������������
			*/
			void SetConstantBuffers(std::vector<Buffer*> buffer, int startID);

			/**
			* @brief ������ɫ����Դ����ɫ��
			* @param[in] shaderResource ��ɫ����Դ
			* @param[in] startID ��Ӧ��ɫ����Դ����ʼ��ţ�Ȼ������������
			*/
			void SetShaderResources(std::vector<ShaderResource*> shaderResource, int startID);

			/**
			* @brief ���ò�������Դ����ɫ��
			* @param[in] staticSampler ��������Դ
			* @param[in] startID ��Ӧ�Ĳ�������Դ����ʼ��ţ�Ȼ������������
			*/
			void SetStaticSampelrs(std::vector<StaticSampler*> staticSampler, int startID);

			/**
			 * @brief ������ȾĿ��
			 * @param[in] renderTarget ��ȾĿ��
			 */
			void SetRenderTarget(RenderTarget* renderTarget);
	
			/**
			 * @brief ������Ⱦ�����ģʽ
			 * @param[in] fillMode ���ģʽ��Ĭ��ΪFillSolid
			 */
			void SetFillMode(FillMode fillMode);

			/**
			 * @brief ���û��״̬���Ƿ����û����ʵ�ֲ�͸���ȣ����ڻ��Ƶ����壬����ʱ�ķ���Ϊ Src * Src.a + Dst * (1 - Src.a)������Src��ʾ������Ⱦ���������أ�Dst��ʾ�ں�̨�����е�����
			 * @param[in] state ���״̬
			 */
			void SetBlendState(bool state);

			/**
			 * @brief ����������Ⱦ�ķ�Χ(��ȾĿ���ϵ�)
			 * @param[in] rect ��Ⱦ�ķ�Χ
			 */
			void SetViewPort(PixelWorldEngine::RectangleF rect);

			/**
			 * @brief ����״̬
			 */
			void ClearState();

			/**
			 * @brief ��������
			 * @param[in] indexCount ʹ�õ������������
			 * @param[in] startIndexLocation ���ĸ�λ�ÿ�ʼ��ȡindexCount��������������������
			 * @param[in] baseVertexLocation ��baseVertexLocationλ�ÿ�ʼ�������0��ţ�Ȼ��ͨ��������ֵ����ȡ����Ķ�������
			 */
			void DrawIndexed(int indexCount, int startIndexLocation = 0, int baseVertexLocation = 0);

			/**
			* @brief ���ƶ������
			* @param[in] indexCount ʹ�õ������������
			* @param[in[ instanceCount ���Ƶ���������
			* @param[in] startIndexLocation ���ĸ�λ�ÿ�ʼ��ȡindexCount��������������������
			* @param[in] baseVertexLocation ��baseVertexLocationλ�ÿ�ʼ�������0��ţ�Ȼ��ͨ��������ֵ����ȡ����Ķ�������
			*/
			void DrawIndexedInstanced(int indexCount, int instanceCount = 1, int startIndexLocation = 0, int baseVertexLocation = 0);

			/**
			 * @brief ��ȡͼ��ģʽ
			 * @return ͼ��ģʽ
			 */
			auto GetGraphicsMode() -> GraphicsMode;
		};
	}

}
