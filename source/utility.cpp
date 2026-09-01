#include "utility.hpp"

#include "format/string.hpp"
#include "time/time.hpp"

#include <cinttypes>
#include <cstdio>

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_error.h>

uint32_t katha::log_line(const string_t format, ...)
{
#if KATHA_PRINT_LOG_DIAGNOSTICS
	const uint64_t start = now();
#endif

	string_format_t formatter = {};
	formatter.format = format;
	va_start(formatter.args, format);

	constexpr uint32_t BUFFER_SIZE = 128;
	char buffer[BUFFER_SIZE] = {};
	uint32_t buffer_index = 0;
	uint32_t bytes_written = 0;

	for (int32_t code = formatter.next(); code != 0; code = formatter.next())
	{
		if ((buffer_index + 4) >= BUFFER_SIZE)
		{
			fwrite(buffer, 1, buffer_index, stdout);
			bytes_written += buffer_index;
			buffer_index = 0;
		}
		
		uint32_t bytes = string_t::write_utf8(code, buffer + buffer_index);
		buffer_index += bytes;
	}
	va_end(formatter.args);

	if (buffer_index)
	{
		fwrite(buffer, 1, buffer_index, stdout);
		bytes_written += buffer_index;
	}

#if KATHA_PRINT_LOG_DIAGNOSTICS
	// log self diagnostics
	const int32_t bw = snprintf(buffer, BUFFER_SIZE,
		" [bytes: %" PRIu32 ", time: %.2f \xce\xbcs]\n",
		bytes_written,
		(now() - start) / 1000.0f
	);
	fwrite(buffer, 1, bw, stdout);
	bytes_written += static_cast<uint32_t>(bw);
#else
	fwrite("\n", 1, 1, stdout);
	bytes_written++;
#endif

	return bytes_written;
}

void* katha::allocate(const uint32_t size, const source_t& source)
{
	void* memory = SDL_malloc(size);
	if (nullptr == memory)
	{
		log_line("error: alloc({u64}) at {src}, {s}",
			&source, SDL_GetError()
		);
	}

	log_line("memory-allocate: {p}, {u64} bytes, in {src}",
		memory, size, &source
	);
	return memory;
}

void katha::release(void* memory, const source_t& source)
{
	if (nullptr == memory)
	{
		log_line("warning: release(0) called in {src}", &source);
		return;
	}

	log_line("memory-free: {p}, in {src}", memory, &source);
	SDL_free(memory);
}