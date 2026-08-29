#pragma once
#ifndef KATHA_TYPE_MATRIX_H__
#define KATHA_TYPE_MATRIX_H__ 1

#include "vector.hpp"

namespace katha
{
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

		vector4_t<T>& operator[](const uint32_t index)
		{
			return columns[index];
		}

		const vector4_t<T>& operator[](const uint32_t index) const
		{
			return columns[index];
		}
	};

	using mat4 = matrix4_t<float>;
	static_assert(sizeof(mat4) == sizeof(float) * 16, "mat4 is malformed");
}

#endif