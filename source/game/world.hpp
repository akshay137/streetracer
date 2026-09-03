#pragma once
#ifndef KATHA_GAME_WORLD_H__
#define KATHA_GAME_WORLD_H__ 1

#include "../enum/result.hpp"
#include "../type/buffer.hpp"
#include "../type/mesh.hpp"
#include "../type/texture.hpp"

namespace katha
{
	struct World
	{
		Mesh mesh = {};
		Texture texture = {};

		Result load();
		void clear();
	};
}

#endif