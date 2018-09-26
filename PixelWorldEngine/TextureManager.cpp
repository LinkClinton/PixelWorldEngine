#include "TextureManager.hpp"

#include "Application.hpp"
#include "Utility.hpp"

int PixelWorldEngine::TextureManager::textureSizes[(int)PixelWorldEngine::TextureType::Count];
int PixelWorldEngine::TextureManager::textureSizeIndex[MAX_TEXTURE_SIZE + 1];
bool PixelWorldEngine::TextureManager::isInitialized = false;

void PixelWorldEngine::TextureManager::CopySubTexture(int id, SubTexture subTexture, Graphics::Texture2D * texture)
{
	subTexture.width = texture->GetWidth();
	subTexture.height = texture->GetHeight();
	
	textureArray->CopyFromTexture2D(texture, subTexture.startPositionX, subTexture.startPositionY, subTexture.arrayIndex,
		PixelWorldEngine::Rectangle(0, 0, subTexture.width, subTexture.height));

	subTexture.MakeTexCoordPosition(TEXTURE_WIDTH, TEXTURE_HEIGHT);

	subTextures.insert(std::pair<int, SubTexture>(id, subTexture));
}

PixelWorldEngine::TextureManager::TextureManager(Application * Application, Graphics::PixelFormat PixelFormat)
{
	graphics = Application->GetGraphics();

	pixelFormat = PixelFormat;

	textureCount = 0;
	textureSpace = TEXTURE_ARRAY_ADD;

	textureArray = new Graphics::Texture2DArray(graphics, TEXTURE_WIDTH, TEXTURE_HEIGHT, TEXTURE_ARRAY_ADD, pixelFormat);

	if (isInitialized == true) return;

	textureSizes[0] = TextureType::Size128;
	textureSizes[1] = TextureType::Size256;
	textureSizes[2] = TextureType::Size512;
	textureSizes[3] = TextureType::Size1024;
	textureSizes[4] = TextureType::Size2048;

	int index = 0;

	for (int i = 1; i <= MAX_TEXTURE_SIZE; i++) {
		textureSizeIndex[i] = index;

		if (i == textureSizes[index]) index++;
	}

	isInitialized = true;
}

PixelWorldEngine::TextureManager::~TextureManager()
{
	Utility::Delete(textureArray);
}

void PixelWorldEngine::TextureManager::RegisterTexture(int id, Graphics::Texture2D * texture)
{
	DebugReturn(DebugLayer::Assert(subTextures.count(id) != 0, Error::TheValueIsNotRight, "id", FunctionName));
	DebugReturn(DebugLayer::Assert(pixelFormat != texture->GetPixelFormat(), Error::ErrorPixelFormat, FunctionName));

	int size = Utility::Max(texture->GetWidth(), texture->GetHeight());

	DebugReturn(DebugLayer::Assert(size < 0 || size > MAX_TEXTURE_SIZE, Error::ErrorTextureWidthOrHeight, FunctionName));

	int index = textureSizeIndex[size];

	if (freeSubTextures[index].size() != 0) {

		CopySubTexture(id, freeSubTextures[index].front(), texture);

		freeSubTextures[index].pop();

		return;
	}

	if (textureCount == textureSpace) {
		
		auto oldTextureArray = textureArray;

		textureSpace += TEXTURE_ARRAY_ADD;

		textureArray = new Graphics::Texture2DArray(graphics, TEXTURE_WIDTH, TEXTURE_HEIGHT, textureSpace, pixelFormat);

		textureArray->CopyFromTexture2DArray(oldTextureArray, 0, 0, textureCount);

		Utility::Delete(oldTextureArray);
	}

	int subSize = textureSizes[index];

	int widthCount = textureArray->GetWidth() / subSize;
	int heightCount = textureArray->GetHeight() / subSize;

	textureCount++;

	for (int x = 0; x < widthCount; x++) {
		for (int y = 0; y < heightCount; y++) {
			SubTexture subTexture;

			subTexture.arrayIndex = textureCount;
			subTexture.startPositionX = x * subSize;
			subTexture.startPositionY = y * subSize;

			freeSubTextures[index].push(subTexture);
		}
	}

	CopySubTexture(id, freeSubTextures[index].front(), texture);

	freeSubTextures[index].pop();
}

void PixelWorldEngine::TextureManager::UnRegisterTexture(int id)
{
	DebugReturn(DebugLayer::Assert(subTextures.count(id) == 0, Error::TheIDIsNotExist, FunctionName));

	auto subTexture = subTextures[id];

	auto size = Utility::Max(subTexture.width, subTexture.height);
	auto index = textureSizeIndex[size];

	freeSubTextures[index].push(subTexture);

	subTextures.erase(id);
}

void PixelWorldEngine::TextureManager::Clear()
{
	textureCount = 0;
	
	subTextures.clear();

	for (int i = 0; i < (int)TextureType::Count; i++) 
		while (freeSubTextures[i].size() != 0) freeSubTextures[i].pop();
		
}

auto PixelWorldEngine::TextureManager::GetTexCoordTransform(int id) -> glm::mat4x4
{
	return subTextures[id].transform;
}

auto PixelWorldEngine::TextureManager::GetTextureArray() -> Graphics::Texture2DArray*
{
	return textureArray;
}

auto PixelWorldEngine::TextureManager::GetArrayIndex(int id) -> int
{
	return subTextures[id].arrayIndex;
}

auto PixelWorldEngine::TextureManager::IsExist(int id) -> bool
{
	if (subTextures.count(id) == 0) return false;
	return true;
}

void PixelWorldEngine::SubTexture::MakeTexCoordPosition(int maxWidth, int maxHeight)
{
	auto startPosition = glm::vec2((float)startPositionX / maxWidth, (float)startPositionY / maxHeight);
	auto endPosition = glm::vec2((float)(startPositionX + width - 1) / maxWidth, (float)(startPositionY + height - 1) / maxHeight);

	transform = glm::scale(glm::translate(glm::mat4(1), glm::vec3(startPosition, 0.0f)), glm::vec3(endPosition - startPosition, 1.0f));
}
