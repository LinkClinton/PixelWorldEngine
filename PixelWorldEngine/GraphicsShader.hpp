#pragma once

#include "pch.hpp"

#include "BaseStruct.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		class Graphics;

		class GraphicsShader {
		private:
			Graphics* graphics;

			std::vector<byte> compiledVertexShaderCode;
			std::vector<byte> compiledPixelShaderCode;

#ifdef _WIN32
		public:
			ID3D11VertexShader* vertexShader;
			ID3D11PixelShader* pixelShader;
#endif // _WIN32

		public:
			GraphicsShader(Graphics* graphics, std::vector<byte> VertexShaderCode, std::vector<byte> PixelShaderCode, bool isCompiled = false);

			auto GetVertexShaderCode() -> std::vector<byte>;

			auto GetPixelShaderCode() -> std::vector<byte>;

			~GraphicsShader();
		};

	}

}
