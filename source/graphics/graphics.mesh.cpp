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

	uint8_t vertex_layout = 0;
	if (!file.read<uint8_t>(&vertex_layout))
	{
		LogLine("error-kbm: failed to read vertex layout");
		return Result::ERROR_VALUE_NULL;
	}

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
		uint32_t index_count = 0;
		if (!file.read<uint32_t>(&index_count))
		{
			LogLine("error-kbm: failed to read index count");
			DeleteMesh(*out_mesh);
			return Result::ERROR_VALUE_NULL;
		}

		LogLine("kbm: v {u}, i {u}", vertex_count, index_count);

		const uint32_t vbuffer_size = sizeof(Vertex) * vertex_count;
		const uint32_t ibuffer_size = sizeof(uint32_t) * index_count;
		
		if (nullptr == buffer)
		{
			buffer_size = vbuffer_size + ibuffer_size;
			buffer = Alloc<uint8_t>(buffer_size);
		}
		if (buffer_size < vbuffer_size + ibuffer_size)
		{
			buffer_size = vbuffer_size + ibuffer_size;
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
		if (ibuffer_size != file.read(buffer + vbuffer_size, ibuffer_size))
		{
			LogLine("error-kbm: failed to read indices");
			Release(buffer);
			DeleteMesh(*out_mesh);
			return Result::ERROR_VALUE_NULL;
		}

		Result result = CreateBuffer(
			&(out_mesh->data[i].vertices),
			{},
			vbuffer_size,
			buffer
		);
		if (!CheckResult(result, "mesh::create_vertex_buffer"))
		{
			Release(buffer);
			DeleteMesh(*out_mesh);
			return result;
		}

		result = CreateBuffer(
			&(out_mesh->data[i].indices),
			EField<BufferUsage>::FromEnum(BufferUsage::ELEMENT),
			ibuffer_size,
			buffer + vbuffer_size
		);
		if (!CheckResult(result, "mesh::create_index_buffer"))
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