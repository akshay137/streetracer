#pragma once
#ifndef KATHA_GRAPHICS_TEXTURE_H__
#define KATHA_GRAPHICS_TEXTURE_H__ 1

#include "../enum/graphics.hpp"
#include "vector.hpp"

namespace katha
{
	struct Texture
	{
		uint64_t handle = 0;

		uvec2 size = {};
		Format format = Format::NONE;
	};
}

#endif