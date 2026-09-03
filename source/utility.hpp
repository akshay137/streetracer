#pragma once
#ifndef KATHA_UTILITY_H__
#define KATHA_UTILITY_H__ 1

#include "type/primitive.hpp"
#include "type/string.hpp"

namespace katha
{
	extern uint32_t LogLine(const String format, ...);

	// exits on failure
	extern void* Allocate(const uint32_t bytes, const source_t& source = source_t::current());
	extern void Release(void* memory, const source_t& source = source_t::current());

	// exits on failure
	template <typename T, bool default_init = true>
	T* Alloc(
		const uint32_t count = 1,
		const source_t& source = source_t::current()
	)
	{
		void* memory = Allocate(count * sizeof(T), source);
		T* data = reinterpret_cast<T*>(memory);
		T temp = {};
		if constexpr (default_init)
		{
			for (uint32_t i = 0; i < count; i++)
			{
				data[i] = temp;
			}
		}
		return data;
	}
}

#endif