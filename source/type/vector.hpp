#pragma once
#ifndef KATHA_CORE_TYPE_MATH_H__
#define KATHA_CORE_TYPE_MATH_H__ 1

#include "primitive.hpp"

namespace katha
{
	template <typename T>
	struct vector2_t
	{
		T x = {};
		T y = {};

		constexpr vector2_t() = default;
		constexpr vector2_t(const T& value): x{value}, y{value} {}
		constexpr vector2_t(const T& x, const T& y): x{x}, y{y} {}

		template <typename RT>
		explicit operator vector2_t<RT>() const
		{
			vector2_t<RT> v(
				static_cast<RT>(x),
				static_cast<RT>(y)
			);
			return v;
		}

		T* array() { return &x; }
		const T* array() const { return &x; }

		T& operator[](const uint32_t index) { return array()[index]; }
		const T& operator[](const uint32_t index) const
		{
			return array()[index];
		}
	};

	using vec2 = vector2_t<float>;
	using ivec2 = vector2_t<int32_t>;
	using uvec2 = vector2_t<uint32_t>;

	template <typename T>
	struct vector3_t
	{
		T x = {};
		T y = {};
		T z = {};

		constexpr vector3_t() = default;
		constexpr vector3_t(const T& value)
			: x{value}, y{value}, z{value}
		{}
		constexpr vector3_t(const T& x, const T& y, const T& z)
			: x{x}, y{y}, z{z}
		{}

		T* array() { return &x; }
		const T* array() const { return &x; }

		T& operator[](const uint32_t index) { return array()[index]; }
		const T& operator[](const uint32_t index) const
		{
			return array()[index];
		}
	};

	using vec3 = vector3_t<float>;
	using ivec3 = vector3_t<int32_t>;
	using uvec3 = vector3_t<uint32_t>;

	template <typename T>
	struct vector4_t
	{
		T x = {};
		T y = {};
		T z = {};
		T w = {};

		constexpr vector4_t() = default;
		constexpr vector4_t(const T& value)
			: x{value}, y{value}, z{value}, w{value}
		{}
		constexpr vector4_t(
			const T& x, const T& y, const T& z, const T& w
		) : x{x}, y{y}, z{z}, w{w}
		{}

		T* array() { return &x; }
		const T* array() const { return &x; }

		T& operator[](const uint32_t index) { return array()[index]; }
		const T& operator[](const uint32_t index) const
		{
			return array()[index];
		}
	};

	using vec4 = vector4_t<float>;
	using ivec4 = vector4_t<int32_t>;
	using uvec4 = vector4_t<uint32_t>;
}

#endif