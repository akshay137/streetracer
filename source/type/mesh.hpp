#pragma once
#ifndef KATHA_TYPE_MESH_H__
#define KATHA_TYPE_MESH_H__ 1

#include "buffer.hpp"

namespace katha
{
	struct Mesh
	{
		struct Data
		{
			Buffer vertices = {};
			Buffer indices = {};
		};

		Data data[128] = {};
		uint8_t mesh_count = 0;
	};
}

#endif