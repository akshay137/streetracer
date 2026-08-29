#pragma once
#ifndef KATHA_GRAPHICS_FRAMEBUFFER_H__
#define KATHA_GRAPHICS_FRAMEBUFFER_H__ 1

#include "texture.hpp"

namespace katha
{
	struct framebuffer_t
	{
		uint64_t id = 0;
		uvec2 size = {};

		texture_t color_0 = {};
		texture_t depth_stencil = {};

		framebuffer_t() = default;
	};
}

#endif