#pragma once
#ifndef KATHA_CORE_CONFIG_H__
#define KATHA_CORE_CONFIG_H__ 1

#include "enums.hpp"
#include "types.hpp"

namespace katha
{
	struct config_t
	{
		graphics_api_e graphics_api = graphics_api_e::gl;
		int32_t preferred_gpu_index = 0;
		int32_t preferred_display_index = 0;

		uint32_t enable_xr:1 = 0; // vr
		uint32_t debug_graphics_api:1 = 0;
		uint32_t force_es_context:1 = 0; // force OpenGL ES context
		uint32_t enable_vsync:1 = 0;
		uint32_t windowed:1 = 0;
		uint32_t log_frame_time:1 = 0;

		ivec2 window_size = {};

		void log() const;

		ivec2 get_default_window_size() const
		{
			return ivec2(1280, 720);
		}
	};
}

#endif