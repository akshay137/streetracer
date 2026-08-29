
#pragma once
#ifndef KATHA_MATH_VECTOR_4_AUTO_GEN_H__
#define KATHA_MATH_VECTOR_4_AUTO_GEN_H__ 1

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
	constexpr vector4_t<T> operator - (const vector4_t<T>& v1)
	{
		vector4_t<T> result = {};
		result.x = -v1.x;
		result.y = -v1.y;
		result.z = -v1.z;
		result.w = -v1.w;
		return result;
	}

	template <typename T>
	constexpr vector4_t<T> operator ~ (const vector4_t<T>& v1)
	{
		vector4_t<T> result = {};
		result.x = ~v1.x;
		result.y = ~v1.y;
		result.z = ~v1.z;
		result.w = ~v1.w;
		return result;
	}

	template <typename T>
	constexpr vector4_t<T> operator + (const vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		vector4_t<T> result = {};
		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z + v2.z;
		result.w = v1.w + v2.w;
		return result;
	}

	template <typename T>
	constexpr vector4_t<T> operator + (const vector4_t<T>& v1, const T& scalar)
	{
		vector4_t<T> result = {};
		result.x = v1.x + scalar;
		result.y = v1.y + scalar;
		result.z = v1.z + scalar;
		result.w = v1.w + scalar;
		return result;
	}

	template <typename T>
	vector4_t<T>& operator += (vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		v1.x = v1.x + v2.x;
		v1.y = v1.y + v2.y;
		v1.z = v1.z + v2.z;
		v1.w = v1.w + v2.w;
		return v1;
	}

	template <typename T>
	vector4_t<T>& operator += (vector4_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x + scalar;
		v1.y = v1.y + scalar;
		v1.z = v1.z + scalar;
		v1.w = v1.w + scalar;
		return v1;
	}

	template <typename T>
	constexpr vector4_t<T> operator - (const vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		vector4_t<T> result = {};
		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		result.z = v1.z - v2.z;
		result.w = v1.w - v2.w;
		return result;
	}

	template <typename T>
	constexpr vector4_t<T> operator - (const vector4_t<T>& v1, const T& scalar)
	{
		vector4_t<T> result = {};
		result.x = v1.x - scalar;
		result.y = v1.y - scalar;
		result.z = v1.z - scalar;
		result.w = v1.w - scalar;
		return result;
	}

	template <typename T>
	vector4_t<T>& operator -= (vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		v1.x = v1.x - v2.x;
		v1.y = v1.y - v2.y;
		v1.z = v1.z - v2.z;
		v1.w = v1.w - v2.w;
		return v1;
	}

	template <typename T>
	vector4_t<T>& operator -= (vector4_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x - scalar;
		v1.y = v1.y - scalar;
		v1.z = v1.z - scalar;
		v1.w = v1.w - scalar;
		return v1;
	}

	template <typename T>
	constexpr vector4_t<T> operator * (const vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		vector4_t<T> result = {};
		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;
		result.z = v1.z * v2.z;
		result.w = v1.w * v2.w;
		return result;
	}

	template <typename T>
	constexpr vector4_t<T> operator * (const vector4_t<T>& v1, const T& scalar)
	{
		vector4_t<T> result = {};
		result.x = v1.x * scalar;
		result.y = v1.y * scalar;
		result.z = v1.z * scalar;
		result.w = v1.w * scalar;
		return result;
	}

	template <typename T>
	vector4_t<T>& operator *= (vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		v1.x = v1.x * v2.x;
		v1.y = v1.y * v2.y;
		v1.z = v1.z * v2.z;
		v1.w = v1.w * v2.w;
		return v1;
	}

	template <typename T>
	vector4_t<T>& operator *= (vector4_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x * scalar;
		v1.y = v1.y * scalar;
		v1.z = v1.z * scalar;
		v1.w = v1.w * scalar;
		return v1;
	}

	template <typename T>
	constexpr vector4_t<T> operator / (const vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		vector4_t<T> result = {};
		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;
		result.z = v1.z / v2.z;
		result.w = v1.w / v2.w;
		return result;
	}

	template <typename T>
	constexpr vector4_t<T> operator / (const vector4_t<T>& v1, const T& scalar)
	{
		vector4_t<T> result = {};
		result.x = v1.x / scalar;
		result.y = v1.y / scalar;
		result.z = v1.z / scalar;
		result.w = v1.w / scalar;
		return result;
	}

	template <typename T>
	vector4_t<T>& operator /= (vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		v1.x = v1.x / v2.x;
		v1.y = v1.y / v2.y;
		v1.z = v1.z / v2.z;
		v1.w = v1.w / v2.w;
		return v1;
	}

	template <typename T>
	vector4_t<T>& operator /= (vector4_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x / scalar;
		v1.y = v1.y / scalar;
		v1.z = v1.z / scalar;
		v1.w = v1.w / scalar;
		return v1;
	}

	template <typename T>
	constexpr vector4_t<T> operator % (const vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		vector4_t<T> result = {};
		result.x = v1.x % v2.x;
		result.y = v1.y % v2.y;
		result.z = v1.z % v2.z;
		result.w = v1.w % v2.w;
		return result;
	}

	template <typename T>
	constexpr vector4_t<T> operator % (const vector4_t<T>& v1, const T& scalar)
	{
		vector4_t<T> result = {};
		result.x = v1.x % scalar;
		result.y = v1.y % scalar;
		result.z = v1.z % scalar;
		result.w = v1.w % scalar;
		return result;
	}

	template <typename T>
	vector4_t<T>& operator %= (vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		v1.x = v1.x % v2.x;
		v1.y = v1.y % v2.y;
		v1.z = v1.z % v2.z;
		v1.w = v1.w % v2.w;
		return v1;
	}

	template <typename T>
	vector4_t<T>& operator %= (vector4_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x % scalar;
		v1.y = v1.y % scalar;
		v1.z = v1.z % scalar;
		v1.w = v1.w % scalar;
		return v1;
	}

	template <typename T>
	constexpr vector4_t<T> operator | (const vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		vector4_t<T> result = {};
		result.x = v1.x | v2.x;
		result.y = v1.y | v2.y;
		result.z = v1.z | v2.z;
		result.w = v1.w | v2.w;
		return result;
	}

	template <typename T>
	constexpr vector4_t<T> operator | (const vector4_t<T>& v1, const T& scalar)
	{
		vector4_t<T> result = {};
		result.x = v1.x | scalar;
		result.y = v1.y | scalar;
		result.z = v1.z | scalar;
		result.w = v1.w | scalar;
		return result;
	}

	template <typename T>
	vector4_t<T>& operator |= (vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		v1.x = v1.x | v2.x;
		v1.y = v1.y | v2.y;
		v1.z = v1.z | v2.z;
		v1.w = v1.w | v2.w;
		return v1;
	}

	template <typename T>
	vector4_t<T>& operator |= (vector4_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x | scalar;
		v1.y = v1.y | scalar;
		v1.z = v1.z | scalar;
		v1.w = v1.w | scalar;
		return v1;
	}

	template <typename T>
	constexpr vector4_t<T> operator & (const vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		vector4_t<T> result = {};
		result.x = v1.x & v2.x;
		result.y = v1.y & v2.y;
		result.z = v1.z & v2.z;
		result.w = v1.w & v2.w;
		return result;
	}

	template <typename T>
	constexpr vector4_t<T> operator & (const vector4_t<T>& v1, const T& scalar)
	{
		vector4_t<T> result = {};
		result.x = v1.x & scalar;
		result.y = v1.y & scalar;
		result.z = v1.z & scalar;
		result.w = v1.w & scalar;
		return result;
	}

	template <typename T>
	vector4_t<T>& operator &= (vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		v1.x = v1.x & v2.x;
		v1.y = v1.y & v2.y;
		v1.z = v1.z & v2.z;
		v1.w = v1.w & v2.w;
		return v1;
	}

	template <typename T>
	vector4_t<T>& operator &= (vector4_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x & scalar;
		v1.y = v1.y & scalar;
		v1.z = v1.z & scalar;
		v1.w = v1.w & scalar;
		return v1;
	}

	template <typename T>
	constexpr vector4_t<T> operator ^ (const vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		vector4_t<T> result = {};
		result.x = v1.x ^ v2.x;
		result.y = v1.y ^ v2.y;
		result.z = v1.z ^ v2.z;
		result.w = v1.w ^ v2.w;
		return result;
	}

	template <typename T>
	constexpr vector4_t<T> operator ^ (const vector4_t<T>& v1, const T& scalar)
	{
		vector4_t<T> result = {};
		result.x = v1.x ^ scalar;
		result.y = v1.y ^ scalar;
		result.z = v1.z ^ scalar;
		result.w = v1.w ^ scalar;
		return result;
	}

	template <typename T>
	vector4_t<T>& operator ^= (vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		v1.x = v1.x ^ v2.x;
		v1.y = v1.y ^ v2.y;
		v1.z = v1.z ^ v2.z;
		v1.w = v1.w ^ v2.w;
		return v1;
	}

	template <typename T>
	vector4_t<T>& operator ^= (vector4_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x ^ scalar;
		v1.y = v1.y ^ scalar;
		v1.z = v1.z ^ scalar;
		v1.w = v1.w ^ scalar;
		return v1;
	}

	template <typename T>
	constexpr vector4_t<T> operator << (const vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		vector4_t<T> result = {};
		result.x = v1.x << v2.x;
		result.y = v1.y << v2.y;
		result.z = v1.z << v2.z;
		result.w = v1.w << v2.w;
		return result;
	}

	template <typename T>
	constexpr vector4_t<T> operator << (const vector4_t<T>& v1, const T& scalar)
	{
		vector4_t<T> result = {};
		result.x = v1.x << scalar;
		result.y = v1.y << scalar;
		result.z = v1.z << scalar;
		result.w = v1.w << scalar;
		return result;
	}

	template <typename T>
	vector4_t<T>& operator <<= (vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		v1.x = v1.x << v2.x;
		v1.y = v1.y << v2.y;
		v1.z = v1.z << v2.z;
		v1.w = v1.w << v2.w;
		return v1;
	}

	template <typename T>
	vector4_t<T>& operator <<= (vector4_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x << scalar;
		v1.y = v1.y << scalar;
		v1.z = v1.z << scalar;
		v1.w = v1.w << scalar;
		return v1;
	}

	template <typename T>
	constexpr vector4_t<T> operator >> (const vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		vector4_t<T> result = {};
		result.x = v1.x >> v2.x;
		result.y = v1.y >> v2.y;
		result.z = v1.z >> v2.z;
		result.w = v1.w >> v2.w;
		return result;
	}

	template <typename T>
	constexpr vector4_t<T> operator >> (const vector4_t<T>& v1, const T& scalar)
	{
		vector4_t<T> result = {};
		result.x = v1.x >> scalar;
		result.y = v1.y >> scalar;
		result.z = v1.z >> scalar;
		result.w = v1.w >> scalar;
		return result;
	}

	template <typename T>
	vector4_t<T>& operator >>= (vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		v1.x = v1.x >> v2.x;
		v1.y = v1.y >> v2.y;
		v1.z = v1.z >> v2.z;
		v1.w = v1.w >> v2.w;
		return v1;
	}

	template <typename T>
	vector4_t<T>& operator >>= (vector4_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x >> scalar;
		v1.y = v1.y >> scalar;
		v1.z = v1.z >> scalar;
		v1.w = v1.w >> scalar;
		return v1;
	}

	template <typename T>
	vector4_t<T> abs (const vector4_t<T>& v)
	{
		vector4_t<T> result = {};
		result.x = abs(v.x);
		result.y = abs(v.y);
		result.z = abs(v.z);
		result.w = abs(v.w);
		return result;
	}

	template <typename T>
	vector4_t<T> radians (const vector4_t<T>& v)
	{
		vector4_t<T> result = {};
		result.x = radians(v.x);
		result.y = radians(v.y);
		result.z = radians(v.z);
		result.w = radians(v.w);
		return result;
	}

	template <typename T>
	vector4_t<T> degrees (const vector4_t<T>& v)
	{
		vector4_t<T> result = {};
		result.x = degrees(v.x);
		result.y = degrees(v.y);
		result.z = degrees(v.z);
		result.w = degrees(v.w);
		return result;
	}

	template <typename T>
	vector4_t<T> sin (const vector4_t<T>& v)
	{
		vector4_t<T> result = {};
		result.x = sin(v.x);
		result.y = sin(v.y);
		result.z = sin(v.z);
		result.w = sin(v.w);
		return result;
	}

	template <typename T>
	vector4_t<T> cos (const vector4_t<T>& v)
	{
		vector4_t<T> result = {};
		result.x = cos(v.x);
		result.y = cos(v.y);
		result.z = cos(v.z);
		result.w = cos(v.w);
		return result;
	}

	template <typename T>
	vector4_t<T> tan (const vector4_t<T>& v)
	{
		vector4_t<T> result = {};
		result.x = tan(v.x);
		result.y = tan(v.y);
		result.z = tan(v.z);
		result.w = tan(v.w);
		return result;
	}

	template <typename T>
	vector4_t<T> acos (const vector4_t<T>& v)
	{
		vector4_t<T> result = {};
		result.x = acos(v.x);
		result.y = acos(v.y);
		result.z = acos(v.z);
		result.w = acos(v.w);
		return result;
	}

	template <typename T>
	vector4_t<T> asin (const vector4_t<T>& v)
	{
		vector4_t<T> result = {};
		result.x = asin(v.x);
		result.y = asin(v.y);
		result.z = asin(v.z);
		result.w = asin(v.w);
		return result;
	}

	template <typename T>
	vector4_t<T> min (const vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		vector4_t<T> result = {};
		result.x = min(v1.x, v2.x);
		result.y = min(v1.y, v2.y);
		result.z = min(v1.z, v2.z);
		result.w = min(v1.w, v2.w);
		return result;
	}

	template <typename T>
	vector4_t<T> max (const vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		vector4_t<T> result = {};
		result.x = max(v1.x, v2.x);
		result.y = max(v1.y, v2.y);
		result.z = max(v1.z, v2.z);
		result.w = max(v1.w, v2.w);
		return result;
	}

	template <typename T>
	vector4_t<T> atan2 (const vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		vector4_t<T> result = {};
		result.x = atan2(v1.x, v2.x);
		result.y = atan2(v1.y, v2.y);
		result.z = atan2(v1.z, v2.z);
		result.w = atan2(v1.w, v2.w);
		return result;
	}

	template <typename T>
	T horizontal_sum(const vector4_t<T>& v)
	{
		T result = v.x + v.y + v.z + v.w;
		return result;
	}

	template <typename T>
	T dot(const vector4_t<T>& v1, const vector4_t<T>& v2)
	{
		const vector4_t<T>& vv = v1 * v2;
		const T result = horizontal_sum(vv);
		return result;
	}

	template <typename T>
	T squared_length(const vector4_t<T>& v)
	{
		const T slen = dot(v, v);
		return slen;
	}

	template <typename T, typename RT=float>
	RT length(const vector4_t<T>& v)
	{
		const T slen = dot(v, v);
		const RT len = std::sqrt(slen);
		return len;
	}

	template <typename T, typename LT=float>
	vector4_t<T> normalize(const vector4_t<T>& v)
	{
		const LT len = length<LT>(v);
		const vector4_t<T> norm = v / len;
		return norm;
	}

}
#endif