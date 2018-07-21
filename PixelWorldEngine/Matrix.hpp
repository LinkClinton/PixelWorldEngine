#pragma once

#include "pch.hpp"

#include "Vector.hpp"

namespace PixelWorldEngine {

	class Matrix {
	public:
		Vector m[4];
	public:
		Matrix();

		Matrix(Vector data[4]);

		friend auto operator * (Matrix &left, Matrix &right) -> Matrix;

		friend auto operator * (Matrix &left, Vector &right) -> Vector;

		friend auto operator *= (Matrix &left, Matrix & right) -> Matrix;

		static auto Multiply(Matrix &left, Matrix &right) -> Matrix;

		static auto Multiply(Matrix &left, Vector &right) -> Vector;

		static auto Multiply(Vector &left, Matrix &right) -> Vector;

		static auto Translate(Vector &translate) -> Matrix;

		static auto Translate(Matrix &matrix, Vector &translate) -> Matrix;

		static auto Rotate(float angle, Vector &axis) -> Matrix;

		static auto Rotate(Matrix &matrix, float angle, Vector &axis) -> Matrix;

		static auto Scale(Vector &vector) -> Matrix;

		static auto Scale(Matrix &matrix, Vector &vector) -> Matrix;

		static auto Identity() -> Matrix;
	};

}