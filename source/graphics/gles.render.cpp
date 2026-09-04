#include "gles.hpp"
#include "../game/world.hpp"
#include "../type/vertex.hpp"
#include "../utility.hpp"

#include <glad/glad.h>
#include <SDL2/SDL_video.h>

void katha::GLES::renderWorld(const World& world)
{
	glClearBufferfv(GL_COLOR, 0, vec4(0.1, 0.1, 0.1, 0).array());

	glUseProgram(mesh_shader_program);
	glBindVertexArray(mesh_vertex_array);
	
	Buffer vbuffer = world.mesh.vertex_buffer;
	bindVertexBuffer<Vertex>(vbuffer, 0);
	bindTexture(0, world.mesh.texture_diffuse);

	glDrawArrays(GL_TRIANGLES, 0, vbuffer.size / sizeof(Vertex));

	SDL_GL_SwapWindow(window);
}