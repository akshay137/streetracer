#pragma once
#ifndef KATHA_TYPE_PRIMITIVE_H__
#define KATHA_TYPE_PRIMITIVE_H__ 1

#include <cstdint>
#include <cstddef>

#include <source_location>

namespace katha
{
	using source_t = std::source_location;

	union primitive_u
	{
		int64_t i;
		uint64_t u;
		float f;
		void* p;
	};
}

#endif