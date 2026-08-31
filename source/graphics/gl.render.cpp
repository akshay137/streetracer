#include "gl.hpp"
#include "../game/world.hpp"
#include "../type/vertex.hpp"
#include "../utility.hpp"

#include <glad/glad.h>
#include <SDL2/SDL_video.h>

void katha::gl_t::render_world(const struct world_t& world)
{
	glUseProgram(mesh_shader_program);
	glBindVertexArray(mesh_vertex_array);
	glBindVertexBuffer(0, world.mesh_buffer, 0, sizeof(vertex_t));

	glDrawArrays(GL_TRIANGLES, 0, 3);

	SDL_GL_SwapWindow(window);
}