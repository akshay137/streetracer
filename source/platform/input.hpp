#pragma once
#ifndef KATHA_PLATFORM_INPUT_H__
#define KATHA_PLATFORM_INPUT_H__ 1

#include "../type/gamepad.hpp"

#include <SDL2/SDL_scancode.h>

namespace katha
{
	struct input_t
	{
		uint64_t keyboard[64] = {};
		ivec2 cursor = {};
		gamepad_t gamepad = {};
		uint8_t mouse = 0;

		bool get_key(const int key) const
		{
			const int index = key / sizeof(uint64_t);
			const int bit = key % sizeof(uint64_t);

			const uint64_t _key = keyboard[index];
			const bool pressed = _key & (1 << bit);
			return pressed;
		}

		void set_key(const int key, bool pressed)
		{
			const int index = key / sizeof(uint64_t);
			const int bit = key % sizeof(uint64_t);

			if (pressed)
			{
				keyboard[index] |= (1 << bit);
			}
			else
			{
				keyboard[index] &= ~(1 << bit);
			}
		}
	};
}

#endif