#pragma once
#ifndef KATHA_ENUM_FORMAT_H__
#define KATHA_ENUM_FORMAT_H__ 1

namespace katha
{
	enum class BufferUsage : int
	{
		ELEMENT, // index buffer
		STREAM,

		__MAX
	};

	enum class Format : int
	{
		NONE = 0, // to specify no texture in some functions
		GREYSCALE8,
		RGBA8,
		RGB8,
		SRGBA8,
		SRGB8,
		DEPTH24_STENCIL8,

		__MAX
	};

	extern const char* FormatToCString(const Format format);
	extern bool ShouldMipmap(const Format format);
}

#endif