#pragma once
#ifndef KATHA_TYPE_MATRIX_H__
#define KATHA_TYPE_MATRIX_H__ 1

#include "vector.hpp"

namespace katha
{
	template <typename T>
	struct Matrix3
	{
		Vector3<T> columns[3] = {};

		Matrix3() = default;
		Matrix3(const T& v)
		{
			columns[0] = Vector3<T>(v, 0, 0);
			columns[1] = Vector3<T>(0, v, 0);
			columns[2] = Vector3<T>(0, 0, v);
		}

		T* array() { return columns[0].array(); }
		const T* array() const { return columns[0].array(); }

		Vector3<T>& operator[](const uint32_t index)
		{
			return columns[index];
		}

		const Vector3<T>& operator[](const uint32_t index) const
		{
			return columns[index];
		}
	};

	template <typename T>
	struct Matrix4
	{
		Vector4<T> columns[4] = {};

		Matrix4() = default;
		Matrix4(const T& v)
		{
			columns[0] = Vector4<T>(v, 0, 0, 0);
			columns[1] = Vector4<T>(0, v, 0, 0);
			columns[2] = Vector4<T>(0, 0, v, 0);
			columns[3] = Vector4<T>(0, 0, 0, v);
		}

		static Matrix4<T> FromColumns(
			const Vector4<T>& c0,
			const Vector4<T>& c1,
			const Vector4<T>& c2,
			const Vector4<T>& c3
		)
		{
			Matrix4<T> result = {};
			result.columns[0] = c0;
			result.columns[1] = c1;
			result.columns[2] = c2;
			result.columns[3] = c3;
			return result;
		}

		static Matrix4<T> FromRows(
			const Vector4<T>& r0,
			const Vector4<T>& r1,
			const Vector4<T>& r2,
			const Vector4<T>& r3
		)
		{
			Matrix4<T> result = {};
			result.columns[0] = Vector4<T>(r0.x, r1.x, r2.x, r3.x);
			result.columns[1] = Vector4<T>(r0.y, r1.y, r2.y, r3.y);
			result.columns[2] = Vector4<T>(r0.z, r1.z, r2.z, r3.z);
			result.columns[3] = Vector4<T>(r0.w, r1.w, r2.w, r3.w);
			return result;
		}

		T* array() { return columns[0].array(); }
		const T* array() const { return columns[0].array(); }

		Vector4<T>& operator[](const uint32_t index)
		{
			return columns[index];
		}

		const Vector4<T>& operator[](const uint32_t index) const
		{
			return columns[index];
		}
	};

	using mat4 = Matrix4<float>;
	static_assert(sizeof(mat4) == sizeof(float) * 16, "mat4 is malformed");
}

#endif