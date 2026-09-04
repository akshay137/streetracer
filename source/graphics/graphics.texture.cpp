#include "graphics.hpp"
#include "../platform/platform.hpp"
#include "../utility.hpp"

bool CheckMagic(katha::File& file)
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

bool read_format(katha::File& file, katha::Format* out_format)
{
	uint8_t format_value = 0;
	if (!file.read<uint8_t>(&format_value))
	{
		return false;
	}

	if (format_value >= static_cast<uint32_t>(katha::Format::__MAX))
	{
		return false;
	}

	katha::Format format = static_cast<katha::Format>(format_value);
	WriteChecked<katha::Format>(out_format, format);
	return true;
}

katha::Result katha::LoadTexture(Texture* out_texture, File& file)
{
	if (nullptr == out_texture)
	{
		LogLine("error: load_texture called with null `out_texture`");
		return Result::ERROR_VALUE_UNEXPECTED;
	}

	if (!CheckMagic(file))
	{
		LogLine("error: not a .kbt texture");
		return Result::ERROR_VALUE_UNEXPECTED;
	}

	Format format;
	if (!read_format(file, &format))
	{
		LogLine("error-load_texture: failed to read format");
		return Result::ERROR_VALUE_NULL;
	}
	LogLine("kbt::format {s}", FormatToCString(format));

	uvec2 size = {};
	if (!file.read<uint32_t>(&size.x) || !file.read<uint32_t>(&size.y))
	{
		LogLine("error-load_texture: failed to read resolution");
		return Result::ERROR_VALUE_NULL;
	}
	LogLine("kbt::size {uv2}", size.array());

	uint32_t row_size = 0;
	if (!file.read<uint32_t>(&row_size))
	{
		LogLine("error-load_texture: failed to read row size");
		return Result::ERROR_VALUE_NULL;
	}

	uint32_t pixel_size = row_size * size.y;
	uint8_t* pixels = Alloc<uint8_t>(pixel_size);
	if (!file.read(pixels, pixel_size))
	{
		LogLine("error-load_texture: failed to read pixel data");
		Release(pixels);
		return Result::ERROR_VALUE_NULL;
	}

	Result result = CreateTexture(out_texture, format, size, pixels);
	Release(pixels);

	return result;
}

katha::Result katha::LoadTexture(Texture* out_texture, const char* filename)
{
	if (nullptr == out_texture) {
		return Result::ERROR_VALUE_NULL;
	}
	
	File file = Platform::Get()->openFileRead(filename);
	if (!file) {
		return Result::ERROR;
	}

	Result result = LoadTexture(out_texture, file);
	file.close();

	return result;
}