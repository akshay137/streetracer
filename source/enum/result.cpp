#include "result.hpp"
#include "../utility.hpp"

const char* katha::ResultToCString(const Result result)
{
	switch (result)
	{
		case Result::SUCCESS: return "Result::SUCCESS";
		case Result::ERROR: return "Result::ERROR";
		
		case Result::ERROR_SDL: return "Result::ERROR_SDL";
		case Result::ERROR_GLES: return "Result::ERROR_GLES";
		case Result::ERROR_VULKAN: return "Result::ERROR_VULKAN";
		case Result::ERROR_XR: return "Result::ERROR_XR";

		case Result::ERROR_VALUE_NULL: return "Result::ERROR_VALUE_NULL";
		case Result::ERROR_VALUE_UNEXPECTED: return "Result::ERROR_VALUE_UNEXPECTED";

		case Result::REQUEST_SKIP: return "Result::REQUEST_SKIP";
		case Result::REQUEST_EXIT: return "Result::REQUEST_EXIT";
	}

	return "Unknown Result";
}

bool katha::CheckResult(const Result result, const char* message)
{
	if (Result::SUCCESS == result) {
		return true;
	}
	
	LogLine("{s}: {s}", message, ResultToCString(result));
	return false;
}