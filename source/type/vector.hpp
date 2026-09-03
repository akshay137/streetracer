#pragma once
#ifndef KATHA_CORE_TYPE_MATH_H__
#define KATHA_CORE_TYPE_MATH_H__ 1

#include "primitive.hpp"

namespace katha
{
	template <typename T>
	struct Vector2
	{
		T x = {};
		T y = {};

		constexpr Vector2() = default;
		constexpr Vector2(const T& value): x{value}, y{value} {}
		constexpr Vector2(const T& x, const T& y): x{x}, y{y} {}

		template <typename RT>
		explicit operator Vector2<RT>() const
		{
			Vector2<RT> v(
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

	using vec2 = Vector2<float>;
	using ivec2 = Vector2<int32_t>;
	using uvec2 = Vector2<uint32_t>;

	template <typename T>
	struct Vector3
	{
		T x = {};
		T y = {};
		T z = {};

		constexpr Vector3() = default;
		constexpr Vector3(const T& value)
			: x{value}, y{value}, z{value}
		{}
		constexpr Vector3(const T& x, const T& y, const T& z)
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

	using vec3 = Vector3<float>;
	using ivec3 = Vector3<int32_t>;
	using uvec3 = Vector3<uint32_t>;

	template <typename T>
	struct Vector4
	{
		T x = {};
		T y = {};
		T z = {};
		T w = {};

		constexpr Vector4() = default;
		constexpr Vector4(const T& value)
			: x{value}, y{value}, z{value}, w{value}
		{}
		constexpr Vector4(
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

	using vec4 = Vector4<float>;
	using ivec4 = Vector4<int32_t>;
	using uvec4 = Vector4<uint32_t>;
}

#endif