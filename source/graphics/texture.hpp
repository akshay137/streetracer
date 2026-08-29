#pragma once
#ifndef KATHA_GRAPHICS_TEXTURE_H__
#define KATHA_GRAPHICS_TEXTURE_H__ 1

#include "../type/vector.hpp"
#include "../enum/format.hpp"

namespace katha
{
	// 2D texture
	struct texture_t
	{
		uint64_t id = 0;
		uint64_t view = 0;

		uvec2 size = {};

		format_e format = format_e::none;
	};
}

#endif