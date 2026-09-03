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

	// uint8_t vertex_layout = 0;
	// if (!file.read<uint8_t>(&vertex_layout))
	// {
	// 	LogLine("error-kbm: failed to read vertex layout");
	// 	return Result::ERROR_VALUE_NULL;
	// }

	uint8_t mesh_count = 0;
	if (!file.read<uint8_t>(&mesh_count))
	{
		LogLine("error-kbm: failed to read mesh count");
		return Result::ERROR_VALUE_NULL;
	}

	LogLine("kbm: mesh_count: {u}", mesh_count);
	out_mesh->mesh_count = mesh_count;

	uint8_t* buffer = nullptr;
	uint32_t buffer_size = 0;
	for (uint8_t i = 0; i < mesh_count; i++)
	{
		uint32_t vertex_count = 0;
		if (!file.read<uint32_t>(&vertex_count))
		{
			LogLine("error-kbm: failed to read vertex count");
			DeleteMesh(*out_mesh);
			return Result::ERROR_VALUE_NULL;
		}

		LogLine("kbm: v {u}", vertex_count);

		const uint32_t vbuffer_size = sizeof(Vertex) * vertex_count;
		
		if (nullptr == buffer)
		{
			buffer_size = vbuffer_size;
			buffer = Alloc<uint8_t>(buffer_size);
		}
		if (buffer_size < vbuffer_size)
		{
			buffer_size = vbuffer_size;
			Release(buffer);
			buffer = Alloc<uint8_t>(buffer_size);
		}

		if (vbuffer_size != file.read(buffer, vbuffer_size))
		{
			LogLine("error-kbm: failed to read vertices");
			Release(buffer);
			DeleteMesh(*out_mesh);
			return Result::ERROR_VALUE_NULL;
		}

		Result result = CreateBuffer(
			out_mesh->vertex_buffers + i,
			EField<BufferUsage>::FromEnum(BufferUsage::STREAM, BufferUsage::ELEMENT),
			vbuffer_size,
			buffer
		);
		if (!CheckResult(result, "mesh::create_vertex_buffer"))
		{
			Release(buffer);
			DeleteMesh(*out_mesh);
			return result;
		}
	}

	Release(buffer);
	return Result::SUCCESS;
}

katha::Result katha::LoadMesh(
	Mesh* out_mesh,
	const char* filename
)
{
	if (nullptr == out_mesh)
	{
		return Result::ERROR_VALUE_NULL;
	}

	File file = Platform::Get()->openFileRead(filename);
	if (!file)
	{
		return Result::ERROR;
	}

	Result result = LoadMesh(out_mesh, file);
	file.close();

	return result;
}