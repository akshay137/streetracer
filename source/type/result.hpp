#pragma once
#ifndef KATHA_TYPE_RESULT_H__
#define KATHA_TYPE_RESULT_H__ 1

#include "../enum/result.hpp"

namespace katha
{
	template <typename T>
	struct result_t
	{
		result_e result = result_e::success;
		T value = {};

		result_t() = default;
		
		result_t(const result_e result)
			: result{result}, value{}
		{}

		result_t(const T& value)
			: result{result_e::success}, value{value}
		{}

		operator bool () const
		{
			return result_e::success == result;
		}

		operator T () const
		{
			return value;
		}
	};
}

#endif