#pragma once
#ifndef KATHA_PLATFORM_FILE_H__
#define KATHA_PLATFORM_FILE_H__ 1

#include "primitive.hpp"
#include <SDL2/SDL_rwops.h>

namespace katha
{
	struct File
	{
		SDL_RWops* handle = nullptr;

		operator bool () const
		{
			return nullptr != handle;
		}

		// exits on failure
		static File OpenRead(const char* filepath);

		// exits on failure
		static File OpenWrite(const char* filepath);

		// must release
		[[nodiscard]] static void* ReadAll(
			const char* filepath,
			uint32_t* out_length
		);

		static uint32_t WriteAll(
			const char* filepath,
			const void* data,
			const uint32_t length
		);

		void close();

		uint32_t length();

		uint32_t read(void* buffer, const uint32_t bytes);
		uint32_t write(const void* buffer, const uint32_t bytes);

		template <typename T>
		bool read(T* out_value)
		{
			T temp;
			uint32_t br = read(&temp, sizeof(T));
			if (sizeof(T) != br)
			{
				return false;
			}

			WriteChecked<T>(out_value, temp);
			return true;
		}
	};
}

#endif