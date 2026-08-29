#pragma once
#ifndef KATHA_RESOURCE_H__
#define KATHA_RESOURCE_H__ 1

#include "../enum/result.hpp"
#include "../type/stream.hpp"
#include "../graphics/mesh.hpp"

namespace katha
{
	result_e load(mesh_t* out_mesh, stream_i* stream);
}

#endif