#pragma once
#ifndef GAME_HIGHWAY_H__
#define GAME_HIGHWAY_H__ 1

#include "action_map.hpp"
#include "../katha/core.hpp"
#include "../katha/physics/transform.hpp"

// TODO: add model/mesh support
#include "../katha/graphics/buffer.hpp"
#include "../katha/graphics/texture.hpp"

namespace katha
{
	struct graphics_i;
	
	struct world_t
	{
		enum class traffic_e : int
		{
			none,
			car,
			truck,
			bike
		};

		struct traffic_t
		{
			vec3 position = {};
			traffic_e type = {};
		};

		transform_t camera = {};

		texture_t checker_board_texture = {};
		buffer_t vertex_buffer = {};

		vec3 player = {};

		int traffic_count = 32;
		traffic_t traffic[32] = {};
		float despawn_time[32] = {};
		int spawn_delay_seconds[32] = {};

		float game_time = 0;

		world_t();

		result_e load(graphics_i* gfx);
		void clear(graphics_i* gfx);

		void update(const action_map_t& action_map, const float delta);

		void spawn_traffic(const int index);

		static vec3 get_bb(const traffic_e traffic);
	};
}

#endif