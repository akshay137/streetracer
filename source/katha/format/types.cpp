#include "types.hpp"
#include "../core/types.hpp"

#include <cstdio>

constexpr const char* DIGIT_STRING = "0123456789ABCDEF";

inline const char* get_base_prefix_reversed(const katha::base_e base)
{
	switch (base)
	{
		case katha::base_e::binary: return "b0";
		case katha::base_e::octal: return "o0";
		case katha::base_e::decimal: return "";
		case katha::base_e::hexadecimal: return "x0";
	}
	return "";
}

uint32_t katha::int_to_string(const int32_t number, char* buffer, const base_e base)
{
	if (0 == number)
	{
		buffer[0] = '0';
		return 1;
	}

	constexpr uint32_t BUFFER_SIZE = 48;
	char temp[BUFFER_SIZE] = {};
	uint32_t index = BUFFER_SIZE; // start writing from end

	const int FACTOR = static_cast<int>(base);
	int v = number;
	while (v)
	{
		const int digit = abs(v % FACTOR);
		v /= FACTOR;
		temp[--index] = DIGIT_STRING[digit];
	}
	if (number < 0)
	{
		temp[--index] = '-';
	}
	
	const char* prefix = get_base_prefix_reversed(base);
	while (*prefix)
	{
		temp[--index] = *prefix;
		prefix++;
	}

	for (uint32_t i = 0; i < BUFFER_SIZE - index; i++)
	{
		buffer[i] = temp[index + i];
	}

	return BUFFER_SIZE - index;
}

uint32_t katha::uint_to_string(const uint32_t number, char* buffer, const base_e base)
{
	if (0 == number)
	{
		buffer[0] = '0';
		return 1;
	}

	constexpr uint32_t BUFFER_SIZE = 48;
	char temp[BUFFER_SIZE] = {};
	uint32_t index = BUFFER_SIZE; // start writing from end

	const uint32_t FACTOR = static_cast<uint32_t>(base);
	uint32_t v = number;
	while (v)
	{
		const int digit = v % FACTOR;
		v /= FACTOR;
		temp[--index] = DIGIT_STRING[digit];
	}

	const char* prefix = get_base_prefix_reversed(base);
	while (*prefix)
	{
		temp[--index] = *prefix;
		prefix++;
	}

	for (uint32_t i = 0; i < BUFFER_SIZE - index; i++)
	{
		buffer[i] = temp[index + i];
	}

	return BUFFER_SIZE - index;
}

uint32_t katha::int64_to_string(const int64_t number, char* buffer, const base_e base)
{
	if (0 == number)
	{
		buffer[0] = '0';
		return 1;
	}

	constexpr uint32_t BUFFER_SIZE = 72;
	char temp[BUFFER_SIZE] = {};
	uint32_t index = BUFFER_SIZE; // start writing from end

	const int64_t FACTOR = static_cast<int64_t>(base);
	int64_t v = number;
	while (v)
	{
		const int digit = abs(v % FACTOR);
		v /= FACTOR;
		temp[--index] = DIGIT_STRING[digit];
	}
	if (number < 0)
	{
		temp[--index] = '-';
	}

	const char* prefix = get_base_prefix_reversed(base);
	while (*prefix)
	{
		temp[--index] = *prefix;
		prefix++;
	}

	for (uint32_t i = 0; i < BUFFER_SIZE - index; i++)
	{
		buffer[i] = temp[index + i];
	}

	return BUFFER_SIZE - index;
}

uint32_t katha::uint64_to_string(const uint64_t number, char* buffer, const base_e base)
{
	if (0 == number)
	{
		buffer[0] = '0';
		return 1;
	}

	const uint64_t FACTOR = static_cast<uint64_t>(base);
	constexpr uint32_t BUFFER_SIZE = 72;
	char temp[BUFFER_SIZE] = {};
	uint32_t index = BUFFER_SIZE; // start writing from end

	uint64_t v = number;
	while (v)
	{
		const int digit = v % FACTOR;
		v /= FACTOR;
		temp[--index] = DIGIT_STRING[digit];
	}

	const char* prefix = get_base_prefix_reversed(base);
	while (*prefix)
	{
		temp[--index] = *prefix;
		prefix++;
	}
	
	for (uint32_t i = 0; i < BUFFER_SIZE - index; i++)
	{
		buffer[i] = temp[index + i];
	}

	return BUFFER_SIZE - index;
}

uint32_t katha::float_to_string(const float number, char* buffer)
{
	int len = snprintf(buffer, 16, "%g", number);
	return static_cast<uint32_t>(len);
}