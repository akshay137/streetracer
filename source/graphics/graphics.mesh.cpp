#include "graphics.hpp"
#include "../type/vertex.hpp"
#include "../platform/platform.hpp"
#include "../utility.hpp"

katha::Result katha::LoadMesh(
	Mesh* out_mesh,
	File& file
)
{
	if (nullptr == out_mesh)
	{
		LogLine("error-kbm: passed null to `out_mesh` in `load_mesh`");
		return Result::ERROR_VALUE_NULL;
	}

	constexpr char MAGIC[] = { 0, 'K', 'B', 'M', 0 };
	char magic[5] = {};
	if (5 != file.read(magic, 5))
	{
		LogLine("error-kbm: failed to read magic");
		return Result::ERROR_VALUE_NULL;
	}
	for (int i = 0; i < 5; i++)
	{
		if (magic[i] != MAGIC[i])
		{
			LogLine("error-kbm: incorrect magic");
			return Result::ERROR_VALUE_UNEXPECTED;
		}
	}

	uint32_t vertex_count = 0;
	if (!file.read<uint32_t>(&vertex_count))
	{
		LogLine("error-kbm: failed to read vertex count");
		return Result::ERROR_VALUE_NULL;
	}
	LogLine("kbm: vertex_count {u}", vertex_count);

	Vertex* vertices = Alloc<Vertex>(vertex_count);
	for (uint32_t i = 0; i < vertex_count; i++)
	{
		Vertex& vertex = vertices[i];
		if (!file.readN(vertex.position.array(), 3))
		{
			Release(vertices);
			LogLine("error-kbm: failed to read vertex position at {u}", i);
			return Result::ERROR_VALUE_NULL;
		}
		if (!file.readN(vertex.uv.array(), 2))
		{
			Release(vertices);
			LogLine("error-kbm: failed to read vertex uv at {u}", i);
			return Result::ERROR_VALUE_NULL;
		}
		if (!file.readN(vertex.normal.array(), 3))
		{
			Release(vertices);
			LogLine("error-kbm: failed to read vertex normal at {u}", i);
			return Result::ERROR_VALUE_NULL;
		}
	}

	Result result = CreateBuffer(
		&(out_mesh->vertex_buffer),
		EnumField<BufferUsage>::None(),
		vertex_count * sizeof(Vertex),
		vertices
	);
	if (!CheckResult(result, "Mesh::CreateBufferVertex"))
	{
		Release(vertices);
		return result;
	}
	Release(vertices);

	result = LoadTexture(&(out_mesh->texture_diffuse), file);
	if (!CheckResult(result, "Mesh::LoadTextureDiffuse"))
	{
		DeleteBuffer(out_mesh->vertex_buffer);
		*out_mesh = {};
		return result;
	}

	return Result::SUCCESS;
}

katha::Result katha::LoadMesh(
	Mesh* out_mesh,
	const char* filename
)
{
	if (nullptr == out_mesh) {
		return Result::ERROR_VALUE_NULL;
	}

	File file = Platform::Get()->openFileRead(filename);
	if (!file) {
		return Result::ERROR;
	}

	Result result = LoadMesh(out_mesh, file);
	file.close();

	return result;
}