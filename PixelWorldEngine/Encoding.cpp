#include "Encoding.hpp"

auto PixelWorldEngine::Encoding::ToUTF16String(std::string str) -> std::u16string
{
	std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> convert;
	
	auto temp = convert.from_bytes(str);

	std::u16string result;

	for (auto it = temp.begin(); it != temp.end(); it++)
		result.push_back(*it);

	return result;
}
