#pragma once
#ifndef KATHA_GAME_WORLD_H__
#define KATHA_GAME_WORLD_H__ 1

#include "../enum/result.hpp"
#include "../type/buffer.hpp"
#include "../type/texture.hpp"

namespace katha
{
	struct world_t
	{
		buffer_t mesh_buffer = {};
		texture_t texture = {};

		result_e load();
		void clear();
	};
}

#endif