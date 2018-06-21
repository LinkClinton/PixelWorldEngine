#pragma once

#include "pch.hpp"

#include "BaseStruct.hpp"

#include "Graphics.hpp"
#include "BufferResource.hpp"

namespace PixelWorldEngine {

	namespace Graphics {

		class Geometry {
		protected:
			Graphics* graphics;

			Buffer* vertexBuffer;
			Buffer* indexBuffer;
		public:
			Geometry(Graphics* graphics);

			~Geometry();

			auto GetVertexBuffer() -> Buffer*;

			auto GetIndexBuffer() -> Buffer*;
		};

		class RectangleF :public Geometry {
		public:
			RectangleF(float left, float top, float right, float bottom, Graphics* graphics);
		};

	}

}