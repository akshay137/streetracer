
#pragma once
#ifndef KATHA_MATH_VECTOR_3_AUTO_GEN_H__
#define KATHA_MATH_VECTOR_3_AUTO_GEN_H__ 1

/*
	This is file is auto generated from `vector_op_gen.py`
	found at {root}/tools/vector_op_gen.py
*/

#include "../core/types.hpp"

#include <cmath>

namespace katha
{

	template <typename T>
	constexpr vector3_t<T> operator - (const vector3_t<T>& v1)
	{
		vector3_t<T> result = {};
		result.x = -v1.x;
		result.y = -v1.y;
		result.z = -v1.z;
		return result;
	}

	template <typename T>
	constexpr vector3_t<T> operator ~ (const vector3_t<T>& v1)
	{
		vector3_t<T> result = {};
		result.x = ~v1.x;
		result.y = ~v1.y;
		result.z = ~v1.z;
		return result;
	}

	template <typename T>
	constexpr vector3_t<T> operator + (const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		vector3_t<T> result = {};
		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z + v2.z;
		return result;
	}

	template <typename T>
	constexpr vector3_t<T> operator + (const vector3_t<T>& v1, const T& scalar)
	{
		vector3_t<T> result = {};
		result.x = v1.x + scalar;
		result.y = v1.y + scalar;
		result.z = v1.z + scalar;
		return result;
	}

	template <typename T>
	vector3_t<T>& operator += (vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		v1.x = v1.x + v2.x;
		v1.y = v1.y + v2.y;
		v1.z = v1.z + v2.z;
		return v1;
	}

	template <typename T>
	vector3_t<T>& operator += (vector3_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x + scalar;
		v1.y = v1.y + scalar;
		v1.z = v1.z + scalar;
		return v1;
	}

	template <typename T>
	constexpr vector3_t<T> operator - (const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		vector3_t<T> result = {};
		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		result.z = v1.z - v2.z;
		return result;
	}

	template <typename T>
	constexpr vector3_t<T> operator - (const vector3_t<T>& v1, const T& scalar)
	{
		vector3_t<T> result = {};
		result.x = v1.x - scalar;
		result.y = v1.y - scalar;
		result.z = v1.z - scalar;
		return result;
	}

	template <typename T>
	vector3_t<T>& operator -= (vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		v1.x = v1.x - v2.x;
		v1.y = v1.y - v2.y;
		v1.z = v1.z - v2.z;
		return v1;
	}

	template <typename T>
	vector3_t<T>& operator -= (vector3_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x - scalar;
		v1.y = v1.y - scalar;
		v1.z = v1.z - scalar;
		return v1;
	}

	template <typename T>
	constexpr vector3_t<T> operator * (const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		vector3_t<T> result = {};
		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;
		result.z = v1.z * v2.z;
		return result;
	}

	template <typename T>
	constexpr vector3_t<T> operator * (const vector3_t<T>& v1, const T& scalar)
	{
		vector3_t<T> result = {};
		result.x = v1.x * scalar;
		result.y = v1.y * scalar;
		result.z = v1.z * scalar;
		return result;
	}

	template <typename T>
	vector3_t<T>& operator *= (vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		v1.x = v1.x * v2.x;
		v1.y = v1.y * v2.y;
		v1.z = v1.z * v2.z;
		return v1;
	}

	template <typename T>
	vector3_t<T>& operator *= (vector3_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x * scalar;
		v1.y = v1.y * scalar;
		v1.z = v1.z * scalar;
		return v1;
	}

	template <typename T>
	constexpr vector3_t<T> operator / (const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		vector3_t<T> result = {};
		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;
		result.z = v1.z / v2.z;
		return result;
	}

	template <typename T>
	constexpr vector3_t<T> operator / (const vector3_t<T>& v1, const T& scalar)
	{
		vector3_t<T> result = {};
		result.x = v1.x / scalar;
		result.y = v1.y / scalar;
		result.z = v1.z / scalar;
		return result;
	}

	template <typename T>
	vector3_t<T>& operator /= (vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		v1.x = v1.x / v2.x;
		v1.y = v1.y / v2.y;
		v1.z = v1.z / v2.z;
		return v1;
	}

	template <typename T>
	vector3_t<T>& operator /= (vector3_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x / scalar;
		v1.y = v1.y / scalar;
		v1.z = v1.z / scalar;
		return v1;
	}

	template <typename T>
	constexpr vector3_t<T> operator % (const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		vector3_t<T> result = {};
		result.x = v1.x % v2.x;
		result.y = v1.y % v2.y;
		result.z = v1.z % v2.z;
		return result;
	}

	template <typename T>
	constexpr vector3_t<T> operator % (const vector3_t<T>& v1, const T& scalar)
	{
		vector3_t<T> result = {};
		result.x = v1.x % scalar;
		result.y = v1.y % scalar;
		result.z = v1.z % scalar;
		return result;
	}

	template <typename T>
	vector3_t<T>& operator %= (vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		v1.x = v1.x % v2.x;
		v1.y = v1.y % v2.y;
		v1.z = v1.z % v2.z;
		return v1;
	}

	template <typename T>
	vector3_t<T>& operator %= (vector3_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x % scalar;
		v1.y = v1.y % scalar;
		v1.z = v1.z % scalar;
		return v1;
	}

	template <typename T>
	constexpr vector3_t<T> operator | (const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		vector3_t<T> result = {};
		result.x = v1.x | v2.x;
		result.y = v1.y | v2.y;
		result.z = v1.z | v2.z;
		return result;
	}

	template <typename T>
	constexpr vector3_t<T> operator | (const vector3_t<T>& v1, const T& scalar)
	{
		vector3_t<T> result = {};
		result.x = v1.x | scalar;
		result.y = v1.y | scalar;
		result.z = v1.z | scalar;
		return result;
	}

	template <typename T>
	vector3_t<T>& operator |= (vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		v1.x = v1.x | v2.x;
		v1.y = v1.y | v2.y;
		v1.z = v1.z | v2.z;
		return v1;
	}

	template <typename T>
	vector3_t<T>& operator |= (vector3_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x | scalar;
		v1.y = v1.y | scalar;
		v1.z = v1.z | scalar;
		return v1;
	}

	template <typename T>
	constexpr vector3_t<T> operator & (const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		vector3_t<T> result = {};
		result.x = v1.x & v2.x;
		result.y = v1.y & v2.y;
		result.z = v1.z & v2.z;
		return result;
	}

	template <typename T>
	constexpr vector3_t<T> operator & (const vector3_t<T>& v1, const T& scalar)
	{
		vector3_t<T> result = {};
		result.x = v1.x & scalar;
		result.y = v1.y & scalar;
		result.z = v1.z & scalar;
		return result;
	}

	template <typename T>
	vector3_t<T>& operator &= (vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		v1.x = v1.x & v2.x;
		v1.y = v1.y & v2.y;
		v1.z = v1.z & v2.z;
		return v1;
	}

	template <typename T>
	vector3_t<T>& operator &= (vector3_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x & scalar;
		v1.y = v1.y & scalar;
		v1.z = v1.z & scalar;
		return v1;
	}

	template <typename T>
	constexpr vector3_t<T> operator ^ (const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		vector3_t<T> result = {};
		result.x = v1.x ^ v2.x;
		result.y = v1.y ^ v2.y;
		result.z = v1.z ^ v2.z;
		return result;
	}

	template <typename T>
	constexpr vector3_t<T> operator ^ (const vector3_t<T>& v1, const T& scalar)
	{
		vector3_t<T> result = {};
		result.x = v1.x ^ scalar;
		result.y = v1.y ^ scalar;
		result.z = v1.z ^ scalar;
		return result;
	}

	template <typename T>
	vector3_t<T>& operator ^= (vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		v1.x = v1.x ^ v2.x;
		v1.y = v1.y ^ v2.y;
		v1.z = v1.z ^ v2.z;
		return v1;
	}

	template <typename T>
	vector3_t<T>& operator ^= (vector3_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x ^ scalar;
		v1.y = v1.y ^ scalar;
		v1.z = v1.z ^ scalar;
		return v1;
	}

	template <typename T>
	constexpr vector3_t<T> operator << (const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		vector3_t<T> result = {};
		result.x = v1.x << v2.x;
		result.y = v1.y << v2.y;
		result.z = v1.z << v2.z;
		return result;
	}

	template <typename T>
	constexpr vector3_t<T> operator << (const vector3_t<T>& v1, const T& scalar)
	{
		vector3_t<T> result = {};
		result.x = v1.x << scalar;
		result.y = v1.y << scalar;
		result.z = v1.z << scalar;
		return result;
	}

	template <typename T>
	vector3_t<T>& operator <<= (vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		v1.x = v1.x << v2.x;
		v1.y = v1.y << v2.y;
		v1.z = v1.z << v2.z;
		return v1;
	}

	template <typename T>
	vector3_t<T>& operator <<= (vector3_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x << scalar;
		v1.y = v1.y << scalar;
		v1.z = v1.z << scalar;
		return v1;
	}

	template <typename T>
	constexpr vector3_t<T> operator >> (const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		vector3_t<T> result = {};
		result.x = v1.x >> v2.x;
		result.y = v1.y >> v2.y;
		result.z = v1.z >> v2.z;
		return result;
	}

	template <typename T>
	constexpr vector3_t<T> operator >> (const vector3_t<T>& v1, const T& scalar)
	{
		vector3_t<T> result = {};
		result.x = v1.x >> scalar;
		result.y = v1.y >> scalar;
		result.z = v1.z >> scalar;
		return result;
	}

	template <typename T>
	vector3_t<T>& operator >>= (vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		v1.x = v1.x >> v2.x;
		v1.y = v1.y >> v2.y;
		v1.z = v1.z >> v2.z;
		return v1;
	}

	template <typename T>
	vector3_t<T>& operator >>= (vector3_t<T>& v1, const T& scalar)
	{
		v1.x = v1.x >> scalar;
		v1.y = v1.y >> scalar;
		v1.z = v1.z >> scalar;
		return v1;
	}

	template <typename T>
	vector3_t<T> abs (const vector3_t<T>& v)
	{
		vector3_t<T> result = {};
		result.x = abs(v.x);
		result.y = abs(v.y);
		result.z = abs(v.z);
		return result;
	}

	template <typename T>
	vector3_t<T> radians (const vector3_t<T>& v)
	{
		vector3_t<T> result = {};
		result.x = radians(v.x);
		result.y = radians(v.y);
		result.z = radians(v.z);
		return result;
	}

	template <typename T>
	vector3_t<T> degrees (const vector3_t<T>& v)
	{
		vector3_t<T> result = {};
		result.x = degrees(v.x);
		result.y = degrees(v.y);
		result.z = degrees(v.z);
		return result;
	}

	template <typename T>
	vector3_t<T> sin (const vector3_t<T>& v)
	{
		vector3_t<T> result = {};
		result.x = sin(v.x);
		result.y = sin(v.y);
		result.z = sin(v.z);
		return result;
	}

	template <typename T>
	vector3_t<T> cos (const vector3_t<T>& v)
	{
		vector3_t<T> result = {};
		result.x = cos(v.x);
		result.y = cos(v.y);
		result.z = cos(v.z);
		return result;
	}

	template <typename T>
	vector3_t<T> tan (const vector3_t<T>& v)
	{
		vector3_t<T> result = {};
		result.x = tan(v.x);
		result.y = tan(v.y);
		result.z = tan(v.z);
		return result;
	}

	template <typename T>
	vector3_t<T> acos (const vector3_t<T>& v)
	{
		vector3_t<T> result = {};
		result.x = acos(v.x);
		result.y = acos(v.y);
		result.z = acos(v.z);
		return result;
	}

	template <typename T>
	vector3_t<T> asin (const vector3_t<T>& v)
	{
		vector3_t<T> result = {};
		result.x = asin(v.x);
		result.y = asin(v.y);
		result.z = asin(v.z);
		return result;
	}

	template <typename T>
	vector3_t<T> min (const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		vector3_t<T> result = {};
		result.x = min(v1.x, v2.x);
		result.y = min(v1.y, v2.y);
		result.z = min(v1.z, v2.z);
		return result;
	}

	template <typename T>
	vector3_t<T> max (const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		vector3_t<T> result = {};
		result.x = max(v1.x, v2.x);
		result.y = max(v1.y, v2.y);
		result.z = max(v1.z, v2.z);
		return result;
	}

	template <typename T>
	vector3_t<T> atan2 (const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		vector3_t<T> result = {};
		result.x = atan2(v1.x, v2.x);
		result.y = atan2(v1.y, v2.y);
		result.z = atan2(v1.z, v2.z);
		return result;
	}

	template <typename T>
	T horizontal_sum(const vector3_t<T>& v)
	{
		T result = v.x + v.y + v.z;
		return result;
	}

	template <typename T>
	T dot(const vector3_t<T>& v1, const vector3_t<T>& v2)
	{
		const vector3_t<T>& vv = v1 * v2;
		const T result = horizontal_sum(vv);
		return result;
	}

	template <typename T>
	T squared_length(const vector3_t<T>& v)
	{
		const T slen = dot(v, v);
		return slen;
	}

	template <typename T, typename RT=float>
	RT length(const vector3_t<T>& v)
	{
		const T slen = dot(v, v);
		const RT len = std::sqrt(slen);
		return len;
	}

	template <typename T, typename LT=float>
	vector3_t<T> normalize(const vector3_t<T>& v)
	{
		const LT len = length<LT>(v);
		const vector3_t<T> norm = v / len;
		return norm;
	}

}
#endif