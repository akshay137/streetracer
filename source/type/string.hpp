#pragma once
#ifndef KATHA_CORE_STRING_H__
#define KATHA_CORE_STRING_H__ 1

#include "primitive.hpp"

namespace katha
{
	// view if capacity is zero and size & buffer have non-zero values
	// owns buffer if capacity is non-zero, and caller must free
	struct String
	{
		char* buffer = nullptr;
		uint32_t size = 0;
		uint32_t capacity = 0;

		String() = default;

		constexpr static uint32_t Length(const char* cstring)
		{
			if (nullptr == cstring)
			{
				return 0;
			}

			uint32_t count = 0;
			while (cstring[count++]) {}
			return count - 1;
		}

		constexpr String(const char* cstring)
			: buffer{(char*)cstring}, size{Length(cstring)}, capacity{0}
		{}

		constexpr String(const char* cstring, const uint32_t size)
			: buffer{(char*)cstring}, size{size}, capacity{0}
		{}

		static String CopyFromCString(
			const char* cstring,
			uint32_t size = 0,
			const source_t& source = source_t::current()
		);

		void clear();

		char& operator[](const uint32_t index)
		{
			return buffer[index];
		}

		const char& operator[](const uint32_t index) const
		{
			return buffer[index];
		}

		bool equals(const String& rhs) const;
		bool find(const String& pattern, uint32_t* out_index) const;

		static int32_t ReadUTF8(const char* buffer, uint32_t* out_bytes);
		static uint32_t WriteUTF8(const int32_t code, char* buffer);

		// some cstring function helpers

		static void CStringCopy(
			const char* source,
			char* destination,
			const uint32_t max_bytes
		);

		static bool CStringStartsWith(
			const char* source,
			const char* prefix
		);

		// path helpers

		static String JoinPath(
			const String& base,
			const String& suffix,
			const source_t& source = source_t::current()
		);
		static bool JoinPath(
			const String& base,
			const String& suffix,
			char* buffer,
			const uint32_t buffer_length
		);
	};
}

#endif