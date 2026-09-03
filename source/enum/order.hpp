#pragma once
#ifndef KATHA_ENUM_ORDER_H__
#define KATHA_ENUM_ORDER_H__ 1

namespace katha
{
	enum class Order : int
	{
		LESS = -1,
		EQUAL = 0,
		GREATER = 1
	};
}

#endif