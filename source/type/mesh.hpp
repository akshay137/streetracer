#pragma once
#ifndef KATHA_TYPE_MESH_H__
#define KATHA_TYPE_MESH_H__ 1

#include "buffer.hpp"
#include "texture.hpp"

namespace katha
{
	struct Mesh
	{
		Buffer vertex_buffer = {};
		Texture texture_diffuse = {};
	};
}

#endif