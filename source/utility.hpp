#pragma once
#ifndef KATHA_UTILITY_H__
#define KATHA_UTILITY_H__ 1

#include "type/primitive.hpp"
#include "type/string.hpp"

namespace katha
{
	extern uint32_t log_line(const string_t format, ...);

	// exits on failure
	extern void* allocate(const uint32_t bytes, const source_t& source = source_t::current());
	extern void release(void* memory, const source_t& source = source_t::current());

	// exits on failure
	template <typename T, bool default_init = true>
	T* alloc(
		const uint32_t count = 1,
		const source_t& source = source_t::current()
	)
	{
		void* memory = allocate(count * sizeof(T), source);
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