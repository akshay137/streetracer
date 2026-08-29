#pragma once
#ifndef KATHA_UTILITY_H__
#define KATHA_UTILITY_H__ 1

#include "type/primitive.hpp"
#include "type/string.hpp"

namespace katha
{
	template <typename T>
	static bool write_checked(T* ptr, const T& value)
	{
		if (nullptr == ptr)
		{
			return false;
		}

		*ptr = value;
		return true;
	}

	template <typename T>
	static bool read_checked(const T* ptr, T* out_value)
	{
		if (nullptr == ptr)
		{
			return false;
		}

		return write_checked<T>(out_value, *ptr);
	}

	extern uint32_t log_line(const string_t format, ...);

	// exits on failure
	extern void* allocate(const uint32_t bytes, const source_t& source = source_t::current());
	extern void release(void* memory, const source_t& source = source_t::current());

	// exits on failure
	template <typename T>
	T* alloc(const uint32_t count, const source_t& source = source_t::current())
	{
		void* memory = allocate(count * sizeof(T), source);
		return reinterpret_cast<T*>(memory);
	}
}

#endif