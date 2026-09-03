#pragma once
#ifndef KATHA_PLATFORM_INPUT_H__
#define KATHA_PLATFORM_INPUT_H__ 1

#include "../type/bitfield.hpp"
#include "../type/gamepad.hpp"

#include <SDL2/SDL_scancode.h>

namespace katha
{
	struct Input
	{
		BitField<512, uint64_t> keyboard;
		ivec2 cursor = {};
		GamePad gamepad = {};
		BitField<3> mouse;

		bool getKey(const int key) const
		{
			return keyboard.has(key);
		}

		void setKey(const int key, bool pressed)
		{
			if (pressed)
			{
				keyboard.set(key);
			}
			else
			{
				keyboard.unset(key);
			}
		}
	};
}

#endif