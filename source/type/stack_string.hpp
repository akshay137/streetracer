#pragma once
#ifndef KATHA_CORE_STACK_STRING_H__
#define KATHA_CORE_STACK_STRING_H__ 1

#include "primitive.hpp"
#include "string.hpp"
#include "../math/utility.hpp"

namespace katha
{
	template <uint16_t CAPACITY>
	struct stack_string_t
	{
		uint32_t size = 0;
		char buffer[CAPACITY] = {};

		constexpr uint16_t capacity() const
		{
			return CAPACITY;
		}

		bool is_full() const
		{
			return size >= CAPACITY;
		}

		uint32_t remaining() const
		{
			return CAPACITY - size;
		}

		// returns pointer to next character in buffer
		char* tail()
		{
			return buffer + size;
		}

		uint32_t append(const string_t& suffix)
		{
			uint32_t append_length = min(remaining(), suffix.size);
			for (uint32_t i = 0; i < append_length; i++)
			{
				*tail() = suffix[i];
				size++;
			}

			return append_length;
		}
	};
}

#endif