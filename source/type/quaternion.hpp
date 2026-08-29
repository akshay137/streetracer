#pragma once
#ifndef KATHA_TYPE_QUATERNION_H__
#define KATHA_TYPE_QUATERNION_H__ 1

#include "vector.hpp"

namespace katha
{
	template <typename T>
	struct quaternion_t
	{
		T x = {};
		T y = {};
		T z = {};
		T w = {};

		quaternion_t() = default;
		constexpr quaternion_t(const T& value)
			: x{value}, y{value}, z{value}, w{value}
		{}
		constexpr quaternion_t(
			const T& x, const T& y, const T& z, const T& w
		) : x{x}, y{y}, z{z}, w{w}
		{}

		constexpr quaternion_t(const vector3_t<T> v, const T& w)
			: x{v.x}, y{v.y}, z{v.z}, w{w}
		{}

		T* array() { return &x; }
		const T* array() const { return &x; }

		static quaternion_t<T> identity()
		{
			quaternion_t<T> i(0, 0, 0, 1);
			return i;
		}

		vector3_t<T> vector() const
		{
			vector3_t<T> v(x, y, z);
			return v;
		}

		quaternion_t<T>& vector(const vector3_t<T>& v)
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

		quaternion_t<T>& scalar(const T& s)
		{
			w = s;
			return *this;
		}
	};

	using quat_t = quaternion_t<float>;
}

#endif