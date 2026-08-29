#include "graphics.hpp"

const char* katha::graphics_api_to_cstring(const katha::graphics_api_e api)
{
	switch (api)
	{
		case katha::graphics_api_e::vulkan: return "vulkan";
		case katha::graphics_api_e::gl: return "gl";
	}
	
	return "unknown graphics api";
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

const char* katha::buffer_usage_to_cstring(const buffer_usage_e usage)
{
	switch (usage)
	{
		case buffer_usage_e::data: return "buffer_usage_e::data";
		case buffer_usage_e::index: return "buffer_usage_e::index";
	}
	return "unknown buffer usage";
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

const char* katha::render_mode_to_cstring(const render_mode_e mode)
{
	switch (mode)
	{
		case render_mode_e::mono: return "render_mode_e::mono";
		case render_mode_e::stereo: return "render_mode_e::stereo";
	}

	return "unknown render mode";
}

const char* katha::vertex_layout_to_cstring(const vertex_layout_e vertex)
{
	switch (vertex)
	{
		case vertex_layout_e::mesh: return "vertex_layout_e::mesh";
	}

	return "unknown vertex";
}