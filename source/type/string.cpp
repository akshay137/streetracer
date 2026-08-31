#include "string.hpp"
#include "../utility.hpp"

#include <cstring>

bool katha::string_t::equals(const string_t& rhs) const
{
	if ((nullptr == buffer) || (nullptr == rhs.buffer))
	{
		return false;
	}
	if (size != rhs.size)
	{
		return false;
	}

	for (uint32_t i = 0; i < size; i++)
	{
		if (buffer[i] != rhs.buffer[i])
		{
			return false;
		}
	}

	return true;
}

bool katha::string_t::find(const string_t& pattern, uint32_t* out_index) const
{
	if (pattern.size > size)
	{
		return false;
	}

	const uint32_t iter_size = size - pattern.size;
	for (uint32_t i = 0; i < iter_size; i++)
	{
		bool match = true;
		for (uint32_t j = 0; j < pattern.size; j++)
		{
			if (buffer[i + j] != pattern.buffer[j])
			{
				match = false;
				break;
			}
		}

		if (match)
		{
			write_checked(out_index, i);
			return true;
		}
	}

	return false;
}

int32_t katha::string_t::read_utf8(const char* buffer, uint32_t* out_bytes)
{
	char byte = 0;
	if (!read_checked(buffer, &byte))
	{
		write_checked<uint32_t>(out_bytes, 0u);
		return 0;
	}

	if (0 == (byte & 0x80))
	{
		write_checked<uint32_t>(out_bytes, 1u);
		return byte;
	}

	if (0xF0 == (byte & 0xF0))
	{
		// 4 bytes: 11110uvv 10vvwwww 10xxxxyy 10yyzzzz
		write_checked<uint32_t>(out_bytes, 4u);
		int32_t code = byte & 0x7;
		code = (code << 6) | (buffer[1] & 0x3F);
		code = (code << 6) | (buffer[2] & 0x3F);
		code = (code << 6) | (buffer[3] & 0x3F);
		return code;
	}

	if (0xE0 == (byte & 0xE0))
	{
		// 3 bytes: 1110wwww 10xxxxyy 10yyzzzz
		write_checked<uint32_t>(out_bytes, 3u);
		int32_t code = byte & 0xf;
		code = (code << 6) | (buffer[1] & 0x3F);
		code = (code << 6) | (buffer[2] & 0x3F);
		return code;
	}

	if (0xC0 == (byte & 0xC0))
	{
		// 2 bytes: 110xxxyy 10yyzzzz
		write_checked<uint32_t>(out_bytes, 2u);
		int32_t code = byte & 0x1F;
		code = (code << 6) | (buffer[1] & 0x3F);
		return code;
	}

	return 0;
}

uint32_t katha::string_t::write_utf8(const int32_t code, char* buffer)
{
	if ((nullptr == buffer) || (code < 0))
	{
		return 0;
	}

	if (code < 0x80)
	{
		buffer[0] = static_cast<char>(code & 0xFF);
		return 1;
	}

	if (code < 0x800)
	{
		buffer[0] = 0xC0 | ((code >> 6) & 0x1F);
		buffer[1] = 0x80 | ((code & 0x3F));
		return 2;
	}

	if (code < 0x10000)
	{
		buffer[0] = 0xE0 | ((code >> 12) & 0xF);
		buffer[1] = 0x80 | ((code >> 6) & 0x3f);
		buffer[2] = 0x80 | (code & 0x3F);
		return 3;
	}

	if (code < 0x200000)
	{
		buffer[0] = 0xF0 | ((code >> 18) & 0x7);
		buffer[1] = 0x80 | ((code >> 12) & 0x3F);
		buffer[2] = 0x80 | ((code >> 6) & 0x3F);
		buffer[3] = 0x80 | (code & 0x3F);
		return 4;
	}

	return 0;
}

void katha::string_t::cstring_copy(
	const char* source,
	char* destination,
	const uint32_t max_bytes
)
{
	strncpy(destination, source, static_cast<size_t>(max_bytes));
}

bool katha::string_t::cstring_starts_with(
	const char* source,
	const char* prefix
)
{
	if ((nullptr == source) || (nullptr == prefix))
	{
		return false;
	}

	const char* s = source;
	const char* p = prefix;

	while ((*s && *p) && (*s == *p))
	{
		s++;
		p++;
	}

	return 0 == *p;
}