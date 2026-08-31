#pragma once
#ifndef KATHA_FORMAT_STRING_H__
#define KATHA_FORMAT_STRING_H__ 1

#include "../type/string.hpp"
#include "../type/stack_string.hpp"

#include <cstdarg>

namespace katha
{
	struct string_format_t
	{
		string_t format = {};
		va_list args;
		
		uint16_t format_index = 0;

		uint16_t pbuffer_index = 0;
		const char* param_str = nullptr;
		stack_string_t<256> param_buffer = {};

		int32_t next();

		int32_t parse_next_int(const string_t& props);
		int32_t parse_next_uint(const string_t& props);
		int32_t parse_next_float(const string_t& props);
		int32_t parse_next_int64(const string_t& props);
		int32_t parse_next_uint64(const string_t& props);
		int32_t parse_next_pointer(const string_t& props);
		int32_t parse_next_cstring(const string_t& props);
		int32_t parse_next_bool(const string_t& props);
		int32_t parse_next_vec3(const string_t& props);
		int32_t parse_next_ivec2(const string_t& props);
		int32_t parse_next_uvec2(const string_t& props);
		int32_t parse_next_vec2(const string_t& props);
		int32_t parse_next_quaternion(const string_t& props);
		int32_t parse_next_mat4(const string_t& props);
		int32_t parse_next_timediff(const string_t& props);
		int32_t parse_next_version(const string_t& props);
		int32_t parse_next_source(const string_t& props);

		static inline constexpr string_t SPEC_INT = "i";
		static inline constexpr string_t SPEC_UINT = "u";
		static inline constexpr string_t SPEC_FLOAT = "f";
		static inline constexpr string_t SPEC_INT64 = "i64";
		static inline constexpr string_t SPEC_UINT64 = "u64";
		static inline constexpr string_t SPEC_POINTER = "p";
		static inline constexpr string_t SPEC_CSTRING = "s";
		static inline constexpr string_t SPEC_BOOL = "b";
		static inline constexpr string_t SPEC_VEC3 = "v3";
		static inline constexpr string_t SPEC_IVEC2 = "iv2";
		static inline constexpr string_t SPEC_UVEC2 = "uv2";
		static inline constexpr string_t SPEC_VEC2 = "v2";
		static inline constexpr string_t SPEC_QUATERNION = "q";
		static inline constexpr string_t SPEC_MAT4 = "m4";
		static inline constexpr string_t SPEC_TIMEDIFF = "td";
		static inline constexpr string_t SPEC_VERSION = "version";
		static inline constexpr string_t SPEC_SOURCE = "src";
	};
}

#endif