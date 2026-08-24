#include "config.hpp"
#include "../core.hpp"

void katha::config_t::log() const
{
	log_line("graphics_api: {s}", graphics_api_to_cstring(graphics_api));
	log_line("preferred gpu: {i}", preferred_gpu_index);
	log_line("preferred display: {i}", preferred_display_index);
	
	log_line("enable_xr: {u}", enable_xr);
	log_line("debug_graphics_api: {u}", debug_graphics_api);
	log_line("force_es_context: {u}", force_es_context);
	log_line("vsync: {u}", enable_vsync);
	log_line("windowed: {u}", windowed);
	log_line("log_frame_time: {u}", log_frame_time);

	log_line("window_size: {iv2}", window_size.array());
}