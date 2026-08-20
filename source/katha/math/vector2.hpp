#pragma once
#ifndef KATHA_MATH_VECTOR_2_H__
#define KATHA_MATH_VECTOR_2_H__ 1

#include "../core/types.hpp"

#include <cmath>

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
	vector2_t<T>& operator += (vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		v1.x += v2.x;
		v1.y += v2.y;
		return v1;
	}

	template <typename T>
	vector2_t<T>& operator *= (vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		v1.x *= v2.x;
		v1.y *= v2.y;
		return v1;
	}

	template <typename T>
	vector2_t<T> operator * (const vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		vector2_t<T> temp = v1;
		temp *= v2;
		return temp;
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

	template <typename T>
	T horizontal_sum(const vector2_t<T>& v)
	{
		T result = v.x + v.y;
		return result;
	}

	template <typename T>
	T dot(const vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		const vector2_t<T> vv = v1 * v2;
		const T result = horizontal_sum(vv);
		return result;
	}

	template <typename T>
	T squared_length(const vector2_t<T>& v)
	{
		const T slen = dot(v, v);
		return slen;
	}

	template <typename T, typename RT=float>
	RT length(const vector2_t<T>& v)
	{
		const T slen = dot(v, v);
		const RT len = std::sqrt(slen);
		return len;
	}
}

#endif