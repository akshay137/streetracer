#pragma once
#ifndef KATHA_MATH_MATRIX_4_H__
#define KATHA_MATH_MATRIX_4_H__ 1

#include "../type/matrix.hpp"
#include "vector4.hpp"

#include <cmath>

namespace katha
{
	template <typename T>
	Matrix4<T> Transpose(const Matrix4<T>& m)
	{
		Matrix4<T> result = {};
		result[0] = Vector4<T>(m[0][0], m[1][0], m[2][0], m[3][0]);
		result[1] = Vector4<T>(m[0][1], m[1][1], m[2][1], m[3][1]);
		result[2] = Vector4<T>(m[0][2], m[1][2], m[2][2], m[3][2]);
		result[3] = Vector4<T>(m[0][3], m[1][3], m[2][3], m[3][3]);
		return result;
	};

	template <typename T>
	Matrix4<T> operator * (const Matrix4<T>& m1, const Matrix4<T>& m2)
	{
		const Matrix4<T> t = Transpose(m1);
		Matrix4<T> result = {};
		
		result[0][0] = HorizontalSum(t[0] * m2[0]);
		result[0][1] = HorizontalSum(t[1] * m2[0]);
		result[0][2] = HorizontalSum(t[2] * m2[0]);
		result[0][3] = HorizontalSum(t[3] * m2[0]);

		result[1][0] = HorizontalSum(t[0] * m2[1]);
		result[1][1] = HorizontalSum(t[1] * m2[1]);
		result[1][2] = HorizontalSum(t[2] * m2[1]);
		result[1][3] = HorizontalSum(t[3] * m2[1]);

		result[2][0] = HorizontalSum(t[0] * m2[2]);
		result[2][1] = HorizontalSum(t[1] * m2[2]);
		result[2][2] = HorizontalSum(t[2] * m2[2]);
		result[2][3] = HorizontalSum(t[3] * m2[2]);
		
		result[3][0] = HorizontalSum(t[0] * m2[3]);
		result[3][1] = HorizontalSum(t[1] * m2[3]);
		result[3][2] = HorizontalSum(t[2] * m2[3]);
		result[3][3] = HorizontalSum(t[3] * m2[3]);

		return result;
	}

	template <typename T>
	Matrix4<T> Translate(const Matrix4<T>& m, const Vector3<T>& v)
	{
		Matrix4<T> t(1.0f);
		t[3] = Vector4<T>(v.x, v.y, v.z, 1.0f);
		
		Matrix4<T> result = m * t;
		return result;
	}

	template <typename T>
	Matrix4<T> Scale(const Matrix4<T>& m, const Vector3<T>& v)
	{
		Matrix4<T> s = Matrix4<T>::FromColumns(
			Vector4<T>(v.x, 0, 0, 0),
			Vector4<T>(0, v.y, 0, 0),
			Vector4<T>(0, 0, v.z, 0),
			Vector4<T>(0, 0, 0, 1)
		);

		Matrix4<T> result = m * s;
		return result;
	}
}

#endif