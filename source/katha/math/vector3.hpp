#pragma once
#ifndef KATHA_MATH_VECTOR_3_H__
#define KATHA_MATH_VECTOR_3_H__ 1

#include "../core/types.hpp"

#include <cmath>

namespace katha
{
	template <typename T>
	vector3_t<T> radians(const vector3_t<T>& v)
	{
		vector3_t<T> result(
			radians(v.x),
			radians(v.y),
			radians(v.z)
		);
		return result;
	}

	template <typename T>
	vector3_t<T> degrees(const vector3_t<T>& v)
	{
		vector3_t<T> result(
			degrees(v.x),
			degrees(v.y),
			degrees(v.z)
		);
		return result;
	}

	template <typename T>
	vector3_t<T> sine(const vector3_t<T>& v)
	{
		vector3_t<T> result(
			std::sin(v.x),
			std::sin(v.y),
			std::sin(v.z)
		);
		return result;
	}

	template <typename T>
	vector3_t<T> cosine(const vector3_t<T>& v)
	{
		vector3_t<T> result(
			std::cos(v.x),
			std::cos(v.y),
			std::cos(v.z)
		);
		return result;
	}

	template <typename T>
	vector3_t<T> operator + (const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		const vector3_t<T> result(
			v1.x + v2.x,
			v1.y + v2.y,
			v1.z + v2.z
		);
		return result;
	}

	template <typename T>
	vector3_t<T> operator - (const vector3_t<T>& v)
	{
		vector3_t<T> result(-v.x, -v.y, -v.z);
		return result;
	}

	template <typename T>
	vector3_t<T>& operator -= (vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		v1.x -= v2.x;
		v1.y -= v2.y;
		v1.z -= v2.z;
		return v1;
	}

	template <typename T>
	vector3_t<T> operator - (const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		vector3_t<T> temp = v1;
		temp -= v2;
		return temp;
	}

	template <typename T>
	vector3_t<T> operator * (const vector3_t<T>& v1, const T s)
	{
		const vector3_t<T> result(
			v1.x * s,
			v1.y * s,
			v1.z * s
		);
		return result;
	}

	template <typename T>
	vector3_t<T> operator * (const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		const vector3_t<T> result(
			v1.x * v2.x,
			v1.y * v2.y,
			v1.z * v2.z
		);
		return result;
	}

	template <typename T>
	vector3_t<T> operator / (const vector3_t<T>& v, const T& s)
	{
		const vector3_t<T> result(
			v.x / s,
			v.y / s,
			v.z / s
		);
		return result;
	}

	template <typename T>
	vector3_t<T> clamp(const vector3_t<T>& v, const T& lower, const T& upper)
	{
		vector3_t<T> result(
			clamp<T>(v.x, lower, upper),
			clamp<T>(v.y, lower, upper),
			clamp<T>(v.z, lower, upper)
		);
		return result;
	}

	// x + y + z
	template <typename T>
	T horizontal_sum(const vector3_t<T>& v)
	{
		const T result = v.x + v.y + v.z;
		return result;
	}

	template <typename T>
	T horizontal_product(const vector3_t<T>& v)
	{
		const T result = v.x * v.y * v.z;
		return result;
	}

	template <typename T>
	T dot(const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		const vector3_t<T> product = v1 * v2;
		T result = horizontal_sum(product);
		return result;
	}

	template <typename T>
	T squared_length(const vector3_t<T>& v)
	{
		const T result = dot(v, v);
		return result;
	}

	template <typename T>
	T length(const vector3_t<T>& v)
	{
		const T s_len = squared_length(v);
		const T result = std::sqrt(s_len);
		return result;
	}

	template <typename T>
	vector3_t<T> normalize(const vector3_t<T>& v)
	{
		const T len = length(v);
		const vector3_t<T> result =  v / len;
		return result;
	}

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