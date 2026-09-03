#pragma once
#ifndef KATHA_MATH_VECTOR_2_H__
#define KATHA_MATH_VECTOR_2_H__ 1

#include "vector2.gen.hpp"

namespace katha
{
	template <typename T, typename ResultType=float>
	ResultType AspectXY(const Vector2<T>& v)
	{
		ResultType aspect = static_cast<ResultType>(v.x)
			/ static_cast<ResultType>(v.y);
		return aspect;
	}
}

#endif