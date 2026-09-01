#include "graphics.hpp"
#include "../platform/platform.hpp"

katha::result_e katha::create_buffer(
	buffer_t* out_buffer,
	efield_t<buffer_usage_e> usage,
	const uint32_t size,
	const void* data
)
{
	return platform_t::get()->gl.create_buffer(out_buffer, usage, size, data);
}

void katha::delete_buffer(const buffer_t& buffer)
{
	platform_t::get()->gl.delete_buffer(buffer);
}

katha::result_e katha::create_texture(
	texture_t* out_texture,
	const format_e format,
	const uvec2 size,
	const void* pixels
)
{
	return platform_t::get()->gl.create_texture(out_texture, format, size, pixels);
}

void katha::delete_texture(const texture_t& texture)
{
	platform_t::get()->gl.delete_texture(texture);
}