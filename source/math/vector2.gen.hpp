
#pragma once
#ifndef KATHA_MATH_VECTOR_2_AUTO_GEN_H__
#define KATHA_MATH_VECTOR_2_AUTO_GEN_H__ 1

/*
	This is file is auto generated from `vector_op_gen.py`
	found at {root}/tools/vector_op_gen.py
*/

#include "../type/vector.hpp"
#include "utility.hpp"

#include <cmath>

namespace katha
{

	template <typename T>
	constexpr vector2_t<T> operator - (const vector2_t<T>& v1)
	{
		vector2_t<T> result = {};
		result.x = -v1.x;
		result.y = -v1.y;
		return result;
	}

	template <typename T>
	constexpr vector2_t<T> operator ~ (const vector2_t<T>& v1)
	{
		vector2_t<T> result = {};
		result.x = ~v1.x;
		result.y = ~v1.y;
		return result;
	}

	template <typename T>
	constexpr vector2_t<T> operator + (const vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		vector2_t<T> result = {};
		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		return result;
	}

	template <typename T>
	constexpr vector2_t<T> operator + (const vector2_t<T>& v1, const T& scalar)
	{
		vector2_t<T> result = {};
		result.x = v1.x + scalar;
		result.y = v1.y + scalar;
		return result;
	}

	template <typename T>
	vector2_t<T>& operator += (vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		v1.x = v1.x + v2.x;
		v1.y = v1.y + v2.y;
		return v1;
	}

	template <typename T>
	vector2_t<T>& operator += (vector2_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x + scalar;
		v1.y = v1.y + scalar;
		return v1;
	}

	template <typename T>
	constexpr vector2_t<T> operator - (const vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		vector2_t<T> result = {};
		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		return result;
	}

	template <typename T>
	constexpr vector2_t<T> operator - (const vector2_t<T>& v1, const T& scalar)
	{
		vector2_t<T> result = {};
		result.x = v1.x - scalar;
		result.y = v1.y - scalar;
		return result;
	}

	template <typename T>
	vector2_t<T>& operator -= (vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		v1.x = v1.x - v2.x;
		v1.y = v1.y - v2.y;
		return v1;
	}

	template <typename T>
	vector2_t<T>& operator -= (vector2_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x - scalar;
		v1.y = v1.y - scalar;
		return v1;
	}

	template <typename T>
	constexpr vector2_t<T> operator * (const vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		vector2_t<T> result = {};
		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;
		return result;
	}

	template <typename T>
	constexpr vector2_t<T> operator * (const vector2_t<T>& v1, const T& scalar)
	{
		vector2_t<T> result = {};
		result.x = v1.x * scalar;
		result.y = v1.y * scalar;
		return result;
	}

	template <typename T>
	vector2_t<T>& operator *= (vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		v1.x = v1.x * v2.x;
		v1.y = v1.y * v2.y;
		return v1;
	}

	template <typename T>
	vector2_t<T>& operator *= (vector2_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x * scalar;
		v1.y = v1.y * scalar;
		return v1;
	}

	template <typename T>
	constexpr vector2_t<T> operator / (const vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		vector2_t<T> result = {};
		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;
		return result;
	}

	template <typename T>
	constexpr vector2_t<T> operator / (const vector2_t<T>& v1, const T& scalar)
	{
		vector2_t<T> result = {};
		result.x = v1.x / scalar;
		result.y = v1.y / scalar;
		return result;
	}

	template <typename T>
	vector2_t<T>& operator /= (vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		v1.x = v1.x / v2.x;
		v1.y = v1.y / v2.y;
		return v1;
	}

	template <typename T>
	vector2_t<T>& operator /= (vector2_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x / scalar;
		v1.y = v1.y / scalar;
		return v1;
	}

	template <typename T>
	constexpr vector2_t<T> operator % (const vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		vector2_t<T> result = {};
		result.x = v1.x % v2.x;
		result.y = v1.y % v2.y;
		return result;
	}

	template <typename T>
	constexpr vector2_t<T> operator % (const vector2_t<T>& v1, const T& scalar)
	{
		vector2_t<T> result = {};
		result.x = v1.x % scalar;
		result.y = v1.y % scalar;
		return result;
	}

	template <typename T>
	vector2_t<T>& operator %= (vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		v1.x = v1.x % v2.x;
		v1.y = v1.y % v2.y;
		return v1;
	}

	template <typename T>
	vector2_t<T>& operator %= (vector2_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x % scalar;
		v1.y = v1.y % scalar;
		return v1;
	}

	template <typename T>
	constexpr vector2_t<T> operator | (const vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		vector2_t<T> result = {};
		result.x = v1.x | v2.x;
		result.y = v1.y | v2.y;
		return result;
	}

	template <typename T>
	constexpr vector2_t<T> operator | (const vector2_t<T>& v1, const T& scalar)
	{
		vector2_t<T> result = {};
		result.x = v1.x | scalar;
		result.y = v1.y | scalar;
		return result;
	}

	template <typename T>
	vector2_t<T>& operator |= (vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		v1.x = v1.x | v2.x;
		v1.y = v1.y | v2.y;
		return v1;
	}

	template <typename T>
	vector2_t<T>& operator |= (vector2_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x | scalar;
		v1.y = v1.y | scalar;
		return v1;
	}

	template <typename T>
	constexpr vector2_t<T> operator & (const vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		vector2_t<T> result = {};
		result.x = v1.x & v2.x;
		result.y = v1.y & v2.y;
		return result;
	}

	template <typename T>
	constexpr vector2_t<T> operator & (const vector2_t<T>& v1, const T& scalar)
	{
		vector2_t<T> result = {};
		result.x = v1.x & scalar;
		result.y = v1.y & scalar;
		return result;
	}

	template <typename T>
	vector2_t<T>& operator &= (vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		v1.x = v1.x & v2.x;
		v1.y = v1.y & v2.y;
		return v1;
	}

	template <typename T>
	vector2_t<T>& operator &= (vector2_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x & scalar;
		v1.y = v1.y & scalar;
		return v1;
	}

	template <typename T>
	constexpr vector2_t<T> operator ^ (const vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		vector2_t<T> result = {};
		result.x = v1.x ^ v2.x;
		result.y = v1.y ^ v2.y;
		return result;
	}

	template <typename T>
	constexpr vector2_t<T> operator ^ (const vector2_t<T>& v1, const T& scalar)
	{
		vector2_t<T> result = {};
		result.x = v1.x ^ scalar;
		result.y = v1.y ^ scalar;
		return result;
	}

	template <typename T>
	vector2_t<T>& operator ^= (vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		v1.x = v1.x ^ v2.x;
		v1.y = v1.y ^ v2.y;
		return v1;
	}

	template <typename T>
	vector2_t<T>& operator ^= (vector2_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x ^ scalar;
		v1.y = v1.y ^ scalar;
		return v1;
	}

	template <typename T>
	constexpr vector2_t<T> operator << (const vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		vector2_t<T> result = {};
		result.x = v1.x << v2.x;
		result.y = v1.y << v2.y;
		return result;
	}

	template <typename T>
	constexpr vector2_t<T> operator << (const vector2_t<T>& v1, const T& scalar)
	{
		vector2_t<T> result = {};
		result.x = v1.x << scalar;
		result.y = v1.y << scalar;
		return result;
	}

	template <typename T>
	vector2_t<T>& operator <<= (vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		v1.x = v1.x << v2.x;
		v1.y = v1.y << v2.y;
		return v1;
	}

	template <typename T>
	vector2_t<T>& operator <<= (vector2_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x << scalar;
		v1.y = v1.y << scalar;
		return v1;
	}

	template <typename T>
	constexpr vector2_t<T> operator >> (const vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		vector2_t<T> result = {};
		result.x = v1.x >> v2.x;
		result.y = v1.y >> v2.y;
		return result;
	}

	template <typename T>
	constexpr vector2_t<T> operator >> (const vector2_t<T>& v1, const T& scalar)
	{
		vector2_t<T> result = {};
		result.x = v1.x >> scalar;
		result.y = v1.y >> scalar;
		return result;
	}

	template <typename T>
	vector2_t<T>& operator >>= (vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		v1.x = v1.x >> v2.x;
		v1.y = v1.y >> v2.y;
		return v1;
	}

	template <typename T>
	vector2_t<T>& operator >>= (vector2_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x >> scalar;
		v1.y = v1.y >> scalar;
		return v1;
	}

	template <typename T>
	vector2_t<T> abs (const vector2_t<T>& v)
	{
		vector2_t<T> result = {};
		result.x = abs(v.x);
		result.y = abs(v.y);
		return result;
	}

	template <typename T>
	vector2_t<T> radians (const vector2_t<T>& v)
	{
		vector2_t<T> result = {};
		result.x = radians(v.x);
		result.y = radians(v.y);
		return result;
	}

	template <typename T>
	vector2_t<T> degrees (const vector2_t<T>& v)
	{
		vector2_t<T> result = {};
		result.x = degrees(v.x);
		result.y = degrees(v.y);
		return result;
	}

	template <typename T>
	vector2_t<T> sin (const vector2_t<T>& v)
	{
		vector2_t<T> result = {};
		result.x = sin(v.x);
		result.y = sin(v.y);
		return result;
	}

	template <typename T>
	vector2_t<T> cos (const vector2_t<T>& v)
	{
		vector2_t<T> result = {};
		result.x = cos(v.x);
		result.y = cos(v.y);
		return result;
	}

	template <typename T>
	vector2_t<T> tan (const vector2_t<T>& v)
	{
		vector2_t<T> result = {};
		result.x = tan(v.x);
		result.y = tan(v.y);
		return result;
	}

	template <typename T>
	vector2_t<T> acos (const vector2_t<T>& v)
	{
		vector2_t<T> result = {};
		result.x = acos(v.x);
		result.y = acos(v.y);
		return result;
	}

	template <typename T>
	vector2_t<T> asin (const vector2_t<T>& v)
	{
		vector2_t<T> result = {};
		result.x = asin(v.x);
		result.y = asin(v.y);
		return result;
	}

	template <typename T>
	vector2_t<T> min (const vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		vector2_t<T> result = {};
		result.x = min(v1.x, v2.x);
		result.y = min(v1.y, v2.y);
		return result;
	}

	template <typename T>
	vector2_t<T> max (const vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		vector2_t<T> result = {};
		result.x = max(v1.x, v2.x);
		result.y = max(v1.y, v2.y);
		return result;
	}

	template <typename T>
	vector2_t<T> atan2 (const vector2_t<T>& v1, const vector2_t<T>& v2)
	{
		vector2_t<T> result = {};
		result.x = atan2(v1.x, v2.x);
		result.y = atan2(v1.y, v2.y);
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
		const vector2_t<T>& vv = v1 * v2;
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

	template <typename T, typename LT=float>
	vector2_t<T> normalize(const vector2_t<T>& v)
	{
		const LT len = length<LT>(v);
		const vector2_t<T> norm = v / len;
		return norm;
	}

}
#endif