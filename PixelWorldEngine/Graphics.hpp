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

		enum class FillMode {
			FillWireFrame = 2,
			FillSolid = 3
		};

		class Graphics {
		private:
			int resolutionWidth, resolutionHeight;

			FillMode fillMode;
			bool blendEnable;

			FT_Library fontLibrary;

			friend class Font;
		public:

#ifdef _WIN32
			ID3D11Device* device;
			ID3D11DeviceContext* deviceContext;

			D3D11_INPUT_ELEMENT_DESC elementDesc[3];
			ID3D11InputLayout* inputLayout;

			D3D11_RASTERIZER_DESC rasterizerDesc;
			ID3D11RasterizerState* rasterizerState;

			D3D11_BLEND_DESC blendDesc;
			ID3D11BlendState* blendState;

			D3D_FEATURE_LEVEL feature;
#endif // _WIN32

		public:
			Graphics();

			void SetShader(GraphicsShader* shader);

			void SetVertexBuffer(Buffer* buffer);
			
			void SetIndexBuffer(Buffer* buffer);

			void SetConstantBuffer(Buffer* buffer, int id);

			void SetShaderResource(ShaderResource* shaderResource, int id);

			void SetStaticSampler(StaticSampler* staticSampler, int id);

			void SetConstantBuffers(std::vector<Buffer*> buffer, int startID);

			void SetShaderResources(std::vector<ShaderResource*> shaderResource, int startID);

			void SetStaticSampelrs(std::vector<StaticSampler*> staticSampler, int startID);

			void SetRenderTarget(RenderTarget* renderTarget);
	
			void SetFillMode(FillMode fillMode);

			//是否启用混合来实现不透明度，大致是对于绘制的物体，其混合时的方程为 Src * Src.a + Dst * (1 - Src.a)
			//其中Src表示我们渲染出来的像素，Dst表示在后台缓冲中的像素。
			void SetBlendState(bool state);

			//表示整个后台缓冲中可见的范围
			void SetViewPort(PixelWorldEngine::RectangleF rect);

			void ClearState();

			void DrawIndexed(int indexCount, int startIndexLocation = 0, int baseVertexLocation = 0);

			void DrawIndexedInstanced(int indexCount, int instanceCount = 1, int startIndexLocation = 0, int baseVertexLocation = 0);

			~Graphics();
		};
	}

}
