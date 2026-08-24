#pragma once
#ifndef GAME_VEHICLE_H__
#define GAME_VEHICLE_H__ 1

#include "../katha/core/types.hpp"
#include "../katha/physics/transform.hpp"

namespace katha
{
	struct vehicle_t
	{
		transform_t transform = {};
		float steering_angle = 0;
	};
}

#endif