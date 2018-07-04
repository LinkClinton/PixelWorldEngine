#pragma once

#include "pch.hpp"

#include "GraphicsEnum.hpp"

namespace PixelWorldEngine {

	class Utility {
	public:
		static auto CharArrayToString(char str[]) -> std::string;

		static auto WCharArrrayToWString(wchar_t str[]) -> std::wstring;

		static auto CharArrayToVector(char str[]) -> std::vector<byte>;

		static auto CountPixelFormatSize(Graphics::PixelFormat pixelFormat) -> int;

		template<typename T>
		static auto Max(T x, T y) -> T;

		template<typename T>
		static auto Min(T x, T y) -> T;

		template<typename T>
		static auto Limit(T value, T lower, T upeer) -> T;

		template<typename T>
		static bool IsLimit(T value, T lower, T upeer);

		template<typename T>
		static void Dispose(T &object);

		template<typename T>
		static void Delete(T &object);
	};

	template<typename T>
	inline auto Utility::Max(T x, T y) -> T
	{
		if (x > y) return x;
		return y;
	}

	template<typename T>
	inline auto Utility::Min(T x, T y) -> T
	{
		if (x < y) return x;
		return y;
	}

	template<typename T>
	inline auto Utility::Limit(T value, T lower, T upeer) -> T
	{
		return Max(lower, Min(value, upeer));
	}

	template<typename T>
	inline bool Utility::IsLimit(T value, T lower, T upeer)
	{
		if (value >= lower && value <= upeer) return true;
		return false;
	}

	template<typename T>
	inline void Utility::Dispose(T & object)
	{
		if (object != nullptr)
			object->Release();
		object = nullptr;
	}

	template<typename T>
	inline void Utility::Delete(T & object)
	{
		if (object != nullptr)
			delete object;
		object = nullptr;
	}

}
