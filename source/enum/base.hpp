#pragma once
#ifndef KATHA_ENUM_BASE_H__
#define KATHA_ENUM_BASE_H__ 1

namespace katha
{
	enum class base_e : int
	{
		binary = 2,
		octal = 8,
		decimal = 10,
		hexadecimal = 16
	};

	extern const char* base_to_cstring(const base_e base);
}

#endif