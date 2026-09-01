#include "graphics.hpp"
#include "../platform/platform.hpp"
#include "../utility.hpp"

bool check_magic_kbt(katha::file_t& file)
{
	constexpr const char* MAGICK = "\0KBT\0";
	
	char magick[5] = {};
	const uint32_t r = file.read(magick, 5);
	if (5 != r)
	{
		return false;
	}

	for (uint32_t i = 0; i < 5; i++)
	{
		if (magick[i] != MAGICK[i])
		{
			return false;
		}
	}

	return true;
}

bool read_format(katha::file_t& file, katha::format_e* out_format)
{
	uint8_t format_value = 0;
	if (!file.read<uint8_t>(&format_value))
	{
		return false;
	}

	if (format_value >= static_cast<uint32_t>(katha::format_e::__max))
	{
		return false;
	}

	katha::format_e format = static_cast<katha::format_e>(format_value);
	write_checked(out_format, format);
	return true;
}

katha::result_e katha::load_texture(texture_t* out_texture, struct file_t& file)
{
	if (nullptr == out_texture)
	{
		log_line("error: load_texture called with null `out_texture`");
		return result_e::error_value_unexpected;
	}

	if (!check_magic_kbt(file))
	{
		log_line("error: not a .kbt texture");
		return result_e::error_value_unexpected;
	}

	format_e format;
	if (!read_format(file, &format))
	{
		log_line("error-load_texture: failed to read format");
		return result_e::error_value_null;
	}
	log_line("kbt::format {s}", format_to_cstring(format));

	uvec2 size = {};
	if (!file.read<uint32_t>(&size.x) || !file.read<uint32_t>(&size.y))
	{
		log_line("error-load_texture: failed to read resolution");
		return result_e::error_value_null;
	}
	log_line("kbt::size {uv2}", size.array());

	uint32_t row_size = 0;
	if (!file.read<uint32_t>(&row_size))
	{
		log_line("error-load_texture: failed to read row size");
		return result_e::error_value_null;
	}

	uint32_t pixel_size = row_size * size.y;
	uint8_t* pixels = alloc<uint8_t>(pixel_size);
	if (!file.read(pixels, pixel_size))
	{
		log_line("error-load_texture: failed to read pixel data");
		release(pixels);
		return result_e::error_value_null;
	}

	result_e result = create_texture(out_texture, format, size, pixels);
	release(pixels);

	return result;
}

katha::result_e katha::load_texture(texture_t* out_texture, const char* filename)
{
	file_t file = platform_t::get()->open_file_read(filename);
	if (!file)
	{
		return result_e::error;
	}

	result_e result = load_texture(out_texture, file);
	file.close();

	return result;
}