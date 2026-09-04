#pragma once
#ifndef KATHA_GRAPHICS_H__
#define KATHA_GRAPHICS_H__ 1

#include "../enum/graphics.hpp"
#include "../enum/result.hpp"
#include "../type/bitfield.hpp"
#include "../type/buffer.hpp"
#include "../type/mesh.hpp"
#include "../type/texture.hpp"

namespace katha
{
	extern Result CreateBuffer(
		Buffer* out_buffer,
		EnumField<BufferUsage> usage,
		const uint32_t size,
		const void* data
	);
	extern void DeleteBuffer(const Buffer& buffer);

	extern Result CreateTexture(
		Texture* out_texture,
		const Format format,
		const uvec2 size,
		const void* pixels
	);
	extern void DeleteTexture(const Texture& texture);

	extern Result LoadTexture(
		Texture* out_texture,
		struct File& file
	);
	extern Result LoadTexture(
		Texture* out_texture,
		const char* filename
	);

	extern Result LoadMesh(
		Mesh* out_mesh,
		struct File& file
	);

	extern Result LoadMesh(
		Mesh* out_mesh,
		const char* filename
	);
	
	extern void DeleteMesh(const Mesh& mesh);
}

#endif