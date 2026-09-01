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
}

#endif