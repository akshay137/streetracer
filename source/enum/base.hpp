#pragma once
#ifndef KATHA_ENUM_BASE_H__
#define KATHA_ENUM_BASE_H__ 1

namespace katha
{
	enum class Base : int
	{
		BINARY = 2,
		OCTAL = 8,
		DECIMAL = 10,
		HEXADECIMAL = 16
	};

	extern const char* BaseToCString(const Base base);
}

#endif