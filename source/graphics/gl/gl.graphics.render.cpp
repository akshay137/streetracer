#include "gl.hpp"
#include "../mesh.hpp"
#include "../../math/matrix4.hpp"
#include "../../math/quaternion.hpp"
#include "../../type/transform.hpp"

#include "../../game/world.hpp"

#include <SDL2/SDL_video.h>
#include "glad/glad.h"

void katha::gl_t::render(
	const world_t& world,
	const render_mode_e render_mode,
	const transform_t& camera_left_offset,
	const framebuffer_t& framebuffer_left,
	const transform_t& camera_right_offset,
	const framebuffer_t& framebuffer_right
)
{
	const transform_t camera_left = world.camera.offset_by(camera_left_offset);
	const transform_t camera_right = world.camera.offset_by(camera_right_offset);
	const transform_t* cameras[2] = {
		&camera_left,
		&camera_right
	};

	const framebuffer_t* render_framebuffers[2] = {
		&(framebuffers.left),
		&(framebuffers.right)
	};
	
	const uint32_t render_count = static_cast<uint32_t>(render_mode);
	for (uint32_t i = 0; i < render_count; i++)
	{
		const transform_t& camera = *(cameras[i]);
		const framebuffer_t& framebuffer = *(render_framebuffers[i]);
		const mat4 perspective = get_perspective_projection(
			radians(45.0f),
			static_cast<vec2>(framebuffer.size),
			vec2(0.01, 500)
		);
		mat4 view = camera.calculate_view_matrix();
		// view[2] = -view[2];
		
		bind_framebuffer(framebuffer);
		clear_screen(vec4(0.1, 0.1, 0.1, 0));

		use_pso(pso_mesh);
		bind_texture(world.checker_board_texture);
		set_uniform_texture_unit(1, 0);

		// player
		mat4 model = world.player.transform.calculate_world_matrix();

		mat4 mvp = perspective * view * model;
		draw_mesh(world.vehicle_mesh, mvp);

		// player forward marker
		transform_t marker = world.player.transform.offset_by({
			.orientation = quat_t::identity(),
			.position = vec3(0, 0, 4)
		});
		model = marker.calculate_world_matrix();
		mvp = perspective * view * model;
		draw_mesh(world.vehicle_mesh, mvp);

		for (int t = 0; t < world.traffic_count; t++)
		{
			const world_t::traffic_t& traffic = world.traffic[t];
			if (world_t::traffic_e::none == traffic.type)
			{
				continue;
			}

			model = translate(mat4(1.0f), traffic.position);
			model = scale(model, world_t::get_bb(traffic.type));
			mvp = perspective * view * model;

			draw_mesh(world.vehicle_mesh, mvp);
		}
	}

	if (framebuffer_left.id)
	{
		blit_to_framebuffer(
			framebuffers.left,
			framebuffer_left
		);
	}

	if (
		(render_mode_e::stereo == render_mode)
		&& framebuffer_right.id
	)
	{
		blit_to_framebuffer(
			framebuffers.right,
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