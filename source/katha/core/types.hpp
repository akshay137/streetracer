#pragma once
#ifndef KATHA_CORE_TYPES_H__
#define KATHA_CORE_TYPES_H__ 1

#include <cstdint>
#include <cstddef>

#include <source_location>

namespace katha
{
	using source_t = std::source_location;

	constexpr static float PI = 3.14159265359f;
	
	struct version_t
	{
		uint32_t major:8 = 0;
		uint32_t minor:8 = 0;
		uint32_t patch:16 = 0;

		constexpr version_t() = default;
		constexpr version_t(
			const uint32_t major,
			const uint32_t minor,
			const uint32_t patch
		): major{major}, minor{minor}, patch{patch}
		{}
	};

	template <typename T>
	struct vector2_t
	{
		T x = {};
		T y = {};

		constexpr vector2_t() = default;
		constexpr vector2_t(const T& value): x{value}, y{value} {}
		constexpr vector2_t(const T& x, const T& y): x{x}, y{y} {}

		template <typename RT>
		explicit operator vector2_t<RT>()
		{
			vector2_t<RT> v(
				static_cast<RT>(x),
				static_cast<RT>(y)
			);
			return v;
		}

		T* array() { return &x; }
		const T* array() const { return &x; }

		T& operator[](const size_t index) { return array()[index]; }
		const T& operator[](const size_t index) const
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

		T& operator[](const size_t index) { return array()[index]; }
		const T& operator[](const size_t index) const
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

		T& operator[](const size_t index) { return array()[index]; }
		const T& operator[](const size_t index) const
		{
			return array()[index];
		}
	};

	using vec4 = vector4_t<float>;
	using ivec4 = vector4_t<int32_t>;
	using uvec4 = vector4_t<uint32_t>;

	// this is a template because then it allows for
	// data & operations to live in separate files
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

	template <typename T>
	struct matrix4_t
	{
		vector4_t<T> columns[4] = {};

		matrix4_t() = default;
		matrix4_t(const T& v)
		{
			columns[0] = vector4_t<T>(v, 0, 0, 0);
			columns[1] = vector4_t<T>(0, v, 0, 0);
			columns[2] = vector4_t<T>(0, 0, v, 0);
			columns[3] = vector4_t<T>(0, 0, 0, v);
		}

		static matrix4_t<T> from_columns(
			const vector4_t<T>& c0,
			const vector4_t<T>& c1,
			const vector4_t<T>& c2,
			const vector4_t<T>& c3
		)
		{
			matrix4_t<T> result = {};
			result.columns[0] = c0;
			result.columns[1] = c1;
			result.columns[2] = c2;
			result.columns[3] = c3;
			return result;
		}

		static matrix4_t<T> from_rows(
			const vector4_t<T>& r0,
			const vector4_t<T>& r1,
			const vector4_t<T>& r2,
			const vector4_t<T>& r3
		)
		{
			matrix4_t<T> result = {};
			result.columns[0] = vector4_t<T>(r0.x, r1.x, r2.x, r3.x);
			result.columns[1] = vector4_t<T>(r0.y, r1.y, r2.y, r3.y);
			result.columns[2] = vector4_t<T>(r0.z, r1.z, r2.z, r3.z);
			result.columns[3] = vector4_t<T>(r0.w, r1.w, r2.w, r3.w);
			return result;
		}

		T* array() { return columns[0].array(); }
		const T* array() const { return columns[0].array(); }

		vector4_t<T>& operator[](const size_t index)
		{
			return columns[index];
		}

		const vector4_t<T>& operator[](const size_t index) const
		{
			return columns[index];
		}
	};

	using mat4 = matrix4_t<float>;
	static_assert(sizeof(mat4) == sizeof(float) * 16, "mat4 is malformed");

	template <typename T>
	static bool write_checked(T* ptr, const T& value)
	{
		if (nullptr == ptr)
		{
			return false;
		}

		*ptr = value;
		return true;
	}

	template <typename T>
	static bool read_checked(const T* ptr, T* out_value)
	{
		if (nullptr == ptr)
		{
			return false;
		}

		return write_checked<T>(out_value, *ptr);
	}

	template <typename T>
	static constexpr T min(const T& lhs, const T& rhs)
	{
		return lhs < rhs ? lhs : rhs;
	}

	template <typename T>
	static constexpr T max(const T& lhs, const T& rhs)
	{
		return lhs > rhs ? lhs : rhs;
	}

	template <typename T>
	static constexpr T clamp(const T& value, const T& lower, const T& upper)
	{
		return max(lower, min(value, upper));
	}

	constexpr static inline float radians(const float angle_in_degrees)
	{
		float result = angle_in_degrees * (PI / 180.0f);
		return result;
	}

	constexpr static inline float degrees(const float angle_in_radians)
	{
		float result = angle_in_radians * (180.0f / PI);
		return result;
	}
}

#endif