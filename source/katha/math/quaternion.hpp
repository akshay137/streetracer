#pragma once
#ifndef KATHA_MATH_QUATERNION_H__
#define KATHA_MATH_QUATERNION_H__ 1

#include "../core/types.hpp"
#include "vector3.hpp"

#include <cmath>

namespace katha
{
	template <typename T>
	quaternion_t<T> from_euler(const vector3_t<T>& euler)
	{
		const vector3_t<T> half_euler = euler * 0.5f;
		const vector3_t<T> s = sine(half_euler);
		const vector3_t<T> c = cosine(half_euler);

		quaternion_t<T> result = {};
		result.x = (s.x * c.y * c.z) - (c.x * s.y * s.z);
		result.y = (c.x * s.y * c.z) + (s.x * c.y * s.z);
		result.z = (c.x * c.y * s.z) - (s.x * s.y * c.z);
		result.w = horizontal_product(c) + horizontal_product(s);
		return result;
	}

	template <typename T>
	vector3_t<T> to_euler(const quaternion_t<T>& q)
	{
		vector3_t<T> result;
		result.x = std::atan2(2 * ((q.w * q.x) + (q.y * q.z)), 1 - 2 * (q.x * q.x + q.y * q.y));
		result.y = std::asin(clamp<T>(2 * ((q.w * q.y) - (q.z * q.x)), -1, 1));
		result.z = std::atan2(2 * ((q.w * q.z) + (q.x * q.y)), 1 - 2 * (q.y * q.y + q.z * q.z));
		return result;
	}

	template <typename T>
	quaternion_t<T> operator * (const quaternion_t<T>& q1, const quaternion_t<T>& q2)
	{
		quaternion_t<T> result = {};
		const vector3_t<T> q1v = q1.vector();
		const vector3_t<T> q2v = q2.vector();
		const vector3_t<T> v
			= q1v * q2.w
			+ q2v * q1.w
			+ cross(q1v, q2v);
		result.vector(v);
		
		const T scalar = (q1.w * q2.w) - dot(q1v, q2v);
		result.scalar(scalar);

		return result;
	}

	template <typename T>
	quaternion_t<T> normalize(const quaternion_t<T>& q)
	{
		const T squared_norm = q.x * q.x
			+ q.y * q.y
			+ q.z * q.z
			+ q.w * q.w;
		const T norm = std::sqrt(squared_norm);
		quaternion_t<T> result(
			q.x / norm,
			q.y / norm,
			q.z / norm,
			q.w / norm
		);
		return result;
	}

	template <typename T>
	quaternion_t<T> conjugate(const quaternion_t<T>& q)
	{
		quaternion_t<T> result(-q.x, -q.y, -q.z, q.w);
		return result;
	}

	template <typename T>
	quaternion_t<T> inverse(const quaternion_t<T>& q)
	{
		const T squared_norm = q.x * q.x
			+ q.y * q.y
			+ q.z * q.z
			+ q.w * q.w;
		quaternion_t<T> result(
			-q.x / squared_norm,
			-q.y / squared_norm,
			-q.z / squared_norm,
			q.w / squared_norm
		);
		return result;
	}

	template <typename T>
	vector3_t<T> rotate(const quaternion_t<T>& q, const vector3_t<T>& v)
	{
		vector3_t<T> qv = q.vector();
		vector3_t<T> uv = cross(qv, v);
		vector3_t<T> uuv = cross(qv, uv);

		vector3_t<T> result = v + ((uv * q.scalar()) + uuv) * 2.0f;
		return result;
	}

	template <typename T>
	matrix4_t<T> to_mat4(const quaternion_t<T>& q)
	{
		matrix4_t result(1.0f);

		const T qxx = q.x * q.x;
		const T qyy = q.y * q.y;
		const T qzz = q.z * q.z;

		const T qxy = q.x * q.y;
		const T qxz = q.x * q.z;
		const T qyz = q.y * q.z;

		const T qwx = q.w * q.x;
		const T qwy = q.w * q.y;
		const T qwz = q.w * q.z;

		constexpr T one = 1;
		constexpr T two = 2;
		result[0][0] = one - two * (qyy + qzz);
		result[0][1] = two * (qxy + qwz);
		result[0][2] = two * (qwz - qwy);

		result[1][0] = two * (qxy - qwz);
		result[1][1] = one - two * (qxx + qzz);
		result[1][2] = two * (qyz + qwx);

		result[2][0] = two * (qxz + qwy);
		result[2][1] = two * (qyz - qwx);
		result[2][2] = one - two * (qxx + qyy);

		return result;
	}
}

#endif