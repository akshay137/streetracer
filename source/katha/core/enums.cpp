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

const char* katha::base_to_cstring(const base_e base)
{
	switch (base)
	{
		case base_e::binary: return "base_e::binary";
		case base_e::octal: return "base_e::octal";
		case base_e::decimal: return "base_e::decimal";
		case base_e::hexadecimal: return "base_e::hexadecimal";
	}

	return "unknown base";
}

const char* katha::vertex_layout_to_cstring(const vertex_layout_e vertex)
{
	switch (vertex)
	{
		case vertex_layout_e::mesh: return "vertex_layout_e::mesh";
	}

	return "unknown vertex";
}

const char* katha::graphics_api_to_cstring(const katha::graphics_api_e api)
{
	switch (api)
	{
		case katha::graphics_api_e::vulkan: return "vulkan";
		case katha::graphics_api_e::gl: return "gl";
	}
	
	return "unknown graphics api";
}

const char* katha::render_mode_to_cstring(const render_mode_e mode)
{
	switch (mode)
	{
		case render_mode_e::mono: return "render_mode_e::mono";
		case render_mode_e::stereo: return "render_mode_e::stereo";
	}

	return "unknown render mode";
}

const char* katha::format_to_cstring(const format_e format)
{
	switch (format)
	{
		case format_e::none: return "format_e::none";
		case format_e::rgba8: return "format_e::rgba8";
		case format_e::rgb8: return "format_e::rgb8";
		case format_e::srgba8: return "format_e::srgba8";
		case format_e::srgb8: return "format_e::srgb8";
		case format_e::depth24_stencil8: return "format_e::depth24_stencil8";
	}
	
	return "unknown format";
}

bool katha::should_mipmap(const format_e format)
{
	switch (format)
	{
		case format_e::none:
		case format_e::depth24_stencil8:
			return false;
	}

	return true;
}

const char* katha::blend_mode_to_cstring(const blend_mode_e mode)
{
	switch (mode)
	{
		case blend_mode_e::none: return "blend_mode_e::none";
		case blend_mode_e::one_minus_src_alpha:
			return "blend_mode_e::one_minus_src_alpha";
	}

	return "unknown blend mode";
}

const char* katha::depth_mode_to_cstring(const depth_mode_e mode)
{
	switch (mode)
	{
		case depth_mode_e::none: return "depth_mode_e::none";
		case depth_mode_e::less: return "depth_mode_e::less";
		case depth_mode_e::greater: return "depth_mode_e::greater";
	}

	return "unknwon depth mode";
}