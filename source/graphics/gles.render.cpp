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
	
	bindTexture(0, world.texture);

	for (uint8_t i = 0; i < world.mesh.mesh_count; i++)
	{
		Buffer vbuffer = world.mesh.vertex_buffers[i];
		bindVertexBuffer<Vertex>(vbuffer, 0);
	
		glDrawArrays(GL_TRIANGLES, 0, vbuffer.size / sizeof(Vertex));
	}

	SDL_GL_SwapWindow(window);
}