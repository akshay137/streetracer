#include "graphics.hpp"

const char* katha::FormatToCString(const Format format)
{
	switch (format)
	{
		case Format::NONE: return "Format::NONE";
		case Format::GREYSCALE8: return "Format::GREYSCALE8";
		case Format::RGBA8: return "Format::RGBA8";
		case Format::RGB8: return "Format::RGB8";
		case Format::SRGBA8: return "Format::SRGBA8";
		case Format::SRGB8: return "Format::SRGB8";
		case Format::DEPTH24_STENCIL8: return "Format::DEPTH24_STENCIL8";
	}
	
	return "Unknown Format";
}

bool katha::ShouldMipmap(const Format format)
{
	switch (format)
	{
		case Format::NONE:
		case Format::DEPTH24_STENCIL8:
			return false;
	}

	return true;
}