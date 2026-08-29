#include "resource.hpp"
#include "../enum/graphics.hpp"
#include "../type/string.hpp"
#include "../utility.hpp"

/**
 * magic::byte(5): 0 K B M 0 (null Katha Binary Mesh null)
 * vertex_layout::byte: [mesh]
 * vertex_count::uint32
 * index_count::uint32: 0 < v < INDEX_T_MAX
 * vertices::vertex_t(vertex_count)
 * indices::index_t(index_count)
 */
katha::result_e katha::load(mesh_t* out_mesh, stream_i* stream)
{
	char magic[5] = {};
	if (5 != stream->read(magic, 5))
	{
		return result_e::error;
	}
	if (0 != magic[4])
	{
		return result_e::error_value_unexpected;
	}

	const string_t magic_str = magic + 1;
	if (!magic_str.equals("KBM"))
	{
		log_line("error-resource: not a mesh file");
		return result_e::error_value_unexpected;
	}

	vertex_layout_e layout = vertex_layout_e::mesh;
	if (!stream->read(&layout))
	{
		return result_e::error;
	}

	uint32_t vertex_count = 0;
	if (!stream->read(&vertex_count))
	{
		return result_e::error;
	}
	uint32_t index_count = 0;
	if (!stream->read(&index_count))
	{
		return result_e::error;
	}
	
	log_line("resource-mesh: layout {s}, vertice {u}, indices: {u}",
		vertex_layout_to_cstring(layout),
		vertex_count,
		index_count
	);

	return result_e::success;
}