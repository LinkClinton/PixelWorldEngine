#include "Vector.hpp"
#include "Matrix.hpp"

#include <glm\glm.hpp>


extern auto Convert(PixelWorldEngine::Matrix matrix) -> glm::mat4x4;

extern auto Convert(glm::mat4x4 matrix) -> PixelWorldEngine::Matrix;

extern auto Convert(PixelWorldEngine::Vector vector) -> glm::vec4;

extern auto Convert(glm::vec4 vector) -> PixelWorldEngine::Vector;

PixelWorldEngine::Vector::Vector(float X, float Y, float Z, float W)
{
	x = X;
	y = Y;
	z = Z;
	w = W;
}

auto PixelWorldEngine::Vector::Length() -> float
{
	return Vector::Length(*this);
}

auto PixelWorldEngine::Vector::Normalize() -> Vector
{
	return Vector::Normalize(*this);
}

auto PixelWorldEngine::operator*(Vector & left, Vector & right) -> Vector
{
	return Vector::Multiply(left, right);
}

auto PixelWorldEngine::operator*(Vector & left, float & right) -> Vector
{
	return Vector::Multiply(left, right);
}

auto PixelWorldEngine::operator*(Vector & left, Matrix & right) -> Vector
{
	return Vector::Multiply(left, right);
}

auto PixelWorldEngine::operator*=(Vector & left, Vector & right) -> Vector
{
	return left = Vector::Multiply(left, right);
}

auto PixelWorldEngine::operator*=(Vector & left, float & right) -> Vector
{
	return left = Vector::Multiply(left, right);
}

auto PixelWorldEngine::operator*=(Vector & left, Matrix & right) -> Vector
{
	return left = Vector::Multiply(left, right);
}

auto PixelWorldEngine::operator/(Vector & left, Vector & right) -> Vector
{
	return Vector::Divide(left, right);
}

auto PixelWorldEngine::operator/(Vector & left, float & right) -> Vector
{
	return Vector::Divide(left, right);
}

auto PixelWorldEngine::operator/=(Vector & left, Vector & right) -> Vector
{
	return left = Vector::Divide(left, right);
}

auto PixelWorldEngine::operator/=(Vector & left, float & right) -> Vector
{
	return left = Vector::Divide(left, right);
}

auto PixelWorldEngine::operator+(Vector & left, Vector & right) -> Vector
{
	return Vector::Add(left, right);
}

auto PixelWorldEngine::operator+=(Vector & left, Vector & right) -> Vector
{
	return left = Vector::Add(left, right);
}

auto PixelWorldEngine::operator-(Vector & left, Vector & right) -> Vector
{
	return Vector::Minus(left, right);
}

auto PixelWorldEngine::operator-=(Vector & left, Vector & right) -> Vector
{
	return left = Vector::Minus(left, right);
}

auto PixelWorldEngine::Vector::Multiply(Vector &left, Vector &right) -> Vector
{
	return Convert(Convert(left) * Convert(right));
}

auto PixelWorldEngine::Vector::Multiply(Vector &left, float &right) -> Vector
{
	return Convert(Convert(left) * right);
}

auto PixelWorldEngine::Vector::Multiply(Vector & left, Matrix & right) -> Vector
{
	return Convert(Convert(left) * Convert(right));
}

auto PixelWorldEngine::Vector::Multiply(Matrix & left, Vector & right) -> Vector
{
	return Convert(Convert(left) * Convert(right));
}

auto PixelWorldEngine::Vector::Divide(Vector &left, Vector &right) -> Vector
{
	return Convert(Convert(left) / Convert(right));
}

auto PixelWorldEngine::Vector::Divide(Vector &left, float &right) -> Vector
{
	return Convert(Convert(left) / right);
}

auto PixelWorldEngine::Vector::Add(Vector &left, Vector &right) -> Vector
{
	return Convert(Convert(left) + Convert(right));
}

auto PixelWorldEngine::Vector::Minus(Vector &left, Vector &right) -> Vector
{
	return Convert(Convert(left) - Convert(right));
}

auto PixelWorldEngine::Vector::Length(Vector &vector) -> float
{
	return vector.x / vector.Normalize().x;
}

auto PixelWorldEngine::Vector::Normalize(Vector &vector) -> Vector
{
	return Convert(glm::normalize(Convert(vector)));
}


