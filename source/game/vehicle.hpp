#pragma once
#ifndef GAME_VEHICLE_H__
#define GAME_VEHICLE_H__ 1

#include "../type/transform.hpp"

namespace katha
{
	struct vehicle_t
	{
		transform_t transform = {};
		
		void apply(
			const float throttle,
			const float brake,
			const float steering_angle,
			const float delta
		);
	};
}

#endif