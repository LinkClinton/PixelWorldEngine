#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

	enum class Error {
		//There are more than one instance.
		MoreThanOneInstance,
		//The data is null.
		TheDataIsNull,
		//The shader was compiled failed. Code : %s
		ShaderCompiledFailed,
		//The file (%s) format is not supported.
		FormatIsNotSupported,
		//The animation (%s) need two frames at least.
		NeedMoreThanOneFrame,
		//We can not add the animation (%s) into animator (%s) when the animator is running.
		CanNotAddAnimationWhenAnimatorRun,
		//We do not have this child object called %s in %s 
		NoChildObject,
		//The texture's width or height is not matched.
		ErrorTextureWidthOrHeight,
		//The Pixel Format is not matched.
		ErrorPixelFormat,
		Count
	};

	static const char* messageTemplate[(int)Error::Count] = {
		"There are more than one instance.",
		"The data is null.",
		"The shader was compiled failed. Code : %s",
		"The file (%s) format is not supported.",
		"The animation (%s) need two frames at least.",
		"We can not add the animation (%s) into animator (%s) when the animator is running.",
		"We do not have this child object called %s in %s.",
		"The texture's width or height is not matched.",
		"The Pixel Format is not matched."
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
