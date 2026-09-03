#pragma once
#ifndef KATHA_TYPE_MESH_H__
#define KATHA_TYPE_MESH_H__ 1

#include "buffer.hpp"
#include "vector.hpp"

namespace katha
{
	struct Mesh
	{
		uint8_t mesh_count = 0;
		Buffer vertex_buffers[128] = {};
	};
}

#endif