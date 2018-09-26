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

auto PixelWorldEngine::Utility::ToString(int number) -> std::string
{
	std::string result = u8"";

	while (number > 0) {
		char c = (number % 10) + '0';
		result = c + result;

		number /= 10;
	}

	return result;
}

auto PixelWorldEngine::Utility::ConvertPosition(Camera * camera, glm::vec2 position) -> glm::vec2
{
	auto rect = camera->GetRectangle();

	auto result = glm::vec2();

	result.x = position.x * (rect.right - rect.left) + rect.left;
	result.y = position.y * (rect.bottom - rect.top) + rect.top;

	return result;
}
