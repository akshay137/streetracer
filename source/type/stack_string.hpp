#pragma once
#ifndef KATHA_CORE_STACK_STRING_H__
#define KATHA_CORE_STACK_STRING_H__ 1

#include "primitive.hpp"
#include "string.hpp"

namespace katha
{
	template <uint16_t CAPACITY>
	struct StackString
	{
		uint32_t size = 0;
		char buffer[CAPACITY] = {};

		constexpr uint16_t capacity() const
		{
			return CAPACITY;
		}

		bool isFull() const
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

		uint32_t append(const String& suffix)
		{
			uint32_t append_length = Min(remaining(), suffix.size);
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