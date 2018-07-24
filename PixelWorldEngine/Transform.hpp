#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	class Transform {
	private:
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 forward;

		float angle;

		glm::mat4x4 matrix;
	public:
		Transform();

		void SetPosition(glm::vec2 position);

		void SetScale(glm::vec2 scale);

		void SetForward(glm::vec2 forward);

		void SetRotate(float angle);

		auto GetPosition() -> glm::vec2;

		auto GetScale() -> glm::vec2;

		auto GetForward() -> glm::vec2;

		auto GetRotate() -> float;

		auto GetMatrix() -> glm::mat4x4;

		static auto CreateMatrixFromTransform(const Transform &transform) -> glm::mat4x4;
	};

}
