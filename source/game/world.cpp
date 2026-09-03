#include "world.hpp"
#include "../graphics/graphics.hpp"
#include "../type/vertex.hpp"

katha::Result katha::World::load()
{
	Result result = LoadMesh(&mesh, "out.kbm");
	if (!CheckResult(result, "world::load_mesh")) {
		return result;
	}

	result = LoadTexture(&texture, "logo.kbt");
	if (!CheckResult(result, "world::load_texture")) {
		return result;
	}

	return Result::SUCCESS;
}

void katha::World::clear()
{
	DeleteTexture(texture);
	DeleteMesh(mesh);
}