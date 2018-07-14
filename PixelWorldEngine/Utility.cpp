#include "Utility.hpp"

auto PixelWorldEngine::Utility::CharArrayToString(char str[]) -> std::string
{
	if (str == nullptr) return std::string();

	auto length = strlen(str);

	std::string result;
	
	for (size_t i = 0; i < length; i++) 
		result.push_back(str[i]);

	return result;
}

auto PixelWorldEngine::Utility::WCharArrrayToWString(wchar_t str[]) -> std::wstring
{
	if (str == nullptr) return std::wstring();

	auto length = wcslen(str);

	std::wstring result;

	for (size_t i = 0; i < length; i++)
		result.push_back(str[i]);

	return result;
}

auto PixelWorldEngine::Utility::CharArrayToVector(char str[]) -> std::vector<byte>
{
	if (str == nullptr) return  std::vector<byte>();

	auto length = strlen(str);

	std::vector<byte> vector(length);

	for (size_t i = 0; i < length; i++)
		vector[i] = str[i];

	return vector;
}

auto PixelWorldEngine::Utility::CountPixelFormatSize(Graphics::PixelFormat pixelFormat) -> int
{
	switch (pixelFormat)
	{
	case PixelWorldEngine::Graphics::PixelFormat::Unknown:
		return 0;
	case PixelWorldEngine::Graphics::PixelFormat::R32G32B32A32_Float:
		return 16;
	case PixelWorldEngine::Graphics::PixelFormat::R8G8B8A8:
		return 4;
	case PixelWorldEngine::Graphics::PixelFormat::A8:
		return 1;
	default:
		return 0;
	}
}
