#pragma once
#ifndef KATHA_CORE_ENUMS_H__
#define KATHA_CORE_ENUMS_H__ 1

namespace katha
{
	enum class result_e : int
	{
		success = 0,
		error,

		error_sdl,
		error_gl,
		error_vulkan,
		error_xr,

		request_skip,
		request_exit,
	};

	extern const char* result_to_cstring(const result_e result);
	extern bool check_result(const result_e result, const char* message);

	enum class graphics_api_e : int
	{
		gl,
		vulkan,
	};

	enum class format_e : int
	{
		none = 0, // to specify no texture in some functions
		rgba8,
		rgb8,
		depth24_stencil8,
	};

	extern const char* format_to_cstring(const format_e format);
}

#endif