#pragma once
#ifndef KATHA_MATH_MATRIX_4_H__
#define KATHA_MATH_MATRIX_4_H__ 1

#include "../core/types.hpp"
#include "vector4.hpp"

#include <cmath>

namespace katha
{
	template <typename T>
	matrix4_t<T> transpose(const matrix4_t<T>& m)
	{
		matrix4_t<T> result = {};
		result[0] = vector4_t<T>(m[0][0], m[1][0], m[2][0], m[3][0]);
		result[1] = vector4_t<T>(m[0][1], m[1][1], m[2][1], m[3][1]);
		result[2] = vector4_t<T>(m[0][2], m[1][2], m[2][2], m[3][2]);
		result[3] = vector4_t<T>(m[0][3], m[1][3], m[2][3], m[3][3]);
		return result;
	};

	template <typename T>
	matrix4_t<T> operator * (const matrix4_t<T>& m1, const matrix4_t<T>& m2)
	{
		const matrix4_t<T> t = transpose(m1);
		matrix4_t<T> result = {};
		
		result[0][0] = horizontal_sum(t[0] * m2[0]);
		result[0][1] = horizontal_sum(t[1] * m2[0]);
		result[0][2] = horizontal_sum(t[2] * m2[0]);
		result[0][3] = horizontal_sum(t[3] * m2[0]);
		result[1][0] = horizontal_sum(t[0] * m2[1]);
		result[1][1] = horizontal_sum(t[1] * m2[1]);
		result[1][2] = horizontal_sum(t[2] * m2[1]);
		result[1][3] = horizontal_sum(t[3] * m2[1]);
		result[2][0] = horizontal_sum(t[0] * m2[2]);
		result[2][1] = horizontal_sum(t[1] * m2[2]);
		result[2][2] = horizontal_sum(t[2] * m2[2]);
		result[2][3] = horizontal_sum(t[3] * m2[2]);
		result[3][0] = horizontal_sum(t[0] * m2[3]);
		result[3][1] = horizontal_sum(t[1] * m2[3]);
		result[3][2] = horizontal_sum(t[2] * m2[3]);
		result[3][3] = horizontal_sum(t[3] * m2[3]);

		return result;
	}

	template <typename T>
	matrix4_t<T> translate(const matrix4_t<T>& m, const vector3_t<T>& v)
	{
		matrix4_t<T> t(1.0f);
		t[3] = vector4_t<T>(v.x, v.y, v.z, 1.0f);
		
		matrix4_t<T> result = m * t;
		return result;
	}
}

#endif