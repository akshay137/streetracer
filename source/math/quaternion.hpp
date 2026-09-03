#pragma once
#ifndef KATHA_MATH_QUATERNION_H__
#define KATHA_MATH_QUATERNION_H__ 1

#include "../type/quaternion.hpp"
#include "vector3.hpp"

#include <cmath>

namespace katha
{
	template <typename T>
	Quaternion<T> operator * (const Quaternion<T>& q1, const Quaternion<T>& q2)
	{
		Quaternion<T> result = {};
		const Vector3<T> q1v = q1.vector();
		const Vector3<T> q2v = q2.vector();
		const Vector3<T> v
			= q1v * q2.w
			+ q2v * q1.w
			+ Cross(q1v, q2v);
		result.vector(v);
		
		const T scalar = (q1.w * q2.w) - Dot(q1v, q2v);
		result.scalar(scalar);

		return result;
	}

	template <typename T>
	Quaternion<T> Normalize(const Quaternion<T>& q)
	{
		const T squared_norm = q.x * q.x
			+ q.y * q.y
			+ q.z * q.z
			+ q.w * q.w;
		const T norm = std::sqrt(squared_norm);
		Quaternion<T> result(
			q.x / norm,
			q.y / norm,
			q.z / norm,
			q.w / norm
		);
		return result;
	}

	template <typename T>
	Quaternion<T> Conjugate(const Quaternion<T>& q)
	{
		Quaternion<T> result(-q.x, -q.y, -q.z, q.w);
		return result;
	}

	template <typename T>
	Quaternion<T> Inverse(const Quaternion<T>& q)
	{
		const T squared_norm = q.x * q.x
			+ q.y * q.y
			+ q.z * q.z
			+ q.w * q.w;
		Quaternion<T> result(
			-q.x / squared_norm,
			-q.y / squared_norm,
			-q.z / squared_norm,
			q.w / squared_norm
		);
		return result;
	}

	template <typename T>
	Vector3<T> Rotate(const Quaternion<T>& q, const Vector3<T>& v)
	{
		Vector3<T> qv = q.vector();
		Vector3<T> uv = Cross(qv, v);
		Vector3<T> uuv = Cross(qv, uv);

		Vector3<T> result = v + ((uv * q.scalar()) + uuv) * 2.0f;
		return result;
	}

	template <typename T>
	Matrix4<T> ToMat4(const Quaternion<T>& q)
	{
		Matrix4<T> result(1.0f);

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