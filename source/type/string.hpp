#pragma once
#ifndef KATHA_CORE_STRING_H__
#define KATHA_CORE_STRING_H__ 1

#include "primitive.hpp"

namespace katha
{
	// view if capacity is zero and size & buffer have non-zero values
	// owns buffer if capacity is non-zero, and caller must free
	struct string_t
	{
		char* buffer = nullptr;
		uint32_t size = 0;
		uint32_t capacity = 0;

		string_t() = default;

		constexpr uint32_t length(const char* cstring)
		{
			if (nullptr == cstring)
			{
				return 0;
			}

			uint32_t count = 0;
			while (cstring[count++]) {}
			return count - 1;
		}

		constexpr string_t(const char* cstring)
			: buffer{(char*)cstring}, size{length(cstring)}, capacity{0}
		{}

		constexpr string_t(const char* cstring, const uint32_t size)
			: buffer{(char*)cstring}, size{size}, capacity{0}
		{}

		char& operator[](const uint32_t index)
		{
			return buffer[index];
		}

		const char& operator[](const uint32_t index) const
		{
			return buffer[index];
		}

		bool equals(const string_t& rhs) const;
		bool find(const string_t& pattern, uint32_t* out_index) const;

		static int32_t read_utf8(const char* buffer, uint32_t* out_bytes);
		static uint32_t write_utf8(const int32_t code, char* buffer);

		// some cstring function helpers

		static void cstring_copy(
			const char* source,
			char* destination,
			const uint32_t max_bytes
		);

		static bool cstring_starts_with(
			const char* source,
			const char* prefix
		);
	};
}

#endif