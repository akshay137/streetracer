#pragma once
#ifndef KATHA_CORE_ENUMS_H__
#define KATHA_CORE_ENUMS_H__ 1

#include <cstdint>

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

	enum class base_e : uint8_t
	{
		binary = 2,
		octal = 8,
		decimal = 10,
		hexadecimal = 16
	};
	extern const char* base_to_cstring(const base_e base);

	enum class vertex_layout_e : uint8_t
	{
		f3_usn2, // vertex_t { vec3, vector2<uint16_t> }
	};
	extern const char* vertex_layout_to_cstring(const vertex_layout_e vertex_layout);

	enum class graphics_api_e : uint8_t
	{
		gl,
		vulkan,
	};
	extern const char* graphics_api_to_cstring(const graphics_api_e api);

	enum class render_mode_e : uint8_t
	{
		mono = 1,
		stereo = 2,
	};
	extern const char* render_mode_to_cstring(const render_mode_e mode);

	enum class format_e : uint8_t
	{
		none = 0, // to specify no texture in some functions
		rgba8,
		rgb8,
		srgba8,
		srgb8,
		depth24_stencil8,
	};
	extern const char* format_to_cstring(const format_e format);
	extern bool should_mipmap(const format_e format);

	enum class blend_mode_e : uint8_t
	{
		none = 0, // disabled blending
		one_minus_src_alpha,
	};
	extern const char* blend_mode_to_cstring(const blend_mode_e mode);

	enum class depth_mode_e : uint8_t
	{
		none = 0, // disabled depth testing
		less,
		greater,
	};
	extern const char* depth_mode_to_cstring(const depth_mode_e mode);
}

#endif