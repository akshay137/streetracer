#pragma once
#ifndef KATHA_FORMAT_TYPES_H__
#define KATHA_FORMAT_TYPES_H__ 1

#include "../type/primitive.hpp"
#include "../enum/base.hpp"

namespace katha
{
	uint32_t int_to_string(const int32_t number, char* buffer, const base_e base = base_e::decimal);
	uint32_t uint_to_string(const uint32_t number, char* buffer, const base_e base = base_e::decimal);

	uint32_t int64_to_string(const int64_t number, char* buffer, const base_e base = base_e::decimal);
	uint32_t uint64_to_string(const uint64_t number, char* buffer, const base_e base = base_e::decimal);

	uint32_t float_to_string(const float number, char* buffer);
	uint32_t double_to_string(const double number, char* buffer);
}

#endif