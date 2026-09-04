#include "world.hpp"
#include "../graphics/graphics.hpp"
#include "../type/vertex.hpp"

katha::Result katha::World::load()
{
	Result result = LoadMesh(&mesh, "cube.kbm");
	if (!CheckResult(result, "world::load_mesh")) {
		return result;
	}

	return Result::SUCCESS;
}

void katha::World::clear()
{
	DeleteMesh(mesh);
}