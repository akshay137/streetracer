#pragma once
#ifndef KATHA_GRAPHICS_BUFFER_H__
#define KATHA_GRAPHICS_BUFFER_H__ 1

#include "primitive.hpp"

namespace katha
{
	struct buffer_t
	{
		uint64_t handle = 0;

		uint32_t size = 0;
	};
}

#endif