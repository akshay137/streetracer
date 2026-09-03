
#pragma once
#ifndef KATHA_MATH_VECTOR_3_AUTO_GEN_H__
#define KATHA_MATH_VECTOR_3_AUTO_GEN_H__ 1

/*
	This is file is auto generated from `vector_op_gen.py`
	found at {root}/tools/vector_op_gen.py
*/

#include "../type/vector.hpp"

#include <cmath>

namespace katha
{

	template <typename T>
	constexpr Vector3<T> operator - (const Vector3<T>& v1)
	{
		Vector3<T> result = {};
		result.x = -v1.x;
		result.y = -v1.y;
		result.z = -v1.z;
		return result;
	}

	template <typename T>
	constexpr Vector3<T> operator ~ (const Vector3<T>& v1)
	{
		Vector3<T> result = {};
		result.x = ~v1.x;
		result.y = ~v1.y;
		result.z = ~v1.z;
		return result;
	}

	template <typename T>
	constexpr Vector3<T> operator + (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		Vector3<T> result = {};
		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z + v2.z;
		return result;
	}

	template <typename T>
	constexpr Vector3<T> operator + (const Vector3<T>& v1, const T& scalar)
	{
		Vector3<T> result = {};
		result.x = v1.x + scalar;
		result.y = v1.y + scalar;
		result.z = v1.z + scalar;
		return result;
	}

	template <typename T>
	Vector3<T>& operator += (Vector3<T>& v1, const Vector3<T>& v2)
	{
		v1.x = v1.x + v2.x;
		v1.y = v1.y + v2.y;
		v1.z = v1.z + v2.z;
		return v1;
	}

	template <typename T>
	Vector3<T>& operator += (Vector3<T>& v1, const T& scalar)
	{
		v1.x = v1.x + scalar;
		v1.y = v1.y + scalar;
		v1.z = v1.z + scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector3<T> operator - (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		Vector3<T> result = {};
		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		result.z = v1.z - v2.z;
		return result;
	}

	template <typename T>
	constexpr Vector3<T> operator - (const Vector3<T>& v1, const T& scalar)
	{
		Vector3<T> result = {};
		result.x = v1.x - scalar;
		result.y = v1.y - scalar;
		result.z = v1.z - scalar;
		return result;
	}

	template <typename T>
	Vector3<T>& operator -= (Vector3<T>& v1, const Vector3<T>& v2)
	{
		v1.x = v1.x - v2.x;
		v1.y = v1.y - v2.y;
		v1.z = v1.z - v2.z;
		return v1;
	}

	template <typename T>
	Vector3<T>& operator -= (Vector3<T>& v1, const T& scalar)
	{
		v1.x = v1.x - scalar;
		v1.y = v1.y - scalar;
		v1.z = v1.z - scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector3<T> operator * (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		Vector3<T> result = {};
		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;
		result.z = v1.z * v2.z;
		return result;
	}

	template <typename T>
	constexpr Vector3<T> operator * (const Vector3<T>& v1, const T& scalar)
	{
		Vector3<T> result = {};
		result.x = v1.x * scalar;
		result.y = v1.y * scalar;
		result.z = v1.z * scalar;
		return result;
	}

	template <typename T>
	Vector3<T>& operator *= (Vector3<T>& v1, const Vector3<T>& v2)
	{
		v1.x = v1.x * v2.x;
		v1.y = v1.y * v2.y;
		v1.z = v1.z * v2.z;
		return v1;
	}

	template <typename T>
	Vector3<T>& operator *= (Vector3<T>& v1, const T& scalar)
	{
		v1.x = v1.x * scalar;
		v1.y = v1.y * scalar;
		v1.z = v1.z * scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector3<T> operator / (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		Vector3<T> result = {};
		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;
		result.z = v1.z / v2.z;
		return result;
	}

	template <typename T>
	constexpr Vector3<T> operator / (const Vector3<T>& v1, const T& scalar)
	{
		Vector3<T> result = {};
		result.x = v1.x / scalar;
		result.y = v1.y / scalar;
		result.z = v1.z / scalar;
		return result;
	}

	template <typename T>
	Vector3<T>& operator /= (Vector3<T>& v1, const Vector3<T>& v2)
	{
		v1.x = v1.x / v2.x;
		v1.y = v1.y / v2.y;
		v1.z = v1.z / v2.z;
		return v1;
	}

	template <typename T>
	Vector3<T>& operator /= (Vector3<T>& v1, const T& scalar)
	{
		v1.x = v1.x / scalar;
		v1.y = v1.y / scalar;
		v1.z = v1.z / scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector3<T> operator % (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		Vector3<T> result = {};
		result.x = v1.x % v2.x;
		result.y = v1.y % v2.y;
		result.z = v1.z % v2.z;
		return result;
	}

	template <typename T>
	constexpr Vector3<T> operator % (const Vector3<T>& v1, const T& scalar)
	{
		Vector3<T> result = {};
		result.x = v1.x % scalar;
		result.y = v1.y % scalar;
		result.z = v1.z % scalar;
		return result;
	}

	template <typename T>
	Vector3<T>& operator %= (Vector3<T>& v1, const Vector3<T>& v2)
	{
		v1.x = v1.x % v2.x;
		v1.y = v1.y % v2.y;
		v1.z = v1.z % v2.z;
		return v1;
	}

	template <typename T>
	Vector3<T>& operator %= (Vector3<T>& v1, const T& scalar)
	{
		v1.x = v1.x % scalar;
		v1.y = v1.y % scalar;
		v1.z = v1.z % scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector3<T> operator | (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		Vector3<T> result = {};
		result.x = v1.x | v2.x;
		result.y = v1.y | v2.y;
		result.z = v1.z | v2.z;
		return result;
	}

	template <typename T>
	constexpr Vector3<T> operator | (const Vector3<T>& v1, const T& scalar)
	{
		Vector3<T> result = {};
		result.x = v1.x | scalar;
		result.y = v1.y | scalar;
		result.z = v1.z | scalar;
		return result;
	}

	template <typename T>
	Vector3<T>& operator |= (Vector3<T>& v1, const Vector3<T>& v2)
	{
		v1.x = v1.x | v2.x;
		v1.y = v1.y | v2.y;
		v1.z = v1.z | v2.z;
		return v1;
	}

	template <typename T>
	Vector3<T>& operator |= (Vector3<T>& v1, const T& scalar)
	{
		v1.x = v1.x | scalar;
		v1.y = v1.y | scalar;
		v1.z = v1.z | scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector3<T> operator & (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		Vector3<T> result = {};
		result.x = v1.x & v2.x;
		result.y = v1.y & v2.y;
		result.z = v1.z & v2.z;
		return result;
	}

	template <typename T>
	constexpr Vector3<T> operator & (const Vector3<T>& v1, const T& scalar)
	{
		Vector3<T> result = {};
		result.x = v1.x & scalar;
		result.y = v1.y & scalar;
		result.z = v1.z & scalar;
		return result;
	}

	template <typename T>
	Vector3<T>& operator &= (Vector3<T>& v1, const Vector3<T>& v2)
	{
		v1.x = v1.x & v2.x;
		v1.y = v1.y & v2.y;
		v1.z = v1.z & v2.z;
		return v1;
	}

	template <typename T>
	Vector3<T>& operator &= (Vector3<T>& v1, const T& scalar)
	{
		v1.x = v1.x & scalar;
		v1.y = v1.y & scalar;
		v1.z = v1.z & scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector3<T> operator ^ (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		Vector3<T> result = {};
		result.x = v1.x ^ v2.x;
		result.y = v1.y ^ v2.y;
		result.z = v1.z ^ v2.z;
		return result;
	}

	template <typename T>
	constexpr Vector3<T> operator ^ (const Vector3<T>& v1, const T& scalar)
	{
		Vector3<T> result = {};
		result.x = v1.x ^ scalar;
		result.y = v1.y ^ scalar;
		result.z = v1.z ^ scalar;
		return result;
	}

	template <typename T>
	Vector3<T>& operator ^= (Vector3<T>& v1, const Vector3<T>& v2)
	{
		v1.x = v1.x ^ v2.x;
		v1.y = v1.y ^ v2.y;
		v1.z = v1.z ^ v2.z;
		return v1;
	}

	template <typename T>
	Vector3<T>& operator ^= (Vector3<T>& v1, const T& scalar)
	{
		v1.x = v1.x ^ scalar;
		v1.y = v1.y ^ scalar;
		v1.z = v1.z ^ scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector3<T> operator << (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		Vector3<T> result = {};
		result.x = v1.x << v2.x;
		result.y = v1.y << v2.y;
		result.z = v1.z << v2.z;
		return result;
	}

	template <typename T>
	constexpr Vector3<T> operator << (const Vector3<T>& v1, const T& scalar)
	{
		Vector3<T> result = {};
		result.x = v1.x << scalar;
		result.y = v1.y << scalar;
		result.z = v1.z << scalar;
		return result;
	}

	template <typename T>
	Vector3<T>& operator <<= (Vector3<T>& v1, const Vector3<T>& v2)
	{
		v1.x = v1.x << v2.x;
		v1.y = v1.y << v2.y;
		v1.z = v1.z << v2.z;
		return v1;
	}

	template <typename T>
	Vector3<T>& operator <<= (Vector3<T>& v1, const T& scalar)
	{
		v1.x = v1.x << scalar;
		v1.y = v1.y << scalar;
		v1.z = v1.z << scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector3<T> operator >> (const Vector3<T>& v1, const Vector3<T>& v2)
	{
		Vector3<T> result = {};
		result.x = v1.x >> v2.x;
		result.y = v1.y >> v2.y;
		result.z = v1.z >> v2.z;
		return result;
	}

	template <typename T>
	constexpr Vector3<T> operator >> (const Vector3<T>& v1, const T& scalar)
	{
		Vector3<T> result = {};
		result.x = v1.x >> scalar;
		result.y = v1.y >> scalar;
		result.z = v1.z >> scalar;
		return result;
	}

	template <typename T>
	Vector3<T>& operator >>= (Vector3<T>& v1, const Vector3<T>& v2)
	{
		v1.x = v1.x >> v2.x;
		v1.y = v1.y >> v2.y;
		v1.z = v1.z >> v2.z;
		return v1;
	}

	template <typename T>
	Vector3<T>& operator >>= (Vector3<T>& v1, const T& scalar)
	{
		v1.x = v1.x >> scalar;
		v1.y = v1.y >> scalar;
		v1.z = v1.z >> scalar;
		return v1;
	}

	template <typename T>
	Vector3<T> Abs(const Vector3<T>& v)
	{
		Vector3<T> result = {};
		result.x = Abs(v.x);
		result.y = Abs(v.y);
		result.z = Abs(v.z);
		return result;
	}

	template <typename T>
	Vector3<T> Radians(const Vector3<T>& v)
	{
		Vector3<T> result = {};
		result.x = Radians(v.x);
		result.y = Radians(v.y);
		result.z = Radians(v.z);
		return result;
	}

	template <typename T>
	Vector3<T> Degrees(const Vector3<T>& v)
	{
		Vector3<T> result = {};
		result.x = Degrees(v.x);
		result.y = Degrees(v.y);
		result.z = Degrees(v.z);
		return result;
	}

	template <typename T>
	Vector3<T> Sin(const Vector3<T>& v)
	{
		Vector3<T> result = {};
		result.x = std::sin(v.x);
		result.y = std::sin(v.y);
		result.z = std::sin(v.z);
		return result;
	}

	template <typename T>
	Vector3<T> Cos(const Vector3<T>& v)
	{
		Vector3<T> result = {};
		result.x = std::cos(v.x);
		result.y = std::cos(v.y);
		result.z = std::cos(v.z);
		return result;
	}

	template <typename T>
	Vector3<T> Tan(const Vector3<T>& v)
	{
		Vector3<T> result = {};
		result.x = std::tan(v.x);
		result.y = std::tan(v.y);
		result.z = std::tan(v.z);
		return result;
	}

	template <typename T>
	Vector3<T> ACos(const Vector3<T>& v)
	{
		Vector3<T> result = {};
		result.x = std::acos(v.x);
		result.y = std::acos(v.y);
		result.z = std::acos(v.z);
		return result;
	}

	template <typename T>
	Vector3<T> ASin(const Vector3<T>& v)
	{
		Vector3<T> result = {};
		result.x = std::asin(v.x);
		result.y = std::asin(v.y);
		result.z = std::asin(v.z);
		return result;
	}

	template <typename T>
	Vector3<T> ATan2(const Vector3<T>& v1, const Vector3<T>& v2)
	{
		Vector3<T> result = {};
		result.x = std::atan2(v1.x, v2.x);
		result.y = std::atan2(v1.y, v2.y);
		result.z = std::atan2(v1.z, v2.z);
		return result;
	}

	template <typename T>
	Vector3<T> Min(const Vector3<T>& v1, const Vector3<T>& v2)
	{
		Vector3<T> result = {};
		result.x = Min(v1.x, v2.x);
		result.y = Min(v1.y, v2.y);
		result.z = Min(v1.z, v2.z);
		return result;
	}

	template <typename T>
	Vector3<T> Max(const Vector3<T>& v1, const Vector3<T>& v2)
	{
		Vector3<T> result = {};
		result.x = Max(v1.x, v2.x);
		result.y = Max(v1.y, v2.y);
		result.z = Max(v1.z, v2.z);
		return result;
	}

	template <typename T>
	T HorizontalSum(const Vector3<T>& v)
	{
		T result = v.x + v.y + v.z;
		return result;
	}

	template <typename T>
	T Dot(const Vector3<T>& v1, const Vector3<T>& v2)
	{
		const Vector3<T>& vv = v1 * v2;
		const T result = HorizontalSum(vv);
		return result;
	}

	template <typename T>
	T SquaredLength(const Vector3<T>& v)
	{
		const T squared_length = Dot(v, v);
		return squared_length;
	}

	template <typename T, typename RT=float>
	RT Length(const Vector3<T>& v)
	{
		const T squared_length = Dot(v, v);
		const RT length = std::sqrt(squared_length);
		return length;
	}

	template <typename T, typename LT=float>
	Vector3<T> Normalize(const Vector3<T>& v)
	{
		const LT length = Length<LT>(v);
		const Vector3<T> normalized = v / length;
		return normalized;
	}

}
#endif