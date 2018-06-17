#include "pch.hpp"

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "Utility.hpp"
#include "DebugLayer.hpp"

#include "GraphicsEnum.hpp"
#include "GraphicsShader.hpp"
#include "BufferResource.hpp"
#include "ShaderResource.hpp"
#include "GraphicsRenderTarget.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		enum class FillMode {
			FillWireFrame = 2,
			FillSolid = 3
		};

		class Graphics {
		private:
			int resolutionWidth, resolutionHeight;
		public:

#ifdef WINDOWS
			ID3D11Device* device;
			ID3D11DeviceContext* deviceContext;

			D3D11_INPUT_ELEMENT_DESC elementDesc[3];
			ID3D11InputLayout* inputLayout;

			D3D11_RASTERIZER_DESC rasterizerDesc;
			ID3D11RasterizerState* rasterizerState;

			D3D11_BLEND_DESC blendDesc;
			ID3D11BlendState* blendState;

			D3D_FEATURE_LEVEL feature;
#endif // WINDOWS

		public:
			Graphics();
			
			void SetShader(GraphicsShader* shader);

			void SetVertexBuffer(Buffer* buffer);
			
			void SetIndexBuffer(Buffer* buffer);

			void SetConstantBuffer(Buffer* buffer, int id);

			void SetShaderResource(ShaderResource* shaderResource, int id);

			void SetConstantBuffers(std::vector<Buffer*> buffer, int startID);

			void SetShaderResources(std::vector<ShaderResource*> shaderResource, int startID);

			void SetRenderTarget(RenderTarget* renderTarget);
	
			void SetFillMode(FillMode fillMode);

			//�Ƿ����û����ʵ�ֲ�͸���ȣ������Ƕ��ڻ��Ƶ����壬����ʱ�ķ���Ϊ Src * Src.a + Dst * (1 - Src.a)
			//����Src��ʾ������Ⱦ���������أ�Dst��ʾ�ں�̨�����е����ء�
			void SetBlendState(bool state);

			void DrawIndexed(int indexCount, int startIndexLocation = 0, int baseVertexLocation = 0);

			~Graphics();
		};
	}

}

#endif // !_GRAPHICS_H_
