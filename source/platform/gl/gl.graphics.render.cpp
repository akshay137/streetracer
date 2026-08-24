#include "gl.hpp"
#include "../../katha/core.hpp"
#include "../../katha/math/matrix4.hpp"
#include "../../katha/math/quaternion.hpp"
#include "../../katha/physics/transform.hpp"
#include "../../katha/physics/vertex.hpp"

#include "../../game/highway.hpp"

#include <glad/glad.h>
#include <SDL2/SDL_video.h>

void katha::gl_t::render(
	const highway_t& highway,
	const render_mode_e render_mode,
	const transform_t& camera_left,
	const framebuffer_t& framebuffer_left,
	const transform_t& camera_right,
	const framebuffer_t& framebuffer_right
)
{
	const transform_t* cameras[2] = {
		&camera_left,
		&camera_right
	};
	const framebuffer_t* framebuffers[2] = {
		&(this->framebuffers.left),
		&(this->framebuffers.right)
	};
	
	const uint32_t render_count = static_cast<uint32_t>(render_mode);
	for (uint32_t i = 0; i < render_count; i++)
	{
		const transform_t& camera = *(cameras[i]);
		const framebuffer_t& framebuffer = *(framebuffers[i]);
		const mat4 perspective = get_perspective_projection(
			radians(90),
			static_cast<vec2>(framebuffer.size),
			vec2(0.01, 500)
		);
		const mat4 view = camera.calculate_view_matrix();
		
		bind_framebuffer(framebuffer);
		clear_screen(vec4(0.1, 0.1, 0.1, 0));

		use_pso(pso_mesh);
		bind_texture(highway.checker_board_texture);
		set_uniform_texture_unit(1, 0);

		set_vertex_buffer(highway.vertex_buffer, 0, 0, sizeof(vertex_t));

		// player
		mat4 model = translate(mat4(1.0), highway.player);
		model = scale(model, vec3(1, 0.5, 1));

		mat4 mvp = perspective * view * model;
		set_uniform_mat4(0, mvp.array());
		draw_arrays(36);

		for (int t = 0; t < highway.traffic_count; t++)
		{
			const highway_t::traffic_t& traffic = highway.traffic[t];
			if (highway_t::traffic_e::none == traffic.type)
			{
				continue;
			}

			model = translate(mat4(1.0f), traffic.position);
			model = scale(model, highway_t::get_bb(traffic.type));
			mvp = perspective * view * model;

			set_uniform_mat4(0, mvp.array());
			draw_arrays(36);
		}
	}

	if (framebuffer_left.id)
	{
		blit_to_framebuffer(
			this->framebuffers.left,
			framebuffer_left
		);
	}

	if (
		(render_mode_e::stereo == render_mode)
		&& framebuffer_right.id
	)
	{
		blit_to_framebuffer(
			this->framebuffers.right,
			framebuffer_right
		);
	}
}

katha::result_e katha::gl_t::present_to_screen()
{
	// show left framebuffer on screen
	framebuffer_t screen = {};
	screen.size = this->framebuffers.left.size;
	set_viewport(static_cast<ivec2>(screen.size));
	blit_to_screen(this->framebuffers.left, ivec4(0, 0, screen.size.x, screen.size.y));

	SDL_GL_SwapWindow((SDL_Window*)window);

	return result_e::success;
}