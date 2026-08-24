#pragma once
#ifndef GAME_ACTION_MAP_H__
#define GAME_ACTION_MAP_H__ 1

#include "../katha/core/types.hpp"

namespace katha
{
	struct action_map_t
	{
		float steering_angle = 0.0f;
		float throttle = 0.0f;
		float brake = 0.0f;
	};
}

#endif