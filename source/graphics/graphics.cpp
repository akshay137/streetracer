#include "graphics.hpp"
#include "../platform/platform.hpp"

katha::Result katha::CreateBuffer(
	Buffer* out_buffer,
	EField<BufferUsage> usage,
	const uint32_t size,
	const void* data
)
{
	return Platform::Get()->gles.createBuffer(out_buffer, usage, size, data);
}

void katha::DeleteBuffer(const Buffer& buffer)
{
	Platform::Get()->gles.deleteBuffer(buffer);
}

katha::Result katha::CreateTexture(
	Texture* out_texture,
	const Format format,
	const uvec2 size,
	const void* pixels
)
{
	return Platform::Get()->gles.createTexture(out_texture, format, size, pixels);
}

void katha::DeleteTexture(const Texture& texture)
{
	Platform::Get()->gles.deleteTexture(texture);
}

void katha::DeleteMesh(const Mesh& mesh)
{
	for (uint8_t i = 0; i < mesh.mesh_count; i++)
	{
		DeleteBuffer(mesh.data[i].vertices);
		DeleteBuffer(mesh.data[i].indices);
	}
}