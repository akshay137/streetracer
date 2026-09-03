#include "types.hpp"

#include <cstdio>

constexpr const char* DIGIT_STRING = "0123456789ABCDEF";

inline const char* GetBasePrefixReversed(const katha::Base base)
{
	switch (base)
	{
		case katha::Base::BINARY: return "b0";
		case katha::Base::OCTAL: return "o0";
		case katha::Base::DECIMAL: return "";
		case katha::Base::HEXADECIMAL: return "x0";
	}
	return "";
}

uint32_t katha::IntToString(const int32_t number, char* buffer, const Base base)
{
	if (0 == number)
	{
		buffer[0] = '0';
		return 1;
	}

	constexpr uint32_t BUFFER_SIZE = 48;
	char temp[BUFFER_SIZE] = {};
	uint32_t index = BUFFER_SIZE; // start writing from end

	const int32_t FACTOR = static_cast<int32_t>(base);
	int32_t v = number;
	while (v)
	{
		const int digit = Abs(v % FACTOR);
		v /= FACTOR;
		temp[--index] = DIGIT_STRING[digit];
	}
	if (number < 0) {
		temp[--index] = '-';
	}
	
	const char* prefix = GetBasePrefixReversed(base);
	while (*prefix)
	{
		temp[--index] = *prefix;
		prefix++;
	}

	for (uint32_t i = 0; i < BUFFER_SIZE - index; i++) {
		buffer[i] = temp[index + i];
	}

	return BUFFER_SIZE - index;
}

uint32_t katha::UIntToString(const uint32_t number, char* buffer, const Base base)
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

	const char* prefix = GetBasePrefixReversed(base);
	while (*prefix)
	{
		temp[--index] = *prefix;
		prefix++;
	}

	for (uint32_t i = 0; i < BUFFER_SIZE - index; i++) {
		buffer[i] = temp[index + i];
	}

	return BUFFER_SIZE - index;
}

uint32_t katha::Int64ToString(const int64_t number, char* buffer, const Base base)
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
		const int digit = Abs(v % FACTOR);
		v /= FACTOR;
		temp[--index] = DIGIT_STRING[digit];
	}
	if (number < 0) {
		temp[--index] = '-';
	}

	const char* prefix = GetBasePrefixReversed(base);
	while (*prefix)
	{
		temp[--index] = *prefix;
		prefix++;
	}

	for (uint32_t i = 0; i < BUFFER_SIZE - index; i++) {
		buffer[i] = temp[index + i];
	}

	return BUFFER_SIZE - index;
}

uint32_t katha::UInt64ToString(const uint64_t number, char* buffer, const Base base)
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

	const char* prefix = GetBasePrefixReversed(base);
	while (*prefix)
	{
		temp[--index] = *prefix;
		prefix++;
	}
	
	for (uint32_t i = 0; i < BUFFER_SIZE - index; i++) {
		buffer[i] = temp[index + i];
	}

	return BUFFER_SIZE - index;
}

uint32_t katha::FloatToString(const float number, char* buffer)
{
	int len = snprintf(buffer, 16, "%g", number);
	return static_cast<uint32_t>(len);
}

uint32_t katha::DoubleToString(const double number, char* buffer)
{
	int len = snprintf(buffer, 16, "%g", number);
	return static_cast<uint32_t>(len);
}