#include "file.hpp"
#include "../platform/platform.hpp"
#include "../utility.hpp"

katha::File katha::File::OpenRead(const char* filepath)
{
	SDL_RWops* handle = SDL_RWFromFile(filepath, "rb");
	if (nullptr == handle)
	{
		LogLine("error-sdl: SDL_RWFromFile {s}", SDL_GetError());
		return {};
	}
	
	File file = {};
	file.handle = handle;
	LogLine("file::open_read({s}) {p}", filepath, handle);
	return file;
}

katha::File katha::File::OpenWrite(const char* filepath)
{
	SDL_RWops* handle = SDL_RWFromFile(filepath, "wb");
	if (nullptr == handle)
	{
		LogLine("error-sdl: SDL_RWFromFile {s}", SDL_GetError());
		return {};
	}
	
	File file = {};
	file.handle = handle;
	LogLine("file::open_write({s}) {p}", filepath, handle);
	return file;
}

uint32_t katha::File::length()
{
	if (nullptr == handle)
	{
		return 0;
	}

	const Sint64 current_pos = SDL_RWtell(handle);
	if (-1 == current_pos)
	{
		LogLine("error-sdl: SDL_RWtell({p}) {s}", handle, SDL_GetError());
		return 0;
	}

	const Sint64 end = SDL_RWseek(handle, 0, RW_SEEK_END);
	if (-1 == end)
	{
		LogLine("error-sdl: SDL_RWseek({p}) {s}", handle, SDL_GetError());
		return 0;
	}

	const Sint64 s = SDL_RWseek(handle, current_pos, RW_SEEK_SET);
	if (-1 == s)
	{
		LogLine("error-sdl: SDL_RWseek({p}) {s}", handle, SDL_GetError());
		return 0;
	}

	return static_cast<uint32_t>(end);
}

void* katha::File::ReadAll(const char* filepath, uint32_t* out_length)
{
	size_t size = 0;
	void* data = SDL_LoadFile(filepath, &size);
	if (nullptr == data)
	{
		LogLine("error-sdl: SDL_LoadFile({s}), {s}", filepath, SDL_GetError());
		return nullptr;
	}

	// to match the later `memory-free` log entry
	LogLine("memory-allocate: {p}, file::read_all({s})", data, filepath);
	WriteChecked<uint32_t>(out_length, static_cast<uint32_t>(size));
	return data;
}

uint32_t katha::File::WriteAll(
	const char* filepath,
	const void* data,
	const uint32_t length
)
{
	File file = File::OpenWrite(filepath);
	const uint32_t bw = file.write(data, length);
	file.close();
	return bw;
}

void katha::File::close()
{
	if (nullptr == handle)
	{
		return;
	}

	if (SDL_RWclose(handle) < 0)
	{
		LogLine("error-sdl: SDL_RWclose {s}", SDL_GetError());
	}

	LogLine("file::close({p})", handle);
	handle = nullptr;
}

uint32_t katha::File::read(void* buffer, const uint32_t bytes)
{
	size_t total_read = 0;
	size_t remaining_read = static_cast<size_t>(bytes);

	uint8_t* data = (uint8_t*)buffer;
	while (total_read < bytes)
	{
		SDL_ClearError();
		const size_t br = SDL_RWread(
			handle,
			data + total_read,
			1, remaining_read
		);
		if (br == remaining_read)
		{
			return bytes;
		}
		else if (0 == br)
		{
			const char* error_str = SDL_GetError();
			if (error_str && error_str[0])
			{
				LogLine("error-sdl: SDL_RWread({p}) {s}",
					handle, error_str
				);
				return 0;
			}

			LogLine("warning: unexpected EOF, request: {u}, read: {u64}", bytes, total_read);
			return static_cast<uint32_t>(total_read); // EOF
		}

		total_read += br;
		remaining_read -= br;
	}

	return 0;
}

uint32_t katha::File::write(const void* buffer, const uint32_t bytes)
{
	size_t total_write = 0;
	size_t remaining_write = static_cast<size_t>(bytes);

	uint8_t* data = (uint8_t*)buffer;
	while (total_write < bytes)
	{
		SDL_ClearError();
		const size_t bw = SDL_RWwrite(
			handle,
			data + total_write,
			1, remaining_write
		);
		if (bw == remaining_write)
		{
			return bytes;
		}
		else if (0 == bw)
		{
			const char* error_str = SDL_GetError();
			if (error_str && error_str[0])
			{
				LogLine("error-sdl: SDL_RWwrite({p}) {s}",
					handle, error_str
				);
				return 0;
			}

			// TODO: write file having EOF?
			return static_cast<uint32_t>(total_write);
		}

		total_write += bw;
		remaining_write -= bw;
	}

	return 0;
}