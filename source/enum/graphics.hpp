#pragma once
#ifndef KATHA_ENUM_GRAPHICS_API_H__
#define KATHA_ENUM_GRAPHICS_API_H__ 1

namespace katha
{
	enum class graphics_api_e : int
	{
		gl,
		vulkan,
	};
	extern const char* graphics_api_to_cstring(const graphics_api_e api);

	enum class blend_mode_e : int
	{
		none = 0, // disabled blending
		one_minus_src_alpha,
	};

	extern const char* blend_mode_to_cstring(const blend_mode_e mode);

	enum class buffer_usage_e : unsigned int
	{
		data = 1 << 0,
		index = 1 << 1,
	};
	extern const char* buffer_usage_to_cstring(const buffer_usage_e usage);

	enum class depth_mode_e : int
	{
		none = 0, // disabled depth testing
		less,
		greater,
	};
	extern const char* depth_mode_to_cstring(const depth_mode_e mode);

	enum class render_mode_e : int
	{
		mono = 1,
		stereo = 2,
	};
	extern const char* render_mode_to_cstring(const render_mode_e mode);

	enum class vertex_layout_e : int
	{
		mesh, // vertex_t { vec3, vector2<uint16> }
	};

	extern const char* vertex_layout_to_cstring(const vertex_layout_e vertex_layout);
}

#endif