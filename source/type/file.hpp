#pragma once
#ifndef KATHA_PLATFORM_FILE_H__
#define KATHA_PLATFORM_FILE_H__ 1

#include "stream.hpp"

#include <SDL2/SDL_rwops.h>

namespace katha
{
	struct file_t : public stream_i
	{
		SDL_RWops* handle = nullptr;

		// exits on failure
		static file_t open_read(const char* filepath);

		// exits on failure
		static file_t open_write(const char* filepath);

		// must release
		[[nodiscard]] static void* read_all(
			const char* filepath,
			uint32_t* out_length
		);

		static uint32_t write_all(
			const char* filepath,
			const void* data,
			const uint32_t length
		);

		void close();

		uint32_t length();

		uint32_t read(void* buffer, const uint32_t bytes) override;
		uint32_t write(const void* buffer, const uint32_t bytes) override;
	};
}

#endif