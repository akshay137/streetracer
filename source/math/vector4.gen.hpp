
#pragma once
#ifndef KATHA_MATH_VECTOR_4_AUTO_GEN_H__
#define KATHA_MATH_VECTOR_4_AUTO_GEN_H__ 1

/*
	This is file is auto generated from `vector_op_gen.py`
	found at {root}/tools/vector_op_gen.py
*/

#include "../type/vector.hpp"

#include <cmath>

namespace katha
{

	template <typename T>
	constexpr Vector4<T> operator - (const Vector4<T>& v1)
	{
		Vector4<T> result = {};
		result.x = -v1.x;
		result.y = -v1.y;
		result.z = -v1.z;
		result.w = -v1.w;
		return result;
	}

	template <typename T>
	constexpr Vector4<T> operator ~ (const Vector4<T>& v1)
	{
		Vector4<T> result = {};
		result.x = ~v1.x;
		result.y = ~v1.y;
		result.z = ~v1.z;
		result.w = ~v1.w;
		return result;
	}

	template <typename T>
	constexpr Vector4<T> operator + (const Vector4<T>& v1, const Vector4<T>& v2)
	{
		Vector4<T> result = {};
		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		result.z = v1.z + v2.z;
		result.w = v1.w + v2.w;
		return result;
	}

	template <typename T>
	constexpr Vector4<T> operator + (const Vector4<T>& v1, const T& scalar)
	{
		Vector4<T> result = {};
		result.x = v1.x + scalar;
		result.y = v1.y + scalar;
		result.z = v1.z + scalar;
		result.w = v1.w + scalar;
		return result;
	}

	template <typename T>
	Vector4<T>& operator += (Vector4<T>& v1, const Vector4<T>& v2)
	{
		v1.x = v1.x + v2.x;
		v1.y = v1.y + v2.y;
		v1.z = v1.z + v2.z;
		v1.w = v1.w + v2.w;
		return v1;
	}

	template <typename T>
	Vector4<T>& operator += (Vector4<T>& v1, const T& scalar)
	{
		v1.x = v1.x + scalar;
		v1.y = v1.y + scalar;
		v1.z = v1.z + scalar;
		v1.w = v1.w + scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector4<T> operator - (const Vector4<T>& v1, const Vector4<T>& v2)
	{
		Vector4<T> result = {};
		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		result.z = v1.z - v2.z;
		result.w = v1.w - v2.w;
		return result;
	}

	template <typename T>
	constexpr Vector4<T> operator - (const Vector4<T>& v1, const T& scalar)
	{
		Vector4<T> result = {};
		result.x = v1.x - scalar;
		result.y = v1.y - scalar;
		result.z = v1.z - scalar;
		result.w = v1.w - scalar;
		return result;
	}

	template <typename T>
	Vector4<T>& operator -= (Vector4<T>& v1, const Vector4<T>& v2)
	{
		v1.x = v1.x - v2.x;
		v1.y = v1.y - v2.y;
		v1.z = v1.z - v2.z;
		v1.w = v1.w - v2.w;
		return v1;
	}

	template <typename T>
	Vector4<T>& operator -= (Vector4<T>& v1, const T& scalar)
	{
		v1.x = v1.x - scalar;
		v1.y = v1.y - scalar;
		v1.z = v1.z - scalar;
		v1.w = v1.w - scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector4<T> operator * (const Vector4<T>& v1, const Vector4<T>& v2)
	{
		Vector4<T> result = {};
		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;
		result.z = v1.z * v2.z;
		result.w = v1.w * v2.w;
		return result;
	}

	template <typename T>
	constexpr Vector4<T> operator * (const Vector4<T>& v1, const T& scalar)
	{
		Vector4<T> result = {};
		result.x = v1.x * scalar;
		result.y = v1.y * scalar;
		result.z = v1.z * scalar;
		result.w = v1.w * scalar;
		return result;
	}

	template <typename T>
	Vector4<T>& operator *= (Vector4<T>& v1, const Vector4<T>& v2)
	{
		v1.x = v1.x * v2.x;
		v1.y = v1.y * v2.y;
		v1.z = v1.z * v2.z;
		v1.w = v1.w * v2.w;
		return v1;
	}

	template <typename T>
	Vector4<T>& operator *= (Vector4<T>& v1, const T& scalar)
	{
		v1.x = v1.x * scalar;
		v1.y = v1.y * scalar;
		v1.z = v1.z * scalar;
		v1.w = v1.w * scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector4<T> operator / (const Vector4<T>& v1, const Vector4<T>& v2)
	{
		Vector4<T> result = {};
		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;
		result.z = v1.z / v2.z;
		result.w = v1.w / v2.w;
		return result;
	}

	template <typename T>
	constexpr Vector4<T> operator / (const Vector4<T>& v1, const T& scalar)
	{
		Vector4<T> result = {};
		result.x = v1.x / scalar;
		result.y = v1.y / scalar;
		result.z = v1.z / scalar;
		result.w = v1.w / scalar;
		return result;
	}

	template <typename T>
	Vector4<T>& operator /= (Vector4<T>& v1, const Vector4<T>& v2)
	{
		v1.x = v1.x / v2.x;
		v1.y = v1.y / v2.y;
		v1.z = v1.z / v2.z;
		v1.w = v1.w / v2.w;
		return v1;
	}

	template <typename T>
	Vector4<T>& operator /= (Vector4<T>& v1, const T& scalar)
	{
		v1.x = v1.x / scalar;
		v1.y = v1.y / scalar;
		v1.z = v1.z / scalar;
		v1.w = v1.w / scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector4<T> operator % (const Vector4<T>& v1, const Vector4<T>& v2)
	{
		Vector4<T> result = {};
		result.x = v1.x % v2.x;
		result.y = v1.y % v2.y;
		result.z = v1.z % v2.z;
		result.w = v1.w % v2.w;
		return result;
	}

	template <typename T>
	constexpr Vector4<T> operator % (const Vector4<T>& v1, const T& scalar)
	{
		Vector4<T> result = {};
		result.x = v1.x % scalar;
		result.y = v1.y % scalar;
		result.z = v1.z % scalar;
		result.w = v1.w % scalar;
		return result;
	}

	template <typename T>
	Vector4<T>& operator %= (Vector4<T>& v1, const Vector4<T>& v2)
	{
		v1.x = v1.x % v2.x;
		v1.y = v1.y % v2.y;
		v1.z = v1.z % v2.z;
		v1.w = v1.w % v2.w;
		return v1;
	}

	template <typename T>
	Vector4<T>& operator %= (Vector4<T>& v1, const T& scalar)
	{
		v1.x = v1.x % scalar;
		v1.y = v1.y % scalar;
		v1.z = v1.z % scalar;
		v1.w = v1.w % scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector4<T> operator | (const Vector4<T>& v1, const Vector4<T>& v2)
	{
		Vector4<T> result = {};
		result.x = v1.x | v2.x;
		result.y = v1.y | v2.y;
		result.z = v1.z | v2.z;
		result.w = v1.w | v2.w;
		return result;
	}

	template <typename T>
	constexpr Vector4<T> operator | (const Vector4<T>& v1, const T& scalar)
	{
		Vector4<T> result = {};
		result.x = v1.x | scalar;
		result.y = v1.y | scalar;
		result.z = v1.z | scalar;
		result.w = v1.w | scalar;
		return result;
	}

	template <typename T>
	Vector4<T>& operator |= (Vector4<T>& v1, const Vector4<T>& v2)
	{
		v1.x = v1.x | v2.x;
		v1.y = v1.y | v2.y;
		v1.z = v1.z | v2.z;
		v1.w = v1.w | v2.w;
		return v1;
	}

	template <typename T>
	Vector4<T>& operator |= (Vector4<T>& v1, const T& scalar)
	{
		v1.x = v1.x | scalar;
		v1.y = v1.y | scalar;
		v1.z = v1.z | scalar;
		v1.w = v1.w | scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector4<T> operator & (const Vector4<T>& v1, const Vector4<T>& v2)
	{
		Vector4<T> result = {};
		result.x = v1.x & v2.x;
		result.y = v1.y & v2.y;
		result.z = v1.z & v2.z;
		result.w = v1.w & v2.w;
		return result;
	}

	template <typename T>
	constexpr Vector4<T> operator & (const Vector4<T>& v1, const T& scalar)
	{
		Vector4<T> result = {};
		result.x = v1.x & scalar;
		result.y = v1.y & scalar;
		result.z = v1.z & scalar;
		result.w = v1.w & scalar;
		return result;
	}

	template <typename T>
	Vector4<T>& operator &= (Vector4<T>& v1, const Vector4<T>& v2)
	{
		v1.x = v1.x & v2.x;
		v1.y = v1.y & v2.y;
		v1.z = v1.z & v2.z;
		v1.w = v1.w & v2.w;
		return v1;
	}

	template <typename T>
	Vector4<T>& operator &= (Vector4<T>& v1, const T& scalar)
	{
		v1.x = v1.x & scalar;
		v1.y = v1.y & scalar;
		v1.z = v1.z & scalar;
		v1.w = v1.w & scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector4<T> operator ^ (const Vector4<T>& v1, const Vector4<T>& v2)
	{
		Vector4<T> result = {};
		result.x = v1.x ^ v2.x;
		result.y = v1.y ^ v2.y;
		result.z = v1.z ^ v2.z;
		result.w = v1.w ^ v2.w;
		return result;
	}

	template <typename T>
	constexpr Vector4<T> operator ^ (const Vector4<T>& v1, const T& scalar)
	{
		Vector4<T> result = {};
		result.x = v1.x ^ scalar;
		result.y = v1.y ^ scalar;
		result.z = v1.z ^ scalar;
		result.w = v1.w ^ scalar;
		return result;
	}

	template <typename T>
	Vector4<T>& operator ^= (Vector4<T>& v1, const Vector4<T>& v2)
	{
		v1.x = v1.x ^ v2.x;
		v1.y = v1.y ^ v2.y;
		v1.z = v1.z ^ v2.z;
		v1.w = v1.w ^ v2.w;
		return v1;
	}

	template <typename T>
	Vector4<T>& operator ^= (Vector4<T>& v1, const T& scalar)
	{
		v1.x = v1.x ^ scalar;
		v1.y = v1.y ^ scalar;
		v1.z = v1.z ^ scalar;
		v1.w = v1.w ^ scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector4<T> operator << (const Vector4<T>& v1, const Vector4<T>& v2)
	{
		Vector4<T> result = {};
		result.x = v1.x << v2.x;
		result.y = v1.y << v2.y;
		result.z = v1.z << v2.z;
		result.w = v1.w << v2.w;
		return result;
	}

	template <typename T>
	constexpr Vector4<T> operator << (const Vector4<T>& v1, const T& scalar)
	{
		Vector4<T> result = {};
		result.x = v1.x << scalar;
		result.y = v1.y << scalar;
		result.z = v1.z << scalar;
		result.w = v1.w << scalar;
		return result;
	}

	template <typename T>
	Vector4<T>& operator <<= (Vector4<T>& v1, const Vector4<T>& v2)
	{
		v1.x = v1.x << v2.x;
		v1.y = v1.y << v2.y;
		v1.z = v1.z << v2.z;
		v1.w = v1.w << v2.w;
		return v1;
	}

	template <typename T>
	Vector4<T>& operator <<= (Vector4<T>& v1, const T& scalar)
	{
		v1.x = v1.x << scalar;
		v1.y = v1.y << scalar;
		v1.z = v1.z << scalar;
		v1.w = v1.w << scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector4<T> operator >> (const Vector4<T>& v1, const Vector4<T>& v2)
	{
		Vector4<T> result = {};
		result.x = v1.x >> v2.x;
		result.y = v1.y >> v2.y;
		result.z = v1.z >> v2.z;
		result.w = v1.w >> v2.w;
		return result;
	}

	template <typename T>
	constexpr Vector4<T> operator >> (const Vector4<T>& v1, const T& scalar)
	{
		Vector4<T> result = {};
		result.x = v1.x >> scalar;
		result.y = v1.y >> scalar;
		result.z = v1.z >> scalar;
		result.w = v1.w >> scalar;
		return result;
	}

	template <typename T>
	Vector4<T>& operator >>= (Vector4<T>& v1, const Vector4<T>& v2)
	{
		v1.x = v1.x >> v2.x;
		v1.y = v1.y >> v2.y;
		v1.z = v1.z >> v2.z;
		v1.w = v1.w >> v2.w;
		return v1;
	}

	template <typename T>
	Vector4<T>& operator >>= (Vector4<T>& v1, const T& scalar)
	{
		v1.x = v1.x >> scalar;
		v1.y = v1.y >> scalar;
		v1.z = v1.z >> scalar;
		v1.w = v1.w >> scalar;
		return v1;
	}

	template <typename T>
	Vector4<T> Abs(const Vector4<T>& v)
	{
		Vector4<T> result = {};
		result.x = Abs(v.x);
		result.y = Abs(v.y);
		result.z = Abs(v.z);
		result.w = Abs(v.w);
		return result;
	}

	template <typename T>
	Vector4<T> Radians(const Vector4<T>& v)
	{
		Vector4<T> result = {};
		result.x = Radians(v.x);
		result.y = Radians(v.y);
		result.z = Radians(v.z);
		result.w = Radians(v.w);
		return result;
	}

	template <typename T>
	Vector4<T> Degrees(const Vector4<T>& v)
	{
		Vector4<T> result = {};
		result.x = Degrees(v.x);
		result.y = Degrees(v.y);
		result.z = Degrees(v.z);
		result.w = Degrees(v.w);
		return result;
	}

	template <typename T>
	Vector4<T> Sin(const Vector4<T>& v)
	{
		Vector4<T> result = {};
		result.x = std::sin(v.x);
		result.y = std::sin(v.y);
		result.z = std::sin(v.z);
		result.w = std::sin(v.w);
		return result;
	}

	template <typename T>
	Vector4<T> Cos(const Vector4<T>& v)
	{
		Vector4<T> result = {};
		result.x = std::cos(v.x);
		result.y = std::cos(v.y);
		result.z = std::cos(v.z);
		result.w = std::cos(v.w);
		return result;
	}

	template <typename T>
	Vector4<T> Tan(const Vector4<T>& v)
	{
		Vector4<T> result = {};
		result.x = std::tan(v.x);
		result.y = std::tan(v.y);
		result.z = std::tan(v.z);
		result.w = std::tan(v.w);
		return result;
	}

	template <typename T>
	Vector4<T> ACos(const Vector4<T>& v)
	{
		Vector4<T> result = {};
		result.x = std::acos(v.x);
		result.y = std::acos(v.y);
		result.z = std::acos(v.z);
		result.w = std::acos(v.w);
		return result;
	}

	template <typename T>
	Vector4<T> ASin(const Vector4<T>& v)
	{
		Vector4<T> result = {};
		result.x = std::asin(v.x);
		result.y = std::asin(v.y);
		result.z = std::asin(v.z);
		result.w = std::asin(v.w);
		return result;
	}

	template <typename T>
	Vector4<T> ATan2(const Vector4<T>& v1, const Vector4<T>& v2)
	{
		Vector4<T> result = {};
		result.x = std::atan2(v1.x, v2.x);
		result.y = std::atan2(v1.y, v2.y);
		result.z = std::atan2(v1.z, v2.z);
		result.w = std::atan2(v1.w, v2.w);
		return result;
	}

	template <typename T>
	Vector4<T> Min(const Vector4<T>& v1, const Vector4<T>& v2)
	{
		Vector4<T> result = {};
		result.x = Min(v1.x, v2.x);
		result.y = Min(v1.y, v2.y);
		result.z = Min(v1.z, v2.z);
		result.w = Min(v1.w, v2.w);
		return result;
	}

	template <typename T>
	Vector4<T> Max(const Vector4<T>& v1, const Vector4<T>& v2)
	{
		Vector4<T> result = {};
		result.x = Max(v1.x, v2.x);
		result.y = Max(v1.y, v2.y);
		result.z = Max(v1.z, v2.z);
		result.w = Max(v1.w, v2.w);
		return result;
	}

	template <typename T>
	T HorizontalSum(const Vector4<T>& v)
	{
		T result = v.x + v.y + v.z + v.w;
		return result;
	}

	template <typename T>
	T Dot(const Vector4<T>& v1, const Vector4<T>& v2)
	{
		const Vector4<T>& vv = v1 * v2;
		const T result = HorizontalSum(vv);
		return result;
	}

	template <typename T>
	T SquaredLength(const Vector4<T>& v)
	{
		const T squared_length = Dot(v, v);
		return squared_length;
	}

	template <typename T, typename RT=float>
	RT Length(const Vector4<T>& v)
	{
		const T squared_length = Dot(v, v);
		const RT length = std::sqrt(squared_length);
		return length;
	}

	template <typename T, typename LT=float>
	Vector4<T> Normalize(const Vector4<T>& v)
	{
		const LT length = Length<LT>(v);
		const Vector4<T> normalized = v / length;
		return normalized;
	}

}
#endif