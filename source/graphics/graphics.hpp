#pragma once
#ifndef KATHA_GRAPHICS_H__
#define KATHA_GRAPHICS_H__ 1

#include "../enum/graphics.hpp"
#include "../type/bitfield.hpp"
#include "../type/buffer.hpp"
#include "../type/texture.hpp"
#include "../type/result.hpp"

namespace katha
{
	extern result_e create_buffer(
		buffer_t* out_buffer,
		efield_t<buffer_usage_e> usage,
		const uint32_t size,
		const void* data
	);
	extern void delete_buffer(const buffer_t& buffer);

	extern result_e create_texture(
		texture_t* out_texture,
		const format_e format,
		const uvec2 size,
		const void* pixels
	);
	extern void delete_texture(const texture_t& texture);

	extern result_e load_texture(
		texture_t* out_texture,
		struct file_t& file
	);
	extern result_e load_texture(
		texture_t* out_texture,
		const char* filename
	);
}

#endif