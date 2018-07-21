#pragma once

#include "pch.hpp"

namespace PixelWorldEngine {

#ifdef _MSC_VER
#define FunctionName __FUNCSIG__
#endif // _MSC_VER

#ifdef _DEBUG

#define DebugReturn(x) if (x == true) return 
#define DebugReturnWithValue(x, value) if (x == true) return value
#define DebugThrow(x) if (x == true) throw "Error"

#else

#define DebugReturn(x)
#define DebugReturnWithValue(x, value)
#define DebugThrow(x)

#endif
	enum class Error {
		//There are more than one instance.
		MoreThanOneInstance,
		//The data is null.
		TheDataIsNull,
		//The shader was compiled failed. 
		ShaderCompiledFailed,
		//The file format is not supported.
		FormatIsNotSupported,
		//The animation need two frames at least.
		NeedMoreThanOneFrame,
		//We can not add the animation into animator when the animator is running.
		CanNotAddAnimationWhenAnimatorRun,
		//We do not have this child object.
		NoChildObject,
		//The texture's width or height is not matched.
		ErrorTextureWidthOrHeight,
		//The Pixel Format is not matched.
		ErrorPixelFormat,
		//The character code is not defined.
		UndefinedCharacter,
		//The width or height is less than 0.
		WidthOrHeightLessThanZero,
		//The object is null
		TheObjectIsNull,
		//The name of object is not exist.
		TheNameIsNotExist,
		//The ID is not exist.
		TheIDIsNotExist,
		//The value is not right.
		TheValueIsNotRight,
		//Allocate memory error.
		MemoryAllocateError,
		Count
	};

	static const char* messageTemplate[(int)Error::Count] = {
		"There are more than one instance.",
		"The data is null.",
		"The shader was compiled failed.",
		"The file format is not supported.",
		"The animation need two frames at least.",
		"We can not add the animation into animator when the animator is running.",
		"We do not have this child object.",
		"The texture's width or height is not matched.",
		"The Pixel Format is not matched.",
		"The character code is not defined.",
		"The width or height is less than 0.",
		"The object is null.",
		"The name of object is not exist.",
		"The ID is not exist.",
		"The value is not right.",
		"Allocate memory error."
	};

	class DebugLayer {
	private:
		static void ReportMessage(std::string message);
	public:
		static void ReportError(Error error, std::string exMessage);

		static bool Assert(bool test, Error error, std::string exMessage);

		static bool Assert(bool test, Error error, std::string target, std::string type);

	};

}
