#pragma once

#include "pch.hpp"

#include "BaseStruct.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		class Graphics;

		/**
		 * @brief 着色器
		 */
		class GraphicsShader {
		private:
			Graphics* graphics; //..

			std::vector<byte> compiledVertexShaderCode; //编译后的顶点着色器
			std::vector<byte> compiledPixelShaderCode; //编译后的像素着色器

#ifdef _WIN32
		public:
			ID3D11VertexShader* vertexShader; //Direct3D 接口
			ID3D11PixelShader* pixelShader; //Direct3D 接口
#endif // _WIN32

		public:
			/**
			 * @brief 构造函数，对于HLSL来说其入口点函数必须是main，对于GLSL来说他已经默认了能如何QAQ
			 * @param[in] graphics 图形接口
			 * @param[in] VertexShaderCode 顶点着色器代码
			 * @param[in] PixelShaderCode 像素着色器代码
			 * @param[in] isCompiled 是否预编译
			 */
			GraphicsShader(Graphics* graphics, std::vector<byte> VertexShaderCode, std::vector<byte> PixelShaderCode, bool isCompiled = false);

			/**
			 * @brief 析构函数
			 */
			~GraphicsShader();

			/**
			 * @brief 获取顶点着色器代码
			 * @return 顶点着色器代码
			 */
			auto GetVertexShaderCode() -> std::vector<byte>;

			/**
			 * @brief 获取像素着色器代码
			 * @return 像素着色器代码
			 */
			auto GetPixelShaderCode() -> std::vector<byte>;
		};

	}

}
