#include "MergeTexture.hpp"

#include "Application.hpp"

PixelWorldEngine::MergeTexture::MergeTexture(Application * application, int width, int height, Graphics::PixelFormat pixelFormat)
{
	graphics = application->GetGraphics();

	finalTexture = new Graphics::Texture2D(graphics, nullptr, width, height,
		pixelFormat);
}

PixelWorldEngine::MergeTexture::~MergeTexture()
{
	Utility::Delete(finalTexture);
}

void PixelWorldEngine::MergeTexture::AddTexture(int id, int positionX, int positionY, Graphics::Texture2D * texture)
{
	DebugReturn(DebugLayer::Assert(finalTexture->GetPixelFormat() != texture->GetPixelFormat(), Error::ErrorPixelFormat, FunctionName));

	SubTexture subTexture;

	subTexture.startPositionX = positionX;
	subTexture.startPositionY = positionY;

	subTexture.width = texture->GetWidth();
	subTexture.height = texture->GetHeight();

	subTexture.MakeTexCoordPosition(finalTexture->GetWidth(), finalTexture->GetHeight());

	subTexture.transform = glm::scale(glm::translate(glm::mat4(1), glm::vec3(subTexture.startPosition, 0.0f)),
		glm::vec3(subTexture.endPosition - subTexture.startPosition, 1.0f));

#ifdef _DEBUG
	for (auto it = subTextures.begin(); it != subTextures.end(); it++) {
		if (it->first == id) continue;

		if (SubTexture::IsEnter(subTexture, it->second) == true) return;
	}
#endif // _DEBUG

	if (subTextures.count(id) != 0)
		RemoveTexture(id);

	subTextures.insert(std::pair<int, SubTexture>(id, subTexture));

	finalTexture->CopyFromTexture2D(texture, subTexture.startPositionX, subTexture.startPositionY,
		PixelWorldEngine::Rectangle(0, 0, subTexture.width, subTexture.height));
}

void PixelWorldEngine::MergeTexture::RemoveTexture(int id)
{
	DebugReturn(DebugLayer::Assert(subTextures.count(id) == 0, Error::TheIDIsNotExist, Utility::ToString(id), FunctionName));

	subTextures.erase(id);
}

auto PixelWorldEngine::MergeTexture::GetTexCoordTransform(int id) -> glm::mat4x4
{
	return subTextures[id].transform;
}

auto PixelWorldEngine::MergeTexture::GetFinalTexture() -> Graphics::Texture2D *
{
	return finalTexture;
}

auto PixelWorldEngine::MergeTexture::GetPixelFormat() -> Graphics::PixelFormat
{
	return finalTexture->GetPixelFormat();
}

auto PixelWorldEngine::MergeTexture::IsExistID(int id) -> bool
{
	if (subTextures.count(id) == 0) return false;
	return true;
}

void PixelWorldEngine::SubTexture::MakeTexCoordPosition(int maxWidth, int maxHeight)
{
	startPosition = glm::vec2((float)startPositionX / maxWidth, (float)startPositionY / maxHeight);
	endPosition = glm::vec2((float)(startPositionX + width - 1) / maxWidth, (float)(startPositionY + height - 1) / maxHeight);
}

bool PixelWorldEngine::SubTexture::IsEnter(SubTexture texture1, SubTexture texture2)
{
	if (texture1.startPositionX + texture1.width - 1 < texture2.startPositionX) return false;
	if (texture1.startPositionX > texture2.startPositionX + texture2.width - 1) return false;

	if (texture1.startPositionY + texture1.height - 1 < texture2.startPositionY) return false;
	if (texture1.startPositionY > texture2.startPositionY + texture2.height - 1) return false;

	return true;
}
