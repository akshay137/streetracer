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
	static bool WriteChecked(T* ptr, const T& value)
	{
		if (nullptr == ptr)
		{
			return false;
		}

		*ptr = value;
		return true;
	}

	template <typename T>
	static bool ReadChecked(const T* ptr, T* out_value)
	{
		if (nullptr == ptr)
		{
			return false;
		}

		return WriteChecked<T>(out_value, *ptr);
	}

	constexpr static float PI = 3.14159265359f;
	
	template <typename T>
	static constexpr T Abs(const T& v)
	{
		return (v < 0) ? -v : v;
	}

	template <typename T>
	static constexpr T Min(const T& lhs, const T& rhs)
	{
		return lhs < rhs ? lhs : rhs;
	}

	template <typename T>
	static constexpr T Max(const T& lhs, const T& rhs)
	{
		return lhs > rhs ? lhs : rhs;
	}

	template <typename T>
	static constexpr T Clamp(const T& value, const T& lower, const T& upper)
	{
		return Max(lower, Min(value, upper));
	}

	template <typename T>
	static constexpr T Lerp(const T& source, const T& target, const float step)
	{
		const T diff = target - source;
		const T result = source + diff * step;
		return result;
	}

	constexpr static inline float Radians(const float angle_in_degrees)
	{
		float result = angle_in_degrees * (PI / 180.0f);
		return result;
	}

	constexpr static inline float Degrees(const float angle_in_radians)
	{
		float result = angle_in_radians * (180.0f / PI);
		return result;
	}
}

#endif