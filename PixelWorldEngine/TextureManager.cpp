#include "TextureManager.hpp"

#include "Application.hpp"
#include "Utility.hpp"

PixelWorldEngine::TextureManager::TextureManager(Application * application)
{
	graphics = application->GetGraphics();

	memset(mergeTextures, 0, sizeof(mergeTextures));
}

void PixelWorldEngine::TextureManager::AddMergeTexture(int id, MergeTexture * mergeTexture)
{
	if (id >= MAX_MERGETEXTURE_COUNT) return;

	mergeTextures[id] = mergeTexture;
}

void PixelWorldEngine::TextureManager::RemoveMergeTexture(int id)
{
	mergeTextures[id] = nullptr;
}

auto PixelWorldEngine::TextureManager::GetWhich(int renderObjectID) -> int
{
	for (int i = 0; i < MAX_MERGETEXTURE_COUNT; i++) {
		if (mergeTextures[i] == nullptr) continue;

		if (mergeTextures[i]->IsExistID(renderObjectID) == true) return i;
	}

	return MAX_MERGETEXTURE_COUNT;
}