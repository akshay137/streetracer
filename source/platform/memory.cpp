#include "../katha/core.hpp"

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_error.h>

void* katha::allocate(const size_t size, const source_t& source)
{
	void* memory = SDL_malloc(size);
	if (nullptr == memory)
	{
		log_line("error: alloc({u64}) at {s}:{u}, {s}",
			source.file_name(),
			static_cast<uint32_t>(source.line()),
			SDL_GetError()
		);
	}

	log_line("memory-allocate: {p}, {u64} bytes, location {s}:{u}",
		memory, size,
		source.file_name(),
		static_cast<uint32_t>(source.line())
	);
	return memory;
}

void katha::release(void* memory, const source_t& source)
{
	if (nullptr == memory)
	{
		log_line("warning: release(0) called from {s}:{u}",
			source.file_name(),
			static_cast<uint32_t>(source.line())
		);
		return;
	}

	log_line("memory-free: {p}, location {s}:{u}",
		memory,
		source.file_name(),
		static_cast<uint32_t>(source.line())
	);
	SDL_free(memory);
}
