#pragma once
#ifndef GAME_HIGHWAY_H__
#define GAME_HIGHWAY_H__ 1

#include "action_map.hpp"
#include "../katha/core.hpp"

namespace katha
{
	struct highway_t
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

		vec3 player = {};

		int traffic_count = 32;
		traffic_t traffic[32] = {};
		float despawn_time[32] = {};
		int spawn_delay_seconds[32] = {};

		float game_time = 0;

		highway_t();

		void update(const action_map_t& action_map, const float delta);

		void spawn_traffic(const int index);

		static vec3 get_bb(const traffic_e traffic);
	};
}

#endif