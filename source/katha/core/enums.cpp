#include "enums.hpp"
#include "../core.hpp"

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

const char* katha::graphics_api_to_cstring(const katha::graphics_api_e api)
{
	switch (api)
	{
		case katha::graphics_api_e::vulkan: return "vulkan";
		case katha::graphics_api_e::gl: return "gl";
		default: return "unknown graphics api";
	}
}

const char* katha::format_to_cstring(const format_e format)
{
	switch (format)
	{
		case format_e::none: return "format_e::none";
		case format_e::rgba8: return "format_e::rgba8";
		case format_e::rgb8: return "format_e::rgb8";
		case format_e::depth24_stencil8: return "format_e::depth24_stencil8";
	}
	
	return "unknown format";
}