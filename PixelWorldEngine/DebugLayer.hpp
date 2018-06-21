#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	enum class Error {
		MoreThanOneInstance,
		TheDataIsNull,
		ShaderCompiledFailed,
		FormatIsNotSupported,
		Count
	};

	static const char* messageTemplate[(int)Error::Count] = {
		"There are more than one instance.",
		"The data is null.",
		"The shader was compiled failed. Code : %s",
		"The file (%s) format is not supported."
	};

	class DebugLayer {
	private:
		static auto GetErrorMessage(Error error, va_list args)->std::string;

		static void ReportMessage(std::string message);
	public:
		static void ReportError(Error error, ...);

		static void Assert(bool test, Error error, ...);

	};

}
