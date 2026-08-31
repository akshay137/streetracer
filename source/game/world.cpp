#include "world.hpp"
#include "../platform/platform.hpp"
#include "../type/vertex.hpp"

katha::result_e katha::world_t::load(platform_t& platform)
{
	vertex_t triangle[] = {
		vertex_t(vec3(0, 0.5, 0), vertex_t::unorm(vec2(0.5, 1))),
		vertex_t(vec3(0.5, -0.5, 0), vertex_t::unorm(vec2(1, 0))),
		vertex_t(vec3(-0.5, -0.5, 0), vertex_t::unorm(vec2(0, 0)))
	};
	mesh_buffer = platform.gl.create_buffer(
		efield_t<buffer_usage_e>::from_enum(buffer_usage_e::array),
		sizeof(triangle),
		triangle
	);
	return result_e::success;
}

void katha::world_t::clear(platform_t& platform)
{
	platform.gl.delete_buffer(mesh_buffer);
}