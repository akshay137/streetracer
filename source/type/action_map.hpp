#pragma once
#ifndef KATHA_TYPE_ACTION_MAP_H__
#define KATHA_TYPE_ACTION_MAP_H__ 1

#include "primitive.hpp"

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