#include "base.hpp"

const char* katha::BaseToCString(const Base base)
{
	switch (base)
	{
		case Base::BINARY: return "Base::BINARY";
		case Base::OCTAL: return "Base::OCTAL";
		case Base::DECIMAL: return "Base::DECIMAL";
		case Base::HEXADECIMAL: return "Base::HEXADECIMAL";
	}

	return "Unknown Base";
}