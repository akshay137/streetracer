#include "config.hpp"
#include "../core.hpp"

const char* graphics_api_str(const katha::graphics_api_e api)
{
	switch (api)
	{
		case katha::graphics_api_e::vulkan: return "vulkan";
		case katha::graphics_api_e::gl: return "gl";
		default: return "unknown graphics api";
	}
}

void katha::config_t::log() const
{
	log_line("graphics_api: {s}", graphics_api_str(graphics_api));
	log_line("preferred gpu: {i}", preferred_gpu_index);
	log_line("preferred display: {i}", preferred_display_index);
	
	log_line("enable_xr: {u}", enable_xr);
	log_line("debug_graphics_api: {u}", debug_graphics_api);
	log_line("vsync: {u}", enable_vsync);
	log_line("windowed: {u}", windowed);
	log_line("log_frame_time: {u}", log_frame_time);

	log_line("window_size: {iv2}", window_size.array());
}