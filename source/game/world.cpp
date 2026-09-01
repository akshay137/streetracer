#include "world.hpp"
#include "../graphics/graphics.hpp"
#include "../type/vertex.hpp"

katha::result_e katha::world_t::load()
{
	vertex_t quad[] = {
		vertex_t(vec3(0.5, 0.5, 0), vertex_t::unorm(vec2(1, 1))),
		vertex_t(vec3(0.5, -0.5, 0), vertex_t::unorm(vec2(1, 0))),
		vertex_t(vec3(-0.5, -0.5, 0), vertex_t::unorm(vec2(0, 0))),

		vertex_t(vec3(-0.5, -0.5, 0), vertex_t::unorm(vec2(0, 0))),
		vertex_t(vec3(-0.5, 0.5, 0), vertex_t::unorm(vec2(0, 1))),
		vertex_t(vec3(0.5, 0.5, 0), vertex_t::unorm(vec2(1, 1)))
	};
	mesh_buffer = create_buffer(
		{},
		sizeof(quad),
		quad
	);

	result_t<texture_t> tres = load_texture("logo.kbt");
	if (!tres)
	{
		return tres.result;
	}

	texture = tres.value;

	return result_e::success;
}

void katha::world_t::clear()
{
	delete_texture(texture);
	delete_buffer(mesh_buffer);
}