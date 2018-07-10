#pragma once

#include "pch.hpp"

#include "Graphics.hpp"
#include "ShaderResource.hpp"

namespace PixelWorldEngine {

	struct SubTexture {
		glm::vec2 startPosition;
		glm::vec2 endPosition;
		
		int startPositionX;
		int startPositionY;
		int width;
		int height;

		glm::mat4x4 transform;

		void MakeTexCoordPosition(int maxWidth, int maxHeight);

		static bool IsEnter(SubTexture texture1, SubTexture texture2);
	};

	class MergeTexture {
	private:
		Graphics::Graphics* graphics;
		
		Graphics::Texture2D* finalTexture;

		std::map<int, SubTexture> subTextures;
	public:
		MergeTexture(Application* application, int width, int height);

		~MergeTexture();

		void AddTexture(int id, int positionX, int positionY, Graphics::Texture2D* texture);

		void RemoveTexture(int id);

		auto GetTexCoordTransform(int id) -> glm::mat4x4;

		auto GetFinalTexture() -> Graphics::Texture2D*;

		auto IsExistID(int id) -> bool;
	};

}