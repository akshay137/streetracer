#pragma once
#ifndef KATHA_TYPE_QUATERNION_H__
#define KATHA_TYPE_QUATERNION_H__ 1

#include "vector.hpp"

namespace katha
{
	template <typename T>
	struct Quaternion
	{
		T x = {};
		T y = {};
		T z = {};
		T w = {};

		Quaternion() = default;
		constexpr Quaternion(const T& value)
			: x{value}, y{value}, z{value}, w{value}
		{}
		constexpr Quaternion(
			const T& x, const T& y, const T& z, const T& w
		) : x{x}, y{y}, z{z}, w{w}
		{}

		constexpr Quaternion(const Vector3<T> v, const T& w)
			: x{v.x}, y{v.y}, z{v.z}, w{w}
		{}

		T* array() { return &x; }
		const T* array() const { return &x; }

		static Quaternion<T> Identity()
		{
			Quaternion<T> i(0, 0, 0, 1);
			return i;
		}

		Vector3<T> vector() const
		{
			Vector3<T> v(x, y, z);
			return v;
		}

		Quaternion<T>& vector(const Vector3<T>& v)
		{
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		T scalar() const
		{
			return w;
		}

		Quaternion<T>& scalar(const T& s)
		{
			w = s;
			return *this;
		}
	};

	using quat = Quaternion<float>;
}

#endif