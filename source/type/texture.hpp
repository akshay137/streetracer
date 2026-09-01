#pragma once
#ifndef KATHA_GRAPHICS_TEXTURE_H__
#define KATHA_GRAPHICS_TEXTURE_H__ 1

#include "../enum/graphics.hpp"
#include "vector.hpp"

namespace katha
{
	struct texture_t
	{
		uint64_t handle = 0;

		uvec2 size = {};
		format_e format = format_e::none;

		void log(const char* prefix = nullptr) const;
	};
}

#endif