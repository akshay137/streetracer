#include "graphics.hpp"

const char* katha::format_to_cstring(const format_e format)
{
	switch (format)
	{
		case format_e::none: return "format_e::none";
		case format_e::greyscale8: return "format_e::greyscale8";
		case format_e::rgba8: return "format_e::rgba8";
		case format_e::rgb8: return "format_e::rgb8";
		case format_e::srgba8: return "format_e::srgba8";
		case format_e::srgb8: return "format_e::srgb8";
		case format_e::depth24_stencil8: return "format_e::depth24_stencil8";
	}
	
	return "unknown format";
}

bool katha::should_mipmap(const format_e format)
{
	switch (format)
	{
		case format_e::none:
		case format_e::depth24_stencil8:
			return false;
	}

	return true;
}