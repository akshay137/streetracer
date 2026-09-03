
#pragma once
#ifndef KATHA_MATH_VECTOR_2_AUTO_GEN_H__
#define KATHA_MATH_VECTOR_2_AUTO_GEN_H__ 1

/*
	This is file is auto generated from `vector_op_gen.py`
	found at {root}/tools/vector_op_gen.py
*/

#include "../type/vector.hpp"

#include <cmath>

namespace katha
{

	template <typename T>
	constexpr Vector2<T> operator - (const Vector2<T>& v1)
	{
		Vector2<T> result = {};
		result.x = -v1.x;
		result.y = -v1.y;
		return result;
	}

	template <typename T>
	constexpr Vector2<T> operator ~ (const Vector2<T>& v1)
	{
		Vector2<T> result = {};
		result.x = ~v1.x;
		result.y = ~v1.y;
		return result;
	}

	template <typename T>
	constexpr Vector2<T> operator + (const Vector2<T>& v1, const Vector2<T>& v2)
	{
		Vector2<T> result = {};
		result.x = v1.x + v2.x;
		result.y = v1.y + v2.y;
		return result;
	}

	template <typename T>
	constexpr Vector2<T> operator + (const Vector2<T>& v1, const T& scalar)
	{
		Vector2<T> result = {};
		result.x = v1.x + scalar;
		result.y = v1.y + scalar;
		return result;
	}

	template <typename T>
	Vector2<T>& operator += (Vector2<T>& v1, const Vector2<T>& v2)
	{
		v1.x = v1.x + v2.x;
		v1.y = v1.y + v2.y;
		return v1;
	}

	template <typename T>
	Vector2<T>& operator += (Vector2<T>& v1, const T& scalar)
	{
		v1.x = v1.x + scalar;
		v1.y = v1.y + scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector2<T> operator - (const Vector2<T>& v1, const Vector2<T>& v2)
	{
		Vector2<T> result = {};
		result.x = v1.x - v2.x;
		result.y = v1.y - v2.y;
		return result;
	}

	template <typename T>
	constexpr Vector2<T> operator - (const Vector2<T>& v1, const T& scalar)
	{
		Vector2<T> result = {};
		result.x = v1.x - scalar;
		result.y = v1.y - scalar;
		return result;
	}

	template <typename T>
	Vector2<T>& operator -= (Vector2<T>& v1, const Vector2<T>& v2)
	{
		v1.x = v1.x - v2.x;
		v1.y = v1.y - v2.y;
		return v1;
	}

	template <typename T>
	Vector2<T>& operator -= (Vector2<T>& v1, const T& scalar)
	{
		v1.x = v1.x - scalar;
		v1.y = v1.y - scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector2<T> operator * (const Vector2<T>& v1, const Vector2<T>& v2)
	{
		Vector2<T> result = {};
		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;
		return result;
	}

	template <typename T>
	constexpr Vector2<T> operator * (const Vector2<T>& v1, const T& scalar)
	{
		Vector2<T> result = {};
		result.x = v1.x * scalar;
		result.y = v1.y * scalar;
		return result;
	}

	template <typename T>
	Vector2<T>& operator *= (Vector2<T>& v1, const Vector2<T>& v2)
	{
		v1.x = v1.x * v2.x;
		v1.y = v1.y * v2.y;
		return v1;
	}

	template <typename T>
	Vector2<T>& operator *= (Vector2<T>& v1, const T& scalar)
	{
		v1.x = v1.x * scalar;
		v1.y = v1.y * scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector2<T> operator / (const Vector2<T>& v1, const Vector2<T>& v2)
	{
		Vector2<T> result = {};
		result.x = v1.x / v2.x;
		result.y = v1.y / v2.y;
		return result;
	}

	template <typename T>
	constexpr Vector2<T> operator / (const Vector2<T>& v1, const T& scalar)
	{
		Vector2<T> result = {};
		result.x = v1.x / scalar;
		result.y = v1.y / scalar;
		return result;
	}

	template <typename T>
	Vector2<T>& operator /= (Vector2<T>& v1, const Vector2<T>& v2)
	{
		v1.x = v1.x / v2.x;
		v1.y = v1.y / v2.y;
		return v1;
	}

	template <typename T>
	Vector2<T>& operator /= (Vector2<T>& v1, const T& scalar)
	{
		v1.x = v1.x / scalar;
		v1.y = v1.y / scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector2<T> operator % (const Vector2<T>& v1, const Vector2<T>& v2)
	{
		Vector2<T> result = {};
		result.x = v1.x % v2.x;
		result.y = v1.y % v2.y;
		return result;
	}

	template <typename T>
	constexpr Vector2<T> operator % (const Vector2<T>& v1, const T& scalar)
	{
		Vector2<T> result = {};
		result.x = v1.x % scalar;
		result.y = v1.y % scalar;
		return result;
	}

	template <typename T>
	Vector2<T>& operator %= (Vector2<T>& v1, const Vector2<T>& v2)
	{
		v1.x = v1.x % v2.x;
		v1.y = v1.y % v2.y;
		return v1;
	}

	template <typename T>
	Vector2<T>& operator %= (Vector2<T>& v1, const T& scalar)
	{
		v1.x = v1.x % scalar;
		v1.y = v1.y % scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector2<T> operator | (const Vector2<T>& v1, const Vector2<T>& v2)
	{
		Vector2<T> result = {};
		result.x = v1.x | v2.x;
		result.y = v1.y | v2.y;
		return result;
	}

	template <typename T>
	constexpr Vector2<T> operator | (const Vector2<T>& v1, const T& scalar)
	{
		Vector2<T> result = {};
		result.x = v1.x | scalar;
		result.y = v1.y | scalar;
		return result;
	}

	template <typename T>
	Vector2<T>& operator |= (Vector2<T>& v1, const Vector2<T>& v2)
	{
		v1.x = v1.x | v2.x;
		v1.y = v1.y | v2.y;
		return v1;
	}

	template <typename T>
	Vector2<T>& operator |= (Vector2<T>& v1, const T& scalar)
	{
		v1.x = v1.x | scalar;
		v1.y = v1.y | scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector2<T> operator & (const Vector2<T>& v1, const Vector2<T>& v2)
	{
		Vector2<T> result = {};
		result.x = v1.x & v2.x;
		result.y = v1.y & v2.y;
		return result;
	}

	template <typename T>
	constexpr Vector2<T> operator & (const Vector2<T>& v1, const T& scalar)
	{
		Vector2<T> result = {};
		result.x = v1.x & scalar;
		result.y = v1.y & scalar;
		return result;
	}

	template <typename T>
	Vector2<T>& operator &= (Vector2<T>& v1, const Vector2<T>& v2)
	{
		v1.x = v1.x & v2.x;
		v1.y = v1.y & v2.y;
		return v1;
	}

	template <typename T>
	Vector2<T>& operator &= (Vector2<T>& v1, const T& scalar)
	{
		v1.x = v1.x & scalar;
		v1.y = v1.y & scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector2<T> operator ^ (const Vector2<T>& v1, const Vector2<T>& v2)
	{
		Vector2<T> result = {};
		result.x = v1.x ^ v2.x;
		result.y = v1.y ^ v2.y;
		return result;
	}

	template <typename T>
	constexpr Vector2<T> operator ^ (const Vector2<T>& v1, const T& scalar)
	{
		Vector2<T> result = {};
		result.x = v1.x ^ scalar;
		result.y = v1.y ^ scalar;
		return result;
	}

	template <typename T>
	Vector2<T>& operator ^= (Vector2<T>& v1, const Vector2<T>& v2)
	{
		v1.x = v1.x ^ v2.x;
		v1.y = v1.y ^ v2.y;
		return v1;
	}

	template <typename T>
	Vector2<T>& operator ^= (Vector2<T>& v1, const T& scalar)
	{
		v1.x = v1.x ^ scalar;
		v1.y = v1.y ^ scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector2<T> operator << (const Vector2<T>& v1, const Vector2<T>& v2)
	{
		Vector2<T> result = {};
		result.x = v1.x << v2.x;
		result.y = v1.y << v2.y;
		return result;
	}

	template <typename T>
	constexpr Vector2<T> operator << (const Vector2<T>& v1, const T& scalar)
	{
		Vector2<T> result = {};
		result.x = v1.x << scalar;
		result.y = v1.y << scalar;
		return result;
	}

	template <typename T>
	Vector2<T>& operator <<= (Vector2<T>& v1, const Vector2<T>& v2)
	{
		v1.x = v1.x << v2.x;
		v1.y = v1.y << v2.y;
		return v1;
	}

	template <typename T>
	Vector2<T>& operator <<= (Vector2<T>& v1, const T& scalar)
	{
		v1.x = v1.x << scalar;
		v1.y = v1.y << scalar;
		return v1;
	}

	template <typename T>
	constexpr Vector2<T> operator >> (const Vector2<T>& v1, const Vector2<T>& v2)
	{
		Vector2<T> result = {};
		result.x = v1.x >> v2.x;
		result.y = v1.y >> v2.y;
		return result;
	}

	template <typename T>
	constexpr Vector2<T> operator >> (const Vector2<T>& v1, const T& scalar)
	{
		Vector2<T> result = {};
		result.x = v1.x >> scalar;
		result.y = v1.y >> scalar;
		return result;
	}

	template <typename T>
	Vector2<T>& operator >>= (Vector2<T>& v1, const Vector2<T>& v2)
	{
		v1.x = v1.x >> v2.x;
		v1.y = v1.y >> v2.y;
		return v1;
	}

	template <typename T>
	Vector2<T>& operator >>= (Vector2<T>& v1, const T& scalar)
	{
		v1.x = v1.x >> scalar;
		v1.y = v1.y >> scalar;
		return v1;
	}

	template <typename T>
	Vector2<T> Abs(const Vector2<T>& v)
	{
		Vector2<T> result = {};
		result.x = Abs(v.x);
		result.y = Abs(v.y);
		return result;
	}

	template <typename T>
	Vector2<T> Radians(const Vector2<T>& v)
	{
		Vector2<T> result = {};
		result.x = Radians(v.x);
		result.y = Radians(v.y);
		return result;
	}

	template <typename T>
	Vector2<T> Degrees(const Vector2<T>& v)
	{
		Vector2<T> result = {};
		result.x = Degrees(v.x);
		result.y = Degrees(v.y);
		return result;
	}

	template <typename T>
	Vector2<T> Sin(const Vector2<T>& v)
	{
		Vector2<T> result = {};
		result.x = std::sin(v.x);
		result.y = std::sin(v.y);
		return result;
	}

	template <typename T>
	Vector2<T> Cos(const Vector2<T>& v)
	{
		Vector2<T> result = {};
		result.x = std::cos(v.x);
		result.y = std::cos(v.y);
		return result;
	}

	template <typename T>
	Vector2<T> Tan(const Vector2<T>& v)
	{
		Vector2<T> result = {};
		result.x = std::tan(v.x);
		result.y = std::tan(v.y);
		return result;
	}

	template <typename T>
	Vector2<T> ACos(const Vector2<T>& v)
	{
		Vector2<T> result = {};
		result.x = std::acos(v.x);
		result.y = std::acos(v.y);
		return result;
	}

	template <typename T>
	Vector2<T> ASin(const Vector2<T>& v)
	{
		Vector2<T> result = {};
		result.x = std::asin(v.x);
		result.y = std::asin(v.y);
		return result;
	}

	template <typename T>
	Vector2<T> ATan2(const Vector2<T>& v1, const Vector2<T>& v2)
	{
		Vector2<T> result = {};
		result.x = std::atan2(v1.x, v2.x);
		result.y = std::atan2(v1.y, v2.y);
		return result;
	}

	template <typename T>
	Vector2<T> Min(const Vector2<T>& v1, const Vector2<T>& v2)
	{
		Vector2<T> result = {};
		result.x = Min(v1.x, v2.x);
		result.y = Min(v1.y, v2.y);
		return result;
	}

	template <typename T>
	Vector2<T> Max(const Vector2<T>& v1, const Vector2<T>& v2)
	{
		Vector2<T> result = {};
		result.x = Max(v1.x, v2.x);
		result.y = Max(v1.y, v2.y);
		return result;
	}

	template <typename T>
	T HorizontalSum(const Vector2<T>& v)
	{
		T result = v.x + v.y;
		return result;
	}

	template <typename T>
	T Dot(const Vector2<T>& v1, const Vector2<T>& v2)
	{
		const Vector2<T>& vv = v1 * v2;
		const T result = HorizontalSum(vv);
		return result;
	}

	template <typename T>
	T SquaredLength(const Vector2<T>& v)
	{
		const T squared_length = Dot(v, v);
		return squared_length;
	}

	template <typename T, typename RT=float>
	RT Length(const Vector2<T>& v)
	{
		const T squared_length = Dot(v, v);
		const RT length = std::sqrt(squared_length);
		return length;
	}

	template <typename T, typename LT=float>
	Vector2<T> Normalize(const Vector2<T>& v)
	{
		const LT length = Length<LT>(v);
		const Vector2<T> normalized = v / length;
		return normalized;
	}

}
#endif