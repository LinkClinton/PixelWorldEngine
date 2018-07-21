#include "Matrix.hpp"

auto Convert(PixelWorldEngine::Matrix matrix) -> glm::mat4x4 
{
	return *(glm::mat4x4*)&matrix;
}

auto Convert(glm::mat4x4 matrix) -> PixelWorldEngine::Matrix 
{
	return *(PixelWorldEngine::Matrix*)&matrix;
}

auto Convert(PixelWorldEngine::Vector vector) -> glm::vec4
{
	return *(glm::vec4*)&vector;
}

auto Convert(glm::vec4 vector) -> PixelWorldEngine::Vector {
	return *(PixelWorldEngine::Vector*)&vector;
}

PixelWorldEngine::Matrix::Matrix()
{
	
}

PixelWorldEngine::Matrix::Matrix(Vector data[4])
{
	m[0] = data[0];
	m[1] = data[1];
	m[2] = data[2];
	m[3] = data[3];
}

auto PixelWorldEngine::Matrix::Multiply(Matrix & left, Matrix & right) -> Matrix
{
	return Convert(Convert(left) * Convert(right));
}

auto PixelWorldEngine::Matrix::Multiply(Matrix & left, Vector & right) -> Vector
{
	return Convert(Convert(left) * Convert(right));
}

auto PixelWorldEngine::Matrix::Multiply(Vector & left, Matrix & right) -> Vector
{
	return Convert(Convert(left) * Convert(right));
}

auto PixelWorldEngine::Matrix::Identity() -> Matrix
{
	return Convert(glm::mat4(1));
}

auto PixelWorldEngine::Matrix::Translate(Vector &translate) -> Matrix
{
	return Convert(glm::translate(glm::mat4(1), glm::vec3(translate.x, translate.y, translate.z)));
}

auto PixelWorldEngine::Matrix::Translate(Matrix &matrix, Vector &translate) -> Matrix
{
	return Convert(glm::translate(Convert(matrix), glm::vec3(translate.x, translate.y, translate.z)));
}

auto PixelWorldEngine::Matrix::Rotate(float angle, Vector & axis) -> Matrix
{
	return Convert(glm::rotate(glm::mat4(1), angle, glm::vec3(axis.x, axis.y, axis.z)));
}

auto PixelWorldEngine::Matrix::Rotate(Matrix & matrix, float angle, Vector & axis) -> Matrix
{
	return Convert(glm::rotate(Convert(matrix), angle, glm::vec3(axis.x, axis.y, axis.z)));
}

auto PixelWorldEngine::Matrix::Scale(Vector & vector) -> Matrix
{
	return Convert(glm::scale(glm::mat4(1), glm::vec3(vector.x, vector.y, vector.z)));
}

auto PixelWorldEngine::Matrix::Scale(Matrix & matrix, Vector & vector) -> Matrix
{
	return Convert(glm::scale(Convert(matrix), glm::vec3(vector.x, vector.y, vector.z)));
}

auto PixelWorldEngine::operator*(Matrix & left, Matrix & right) -> Matrix
{
	return Matrix::Multiply(left, right);
}

auto PixelWorldEngine::operator*(Matrix & left, Vector & right) -> Vector
{
	return Matrix::Multiply(left, right);
}

auto PixelWorldEngine::operator*=(Matrix & left, Matrix & right) -> Matrix
{
	return left = Matrix::Multiply(left, right);
}
