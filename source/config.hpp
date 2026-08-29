#pragma once
#ifndef KATHA_CONFIG_H__
#define KATHA_CONFIG_H__ 1

#include "enum/features.hpp"
#include "enum/graphics.hpp"
#include "type/flags.hpp"
#include "type/vector.hpp"

namespace katha
{
	struct config_t
	{
		graphics_api_e graphics_api = graphics_api_e::gl;
		int32_t preferred_gpu_index = 0;
		int32_t preferred_display_index = 0;
		
		eflags_t<feature_e, uint64_t> features = {};

		ivec2 window_size = {};

		bool enabled(const feature_e feature) const
		{
			return features.has(feature);
		}

		void log() const;

		ivec2 get_default_window_size() const
		{
			return ivec2(1280, 720);
		}

		bool parse_command_line(int argc, char** args);
	};
}

#endif