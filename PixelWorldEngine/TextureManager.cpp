#include "TextureManager.hpp"

#include "Application.hpp"
#include "Utility.hpp"

PixelWorldEngine::TextureManager::TextureManager(Application * application, int WidthRequirement, int HeightRequirement)
{
	graphics = application->GetGraphics();

	widthRequirement = WidthRequirement;
	heightRequirement = HeightRequirement;

	maxRenderObjectID = 0;

	finalTexture = nullptr;
}

void PixelWorldEngine::TextureManager::AddTexture(int id, Graphics::Texture2D * texture)
{
	DebugLayer::Assert(texture->GetWidth() != widthRequirement ||
		texture->GetHeight() != heightRequirement, Error::ErrorTextureWidthOrHeight);

	DebugLayer::Assert(texture->GetPixelFormat() != Graphics::PixelFormat::R8G8B8A8, Error::ErrorPixelFormat);

	if (id <= 0) return;

	textures[id] = texture;

	maxRenderObjectID = Utility::Max(maxRenderObjectID, id);
}

void PixelWorldEngine::TextureManager::RemoveTexture(int id)
{
	if (textures.count(id) != 0)
		textures.erase(id);
}

void PixelWorldEngine::TextureManager::MergeTextures()
{
	Utility::Delete(finalTexture);

	finalTexture = new Graphics::Texture2D(graphics, nullptr, widthRequirement * maxRenderObjectID, heightRequirement,
		Graphics::PixelFormat::R8G8B8A8);

	for (auto it = textures.begin(); it != textures.end(); it++) {

		finalTexture->CopyFromTexture2D(it->second, (it->first - 1) * widthRequirement, 0,
			PixelWorldEngine::Rectangle(0, 0, widthRequirement, heightRequirement));
	}
}

auto PixelWorldEngine::TextureManager::GetFinalTexture() -> Graphics::Texture2D *
{
	return finalTexture;
}

auto PixelWorldEngine::TextureManager::GetWidthRequirement() -> int
{
	return widthRequirement;
}

auto PixelWorldEngine::TextureManager::GetHeightRequirement() -> int
{
	return heightRequirement;
}

auto PixelWorldEngine::TextureManager::GetMaxRenderObjectID() -> int
{
	return maxRenderObjectID;
}
