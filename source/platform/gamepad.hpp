#pragma once
#ifndef KATHA_PLATFORM_GAMEPAD_H__
#define KATHA_PLATFORM_GAMEPAD_H__ 1

#include "../katha/core/types.hpp"

namespace katha
{
	struct gamepad_t
	{
		vec2 stick_left = {};
		vec2 stick_right = {};
		float trigger_left = 0;
		float trigger_right = 0;

		uint16_t dpad_up:1 = 0;
		uint16_t dpad_left:1 = 0;
		uint16_t dpad_down:1 = 0;
		uint16_t dpad_right:1 = 0;

		uint16_t action_up:1 = 0;
		uint16_t action_left:1 = 0;
		uint16_t action_down:1 = 0;
		uint16_t action_right:1 = 0;

		uint16_t shoulder_left:1 = 0;
		uint16_t shoulder_right:1 = 0;

		uint16_t l3:1 = 0;
		uint16_t r3:1 = 0;

		uint16_t select:1 = 0;
		uint16_t start:1 = 0;

		void clamp();
		void merge(const gamepad_t& gp);
	};
}

#endif