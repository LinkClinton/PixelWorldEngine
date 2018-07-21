#pragma once

#include "pch.hpp"

#define GLM_MDOE

namespace PixelWorldEngine {

	class Matrix;

	class Vector {
	public:
		float x, y, z, w;
	public:
		Vector(float x = 0, float y = 0, float z = 0, float w = 0);

		auto Length() -> float;

		auto Normalize() -> Vector;

		friend auto operator * (Vector &left, Vector &right) -> Vector;

		friend auto operator * (Vector &left, float &right) -> Vector;

		friend auto operator * (Vector &left, Matrix &right) -> Vector;

		friend auto operator *= (Vector &left, Vector &right) -> Vector;

		friend auto operator *= (Vector &left, float &right) -> Vector;

		friend auto operator *= (Vector &left, Matrix &right) -> Vector;

		friend auto operator / (Vector &left, Vector &right) -> Vector;
		
		friend auto operator / (Vector &left, float &right) -> Vector;

		friend auto operator /=(Vector &left, Vector &right) -> Vector;

		friend auto operator /=(Vector &left, float &right) -> Vector;

		friend auto operator + (Vector &left, Vector &right)->Vector;

		friend auto operator += (Vector &left, Vector &right)->Vector;

		friend auto operator -(Vector &left, Vector &right)->Vector;

		friend auto operator -=(Vector &left, Vector &right)->Vector;

		static auto Multiply(Vector &left, Vector &right) -> Vector;

		static auto Multiply(Vector &left, float &right) -> Vector;

		static auto Multiply(Vector &left, Matrix &right) -> Vector;

		static auto Multiply(Matrix &left, Vector &right) -> Vector;

		static auto Divide(Vector &left, Vector &right) -> Vector;

		static auto Divide(Vector &left, float &right) -> Vector;

		static auto Add(Vector &left, Vector &right) -> Vector;

		static auto Minus(Vector &left, Vector &right) -> Vector;

		static auto Length(Vector &vector) -> float;

		static auto Normalize(Vector &vector)->Vector;
	};

}