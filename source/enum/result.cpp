#include "result.hpp"
#include "../utility.hpp"

const char* katha::result_to_cstring(const result_e result)
{
	switch (result)
	{
		case result_e::success: return "result_e::success";
		case result_e::error: return "result_e::error";
		
		case result_e::error_sdl: return "result_e::error_sdl";
		case result_e::error_gl: return "result_e::error_gl";
		case result_e::error_vulkan: return "result_e::error_vulkan";
		case result_e::error_xr: return "result_e::error_xr";

		case result_e::error_value_null: return "result_e::error_value_null";
		case result_e::error_value_unexpected: return "result_e::error_value_unexpected";

		case result_e::request_skip: return "result_e::request_skip";
		case result_e::request_exit: return "result_e::request_exit";
	}

	return "unknown result";
}

bool katha::check_result(const result_e result, const char* message)
{
	if (result_e::success != result)
	{
		log_line("{s}: {s}", message, result_to_cstring(result));
		return false;
	}

	return true;
}