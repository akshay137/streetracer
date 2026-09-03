#pragma once
#ifndef KATHA_FORMAT_TYPES_H__
#define KATHA_FORMAT_TYPES_H__ 1

#include "../type/primitive.hpp"
#include "../enum/base.hpp"

namespace katha
{
	uint32_t IntToString(const int32_t number, char* buffer, const Base base = Base::DECIMAL);
	uint32_t UIntToString(const uint32_t number, char* buffer, const Base base = Base::DECIMAL);

	uint32_t Int64ToString(const int64_t number, char* buffer, const Base base = Base::DECIMAL);
	uint32_t UInt64ToString(const uint64_t number, char* buffer, const Base base = Base::DECIMAL);

	uint32_t FloatToString(const float number, char* buffer);
	uint32_t DoubleToString(const double number, char* buffer);
}

#endif