#pragma once
#ifndef KATHA_MATH_VECTOR_4_H__
#define KATHA_MATH_VECTOR_4_H__ 1

#include "../core/types.hpp"

#include <cmath>

namespace katha
{
	template <typename T>
	vector4_t<T> operator * (const vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		vector4_t<T> result(
			v1.x * v2.x,
			v1.y * v2.y,
			v1.z * v2.z,
			v1.w * v2.w
		);
		return result;
	}

	// x + y + z + w
	template <typename T>
	T horizontal_sum(const vector4_t<T>& v)
	{
		T result = v.x + v.y + v.z + v.w;
		return result;
	}
}

#endif