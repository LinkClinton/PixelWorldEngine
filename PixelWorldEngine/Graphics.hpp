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
		 * @brief 图形模式
		 */
		enum GraphicsMode {
			Low, //低
			High //高
		};

		/**
		 * @brief 渲染填充模式
		 */
		enum class FillMode {
			FillWireFrame = 2, //只渲染线框
			FillSolid = 3 //渲染实体
		};

		/**
		 * @brief 图形接口
		 */
		class Graphics {
		private:
			FillMode fillMode; //填充模式，默认为FillSolid
			bool blendEnable; //是否开启混合，默认为false

			FT_Library fontLibrary; //FreeType字体库

			GraphicsMode graphicsMode; //图形模式，表示其版本高低

			friend class Font;
		public:

#ifdef _WIN32
			ID3D11Device* device; //Direct3D 设备
			ID3D11DeviceContext* deviceContext; //上下文

			D3D11_INPUT_ELEMENT_DESC elementDesc[3]; //顶点输入格式
			ID3D11InputLayout* inputLayout; //输入布局

			D3D11_RASTERIZER_DESC rasterizerDesc; //描述光栅化状态
			ID3D11RasterizerState* rasterizerState; //光栅化状态

			D3D11_BLEND_DESC blendDesc; //描述混合状态
			ID3D11BlendState* blendState; //混合状态

			D3D_FEATURE_LEVEL feature; //特征等级
#endif // _WIN32

		public:
			/**
			 * @brief 构造函数
			 */
			Graphics();

			/**
			 * @brief 析构函数
			 */
			~Graphics();

			/**
			 * @brief 设置着色器
			 * @param[in] shader 着色器，目前只支持顶点和像素
			 */
			void SetShader(GraphicsShader* shader);

			/**
			 * @brief 设置顶点缓冲
			 * @param[in] buffer 顶点缓冲
			 */
			void SetVertexBuffer(Buffer* buffer);
			
			/**
			 * @brief 设置索引缓冲
			 * @param[in] buffer 索引缓冲
			 */
			void SetIndexBuffer(Buffer* buffer);

			/**
			 * @brief 设置常量缓冲到着色器
			 * @param[in] buffer 缓冲数据
			 * @param[in] id 对应着色器数据编号
			 */
			void SetConstantBuffer(Buffer* buffer, int id);

			/**
			 * @brief 设置着色器资源到着色器
			 * @param[in] shaderResource 着色器资源
			 * @param[in] id 对应着色器资源编号
			 */
			void SetShaderResource(ShaderResource* shaderResource, int id);

			/**
			 * @brief 设置采样器资源到着色器
			 * @param[in] staticSampler 采样器资源
			 * @param[in] id 对应的采样器资源编号
			 */
			void SetStaticSampler(StaticSampler* staticSampler, int id);

			/**
			* @brief 设置常量缓冲到着色器
			* @param[in] buffer 缓冲数据
			* @param[in] startID 对应着色器数据的起始编号，然后依次往后推
			*/
			void SetConstantBuffers(std::vector<Buffer*> buffer, int startID);

			/**
			* @brief 设置着色器资源到着色器
			* @param[in] shaderResource 着色器资源
			* @param[in] startID 对应着色器资源的起始编号，然后依次往后推
			*/
			void SetShaderResources(std::vector<ShaderResource*> shaderResource, int startID);

			/**
			* @brief 设置采样器资源到着色器
			* @param[in] staticSampler 采样器资源
			* @param[in] startID 对应的采样器资源的起始编号，然后依次往后推
			*/
			void SetStaticSampelrs(std::vector<StaticSampler*> staticSampler, int startID);

			/**
			 * @brief 设置渲染目标
			 * @param[in] renderTarget 渲染目标
			 */
			void SetRenderTarget(RenderTarget* renderTarget);
	
			/**
			 * @brief 设置渲染的填充模式
			 * @param[in] fillMode 填充模式，默认为FillSolid
			 */
			void SetFillMode(FillMode fillMode);

			/**
			 * @brief 设置混合状态，是否启用混合来实现不透明度，对于绘制的物体，其混合时的方程为 Src * Src.a + Dst * (1 - Src.a)，其中Src表示我们渲染出来的像素，Dst表示在后台缓冲中的像素
			 * @param[in] state 混合状态
			 */
			void SetBlendState(bool state);

			/**
			 * @brief 设置我们渲染的范围(渲染目标上的)
			 * @param[in] rect 渲染的范围
			 */
			void SetViewPort(PixelWorldEngine::RectangleF rect);

			/**
			 * @brief 重置状态
			 */
			void ClearState();

			/**
			 * @brief 绘制物体
			 * @param[in] indexCount 使用的索引缓冲个数
			 * @param[in] startIndexLocation 从哪个位置开始读取indexCount个索引缓冲来绘制物体
			 * @param[in] baseVertexLocation 从baseVertexLocation位置开始，顶点从0编号，然后通过索引的值来获取具体的顶点数据
			 */
			void DrawIndexed(int indexCount, int startIndexLocation = 0, int baseVertexLocation = 0);

			/**
			* @brief 绘制多个物体
			* @param[in] indexCount 使用的索引缓冲个数
			* @param[in[ instanceCount 绘制的物体数量
			* @param[in] startIndexLocation 从哪个位置开始读取indexCount个索引缓冲来绘制物体
			* @param[in] baseVertexLocation 从baseVertexLocation位置开始，顶点从0编号，然后通过索引的值来获取具体的顶点数据
			*/
			void DrawIndexedInstanced(int indexCount, int instanceCount = 1, int startIndexLocation = 0, int baseVertexLocation = 0);

			/**
			 * @brief 获取图形模式
			 * @return 图形模式
			 */
			auto GetGraphicsMode() -> GraphicsMode;
		};
	}

}
