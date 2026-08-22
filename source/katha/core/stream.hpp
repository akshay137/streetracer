#pragma once
#ifndef KATHA_CORE_STREAM_H__
#define KATHA_CORE_STREAM_H__ 1

#include "types.hpp"

namespace katha
{
	// strictly used as a parameter for some functions
	struct stream_i
	{
		~stream_i() {};

		virtual uint32_t read(void* buffer, const uint32_t bytes) = 0;
		virtual uint32_t write(const void* buffer, const uint32_t bytes) = 0;

		template <typename T>
		bool read(T* out_value)
		{
			T value = {};
			uint32_t br = read(&value, sizeof(T));
			if (sizeof(T) == br)
			{
				write_checked(out_value, value);
				return true;
			}

			return false;
		}

		template <typename T>
		bool write(const T& value)
		{
			uint32_t bw = write(&value, sizeof(T));
			return sizeof(T) == bw;
		}
	};
}

#endif