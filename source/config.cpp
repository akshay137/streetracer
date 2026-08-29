#include "config.hpp"
#include "utility.hpp"

#include <cstdlib>

void katha::config_t::log() const
{
	log_line("graphics_api: {s}", graphics_api_to_cstring(graphics_api));
	log_line("preferred gpu: {i}", preferred_gpu_index);
	log_line("preferred display: {i}", preferred_display_index);
	
	log_line("vr: {b}", features.has(feature_e::vr));
	log_line("vsync: {b}", features.has(feature_e::vsync));
	log_line("window_mode: {b}", features.has(feature_e::window_mode));
	
	log_line("debug_graphics: {b}", features.has(feature_e::debug_graphics));
	
	log_line("force_es_context: {b}", features.has(feature_e::force_opengl_es));
	log_line("log_frame_time: {b}", features.has(feature_e::log_frame_time));

	log_line("window_size: {iv2}", window_size.array());
}

bool katha::config_t::parse_command_line(int argc, char** args)
{
	window_size = get_default_window_size();

	for (int i = 1; i < argc; i++)
	{
		const bool has_next_argument = (i + 1) < argc;
		const string_t arg(args[i]);

		if (arg.equals("--gl"))
		{
			graphics_api = graphics_api_e::gl;
		}
		else if (arg.equals("--vulkan"))
		{
			graphics_api = graphics_api_e::vulkan;
		}

		if (arg.equals("-display") && has_next_argument)
		{
			int32_t display = atoi(args[i + 1]);
			preferred_display_index = display;
		}
		if (arg.equals("-gpu") && has_next_argument)
		{
			int32_t gpu = atoi(args[i + i]);
			preferred_gpu_index = gpu;
		}

#if KATHA_XR
		if (arg.equals("--vr"))
		{
			features.set(feature_e::vr);
		}
#endif
		if (arg.equals("--vsync"))
		{
			features.set(feature_e::vsync);
		}
		if (arg.equals("--window_mode"))
		{
			features.set(feature_e::window_mode);
		}

		if (arg.equals("--debug_graphics_api"))
		{
			features.set(feature_e::debug_graphics);
		}

		if (arg.equals("--force_es_context"))
		{
			features.set(feature_e::force_opengl_es);
		}
		if (arg.equals("--log_frame_time"))
		{
			features.set(feature_e::log_frame_time);
		}
	}

	return true;
}