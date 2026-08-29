#pragma once
#ifndef KATHA_ENUM_RESULT_H__
#define KATHA_ENUM_RESULT_H__ 1

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

		error_value_null,
		error_value_unexpected,

		request_skip,
		request_exit,
	};
	
	extern const char* result_to_cstring(const result_e result);
	extern bool check_result(const result_e result, const char* message);
}

#endif