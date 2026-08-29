#include "base.hpp"

const char* katha::base_to_cstring(const base_e base)
{
	switch (base)
	{
		case base_e::binary: return "base_e::binary";
		case base_e::octal: return "base_e::octal";
		case base_e::decimal: return "base_e::decimal";
		case base_e::hexadecimal: return "base_e::hexadecimal";
	}

	return "unknown base";
}