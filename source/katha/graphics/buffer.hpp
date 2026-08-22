#pragma once
#ifndef KATHA_GRAPHICS_BUFFER_H__
#define KATHA_GRAPHICS_BUFFER_H__ 1

#include <cstdint>

namespace katha
{
	struct buffer_t
	{
		uint64_t id = 0;
		uint32_t size = 0;

		void log(const bool pretty_print = false) const;
	};
}

#endif