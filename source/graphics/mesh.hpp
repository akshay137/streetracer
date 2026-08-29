#pragma once
#ifndef KATHA_GRAPHICS_MESH_H__
#define KATHA_GRAPHICS_MESH_H__ 1

#include "buffer.hpp"
#include "../type/vertex.hpp"

namespace katha
{
	struct mesh_t
	{
		buffer_t vertex_buffer = {};
		buffer_t index_buffer = {};

		uint32_t vertex_count() const
		{
			return vertex_buffer.size / sizeof(vertex_t);
		}

		uint32_t index_count() const
		{
			return index_buffer.size / sizeof(uint16_t);
		}
	};
}

#endif