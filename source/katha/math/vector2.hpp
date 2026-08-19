#pragma once
#ifndef KATHA_MATH_VECTOR_2_H__
#define KATHA_MATH_VECTOR_2_H__ 1

#include "../core/types.hpp"

namespace katha
{
	template <typename T, typename ResultType = float>
	ResultType aspect_xy(const vector2_t<T>& v)
	{
		ResultType aspect = static_cast<ResultType>(v.x)
			/ static_cast<ResultType>(v.y);
		return aspect;
	}

	template <typename T>
	vector2_t<T> operator += (const vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		vector2_t<T> result(v1.x + v2.x, v1.y + v2.y);
		return result;
	}

	template <typename T>
	vector2_t<T> clamp(const vector2_t<T>& v, const T& lower, const T& upper)
	{
		vector2_t<T> result(
			clamp<T>(v.x, lower, upper),
			clamp<T>(v.y, lower, upper)
		);
		return result;
	}
}

#endif