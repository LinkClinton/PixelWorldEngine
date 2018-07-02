#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	struct Vertex {
		float x, y, z;
		float red, green, blue, alpha;
		float u0, v0;

		void SetPosition(float X, float Y, float Z) {
			x = X;
			y = Y;
			z = Z;
		}

		void SetTexturePosition1(float u, float v) {
			u0 = u;
			v0 = v;
		}
	};

	struct RectangleF {
		float left;
		float top;
		float right;
		float bottom;

		RectangleF() = default;

		RectangleF(float Left, float Top, float Right, float Bottom) {
			left = Left;
			top = Top;
			right = Right;
			bottom = Bottom;
		}

		static auto Transform(RectangleF rect, float x, float y) -> RectangleF {
			return RectangleF(rect.left + x, rect.top + y, rect.right + x, rect.bottom + y);
		}

	};

	struct Rectangle {
		int left;
		int top;
		int right;
		int bottom;

		Rectangle() = default;
		
		Rectangle(int Left, int Top, int Right, int Bottom) {
			left = Left;
			top = Top;
			right = Right;
			bottom = Bottom;
		}
	};

}

