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

	for (uint8_t i = 0; i < world.mesh.mesh_count; i++)
	{
		Mesh::Data node = world.mesh.data[i];
		bindVertexBuffer<Vertex>(node.vertices, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLuint>(node.indices.handle));
		bindTexture(0, world.texture);
	
		glDrawElements(
			GL_TRIANGLES,
			static_cast<GLsizei>(node.indices.size / sizeof(uint32_t)),
			GL_UNSIGNED_INT,
			0
		);
	}

	SDL_GL_SwapWindow(window);
}