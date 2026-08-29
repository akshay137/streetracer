#pragma once
#ifndef KATHA_MATH_VECTOR_3_H__
#define KATHA_MATH_VECTOR_3_H__ 1

#include "vector3.gen.hpp"

namespace katha
{
	template <typename T>
	vector3_t<T> cross(const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		const vector3_t<T> result(
			(v1.y * v2.z) - (v1.z * v2.y),
			(v1.z * v2.x) - (v1.x * v2.z),
			(v1.x * v2.y) - (v1.y * v2.x)
		);
		return result;
	}
}

#endif