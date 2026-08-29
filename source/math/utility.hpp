#pragma once
#ifndef KATHA_MATH_UTILITY_H__
#define KATHA_MATH_UTILITY_H__ 1

namespace katha
{
	constexpr static float PI = 3.14159265359f;
	
	template <typename T>
	static constexpr T abs(const T& v)
	{
		return (v < 0) ? -v : v;
	}

	template <typename T>
	static constexpr T min(const T& lhs, const T& rhs)
	{
		return lhs < rhs ? lhs : rhs;
	}

	template <typename T>
	static constexpr T max(const T& lhs, const T& rhs)
	{
		return lhs > rhs ? lhs : rhs;
	}

	template <typename T>
	static constexpr T clamp(const T& value, const T& lower, const T& upper)
	{
		return max(lower, min(value, upper));
	}

	template <typename T>
	static constexpr T lerp(const T& source, const T& target, const float step)
	{
		const T diff = target - source;
		const T result = source + diff * step;
		return result;
	}

	constexpr static inline float radians(const float angle_in_degrees)
	{
		float result = angle_in_degrees * (PI / 180.0f);
		return result;
	}

	constexpr static inline float degrees(const float angle_in_radians)
	{
		float result = angle_in_radians * (180.0f / PI);
		return result;
	}
}

#endif