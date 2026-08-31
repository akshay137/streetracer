#pragma once
#ifndef KATHA_GAME_WORLD_H__
#define KATHA_GAME_WORLD_H__ 1

#include "../enum/result.hpp"
#include "../type/primitive.hpp"

namespace katha
{
	struct world_t
	{
		uint32_t mesh_buffer = 0;

		result_e load(struct platform_t& platform);
		void clear(struct platform_t& platform);
	};
}

#endif