#pragma once
#ifndef KATHA_ENUM_RESULT_H__
#define KATHA_ENUM_RESULT_H__ 1

namespace katha
{
	enum class Result : int
	{
		SUCCESS = 0,
		ERROR, // generic error

		ERROR_SDL,
		ERROR_GLES,
		ERROR_VULKAN,
		ERROR_XR,

		ERROR_VALUE_NULL,
		ERROR_VALUE_UNEXPECTED,

		REQUEST_SKIP,
		REQUEST_EXIT,
	};
	
	extern const char* ResultToCString(const Result result);
	extern bool CheckResult(const Result result, const char* message);
}

#endif