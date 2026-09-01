#pragma once
#ifndef KATHA_ENUM_FORMAT_H__
#define KATHA_ENUM_FORMAT_H__ 1

namespace katha
{
	enum class buffer_usage_e : int
	{
		element, // index buffer
		stream,

		__max
	};

	enum class format_e : int
	{
		none = 0, // to specify no texture in some functions
		greyscale8,
		rgba8,
		rgb8,
		srgba8,
		srgb8,
		depth24_stencil8,

		__max
	};

	extern const char* format_to_cstring(const format_e format);
	extern bool should_mipmap(const format_e format);
}

#endif