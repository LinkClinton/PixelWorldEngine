#include "Transform.hpp"

PixelWorldEngine::Transform::Transform()
{
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	forward = glm::vec3(0, 1, 0);
	angle = 0.0f;

	matrix = glm::mat4(1);
}

void PixelWorldEngine::Transform::SetPosition(glm::vec2 Position)
{
	position = glm::vec3(Position, 0.0f);

	matrix = CreateMatrixFromTransform(*this);
}

void PixelWorldEngine::Transform::SetScale(glm::vec2 Scale)
{
	scale = glm::vec3(Scale, 1.0f);

	matrix = CreateMatrixFromTransform(*this);
}

void PixelWorldEngine::Transform::SetForward(glm::vec2 Forward)
{
	if (Forward == glm::vec2(0.0f, 0.0f)) Forward = glm::vec2(0.0f, 1.0f);

	forward = glm::normalize(glm::vec3(Forward, 0.0f));

	angle = 0.0f;

	if (forward.y == 0.0f && forward.x > 0.0f)
		angle = glm::pi<float>() * 0.5f;

	if (forward.y == 0.0f && forward.x < 0.0f)
		angle = glm::pi<float>() * 1.5f;

	if (forward.y != 0.0f && forward.x != 0.0f)
		angle = glm::atan(forward.x / forward.y);

	matrix = CreateMatrixFromTransform(*this);
}

void PixelWorldEngine::Transform::SetRotate(float Angle)
{
	angle = Angle;

	forward = glm::rotate(glm::mat4(1), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	matrix = CreateMatrixFromTransform(*this);
}

auto PixelWorldEngine::Transform::GetPosition() -> glm::vec2
{
	return position;
}

auto PixelWorldEngine::Transform::GetScale() -> glm::vec2
{
	return scale;
}

auto PixelWorldEngine::Transform::GetForward() -> glm::vec2
{
	return forward;
}

auto PixelWorldEngine::Transform::GetRotate() -> float
{
	return angle;
}

auto PixelWorldEngine::Transform::GetMatrix() -> glm::mat4x4
{
	return matrix;
}

auto PixelWorldEngine::Transform::CreateMatrixFromTransform(const Transform & transform) -> glm::mat4x4
{
	glm::mat4x4 transformMatrix = glm::mat4(1);

	transformMatrix = glm::translate(transformMatrix, transform.position);
	transformMatrix = glm::rotate(transformMatrix, transform.angle, glm::vec3(0.0f, 0.0f, 1.0f));
	transformMatrix = glm::scale(transformMatrix, transform.scale);

	return transformMatrix;
}

auto PixelWorldEngine::operator+(const Transform & left, const Transform & right) -> Transform
{
	Transform result;

	result.position = left.position + right.position;
	result.scale = left.scale + right.scale;
	result.SetRotate(left.angle + right.angle);

	return result;
}

auto PixelWorldEngine::operator-(const Transform & left, const Transform & right) -> Transform
{
	Transform result;

	result.position = left.position - right.position;
	result.scale = left.scale - right.scale;
	result.SetRotate(left.angle - right.angle);

	return result;
}

auto PixelWorldEngine::operator*(const Transform & left, float value) -> Transform
{
	Transform result;

	result.position = left.position * value;
	result.scale = left.scale * value;
	result.SetRotate(left.angle * value);

	return result;
}

auto PixelWorldEngine::operator/(const Transform & left, float value) -> Transform
{
	Transform result;

	result.position = left.position / value;
	result.scale = left.scale / value;
	result.SetRotate(left.angle / value);

	return result;
}