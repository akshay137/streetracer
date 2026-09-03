#pragma once
#ifndef KATHA_FORMAT_STRING_H__
#define KATHA_FORMAT_STRING_H__ 1

#include "../type/string.hpp"
#include "../type/stack_string.hpp"

#include <cstdarg>

namespace katha
{
	// FIXME: bool next(int32_t* out_code)
	struct StringFormat
	{
		String format = {};
		va_list args;
		
		uint16_t format_index = 0;

		uint16_t pbuffer_index = 0;
		const char* param_str = nullptr;
		StackString<256> param_buffer = {};

		int32_t next();

		int32_t __parseNextInt(const String& props);
		int32_t __parseNextUInt(const String& props);
		int32_t __parseNextFloat(const String& props);
		int32_t __parseNextInt64(const String& props);
		int32_t __parseNextUInt64(const String& props);
		int32_t __parseNextPointer(const String& props);
		int32_t __parseNextCString(const String& props);
		int32_t __parseNextBool(const String& props);
		int32_t __parseNextVec3(const String& props);
		int32_t __parseNextIVec2(const String& props);
		int32_t __parseNextUVec2(const String& props);
		int32_t __parseNextVec2(const String& props);
		int32_t __parseNextQuaternion(const String& props);
		int32_t __parseNextMat4(const String& props);
		int32_t __parseNextTimeDiff(const String& props);
		int32_t __parseNextVersion(const String& props);
		int32_t __parseNextSource(const String& props);

		static inline constexpr String SPEC_INT = "i";
		static inline constexpr String SPEC_UINT = "u";
		static inline constexpr String SPEC_FLOAT = "f";
		static inline constexpr String SPEC_INT64 = "i64";
		static inline constexpr String SPEC_UINT64 = "u64";
		static inline constexpr String SPEC_POINTER = "p";
		static inline constexpr String SPEC_CSTRING = "s";
		static inline constexpr String SPEC_BOOL = "b";
		static inline constexpr String SPEC_VEC3 = "v3";
		static inline constexpr String SPEC_IVEC2 = "iv2";
		static inline constexpr String SPEC_UVEC2 = "uv2";
		static inline constexpr String SPEC_VEC2 = "v2";
		static inline constexpr String SPEC_QUATERNION = "q";
		static inline constexpr String SPEC_MAT4 = "m4";
		static inline constexpr String SPEC_TIMEDIFF = "td";
		static inline constexpr String SPEC_VERSION = "version";
		static inline constexpr String SPEC_SOURCE = "src";
	};
}

#endif