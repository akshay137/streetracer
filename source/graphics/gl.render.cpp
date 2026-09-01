#include "gl.hpp"
#include "../game/world.hpp"
#include "../type/vertex.hpp"
#include "../utility.hpp"

#include <glad/glad.h>
#include <SDL2/SDL_video.h>

void katha::gl_t::render_world(const struct world_t& world)
{
	glClearBufferfv(GL_COLOR, 0, vec4(0.1, 0.1, 0.1, 0).array());

	glUseProgram(mesh_shader_program);
	glBindVertexArray(mesh_vertex_array);
	bind_vertex_buffer<vertex_t>(world.mesh_buffer, 0);
	bind_texture(0, world.texture);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	SDL_GL_SwapWindow(window);
}