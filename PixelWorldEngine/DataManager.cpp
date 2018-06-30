#include "DataManager.hpp"

#include "Application.hpp"

#include <SDL2\SDL_image.h>

#ifdef _WIN32
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_Image.lib")
#endif // _WIN32


PixelWorldEngine::FileData::FileData(byte * Data)
{
	if (Data == nullptr) return;
	
	auto length = strlen((char*)Data);

	data.resize(length);

	for (size_t i = 0; i < length; i++)
		data[i] = Data[i];
}

auto PixelWorldEngine::FileData::GetSize() -> size_t
{
	return data.size();
}

auto PixelWorldEngine::FileData::GetData() -> byte *
{
	return &data[0];
}

auto PixelWorldEngine::FileData::GetVector() -> std::vector<byte>
{
	return std::vector<byte>(data);
}

PixelWorldEngine::DataManager::DataManager(Application* application)
{
	graphics = application->GetGraphics();

	IMG_Init(IMG_InitFlags::IMG_INIT_JPG | IMG_InitFlags::IMG_INIT_PNG);
}

PixelWorldEngine::DataManager::~DataManager()
{
	IMG_Quit();
}

auto PixelWorldEngine::DataManager::ReadFile(std::string fileName) -> FileData
{
	FileData result;

	std::ifstream file;

	file.open(fileName, std::ios::binary | std::ios::ate);

	size_t size = (size_t)file.tellg();

	result.data.resize(size);

	file.seekg(0, std::ios::beg);
	file.read((char*)&result.data[0], size);
	file.close();

	return result;

}

auto PixelWorldEngine::DataManager::RegisterTexture(std::string fileName) -> Graphics::Texture2D *
{
	if (textures[fileName] != nullptr) return textures[fileName];

	auto file = ReadFile(fileName);
	auto SDL_Rw = SDL_RWFromMem(file.GetData(), file.GetSize());

	SDL_Surface* SDL_image = nullptr;

	if ((bool)IMG_isBMP(SDL_Rw) == true) 
		SDL_image = IMG_LoadBMP_RW(SDL_Rw);

	if ((bool)IMG_isPNG(SDL_Rw) == true)
		SDL_image = IMG_LoadPNG_RW(SDL_Rw);

	if ((bool)IMG_isJPG(SDL_Rw) == true)
		SDL_image = IMG_LoadJPG_RW(SDL_Rw);

	DebugLayer::Assert(SDL_image == nullptr, Error::FormatIsNotSupported, fileName);

	SDL_Surface* SDL_resultImage = nullptr;

	bool needFree = false;

	if (SDL_image->format->format != SDL_PIXELFORMAT_ABGR8888) {
		SDL_resultImage = SDL_ConvertSurfaceFormat(SDL_image, SDL_PIXELFORMAT_ABGR8888, 0);
		needFree = true;
	}
	else SDL_resultImage = SDL_image;

	auto texture = new Graphics::Texture2D(graphics, SDL_resultImage->pixels, SDL_resultImage->w, SDL_resultImage->h,
		Graphics::PixelFormat::R8G8B8A8);
	
	textures[fileName] = texture;

	SDL_FreeSurface(SDL_image);
	SDL_FreeRW(SDL_Rw);

	if (needFree == true)
		SDL_FreeSurface(SDL_resultImage);

	return texture;
}

auto PixelWorldEngine::DataManager::UnRegisterTexture(std::string fileName)
{
	if (textures[fileName] == nullptr) return;

	Utility::Delete(textures[fileName]);
}
