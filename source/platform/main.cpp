#include "platform.hpp"
#include "gl/gl.hpp"
#include "../katha/time/time.hpp"
#include "../katha/physics/vertex.hpp"
#include "../katha/physics/transform.hpp"
#include "../katha/math/vector2.hpp"
#include "../katha/math/quaternion.hpp"
#include "../katha/math/matrix4.hpp"

#if KATHA_XR
#include "xr/xr.hpp"
#endif

#include "../game/action_map.hpp"
#include "../game/highway.hpp"

#include <clocale>

#include <SDL2/SDL.h>

using namespace katha;

constexpr const char* vertex_shader = ""
"#version 310 es\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec2 tex_coords;\n"
"layout (location = 0) uniform mat4 mvp;\n"
"out vec2 uv;\n"
"void main()\n"
"{\n"
	"gl_Position = mvp * vec4(position, 1);\n"
	"uv = tex_coords;"
"}\n"
;

constexpr const char* fragment_shader = ""
"#version 310 es\n"
"precision highp float;\n"
"in vec2 uv;\n"
"layout (location = 0) out vec4 color;\n"
"layout (binding = 0, location = 1) uniform sampler2D diffuse;\n"
"void main()\n"
"{\n"
	"color = texture(diffuse, uv);\n"
"}\n"
;

constexpr vertex_t vertices[] = {
	{ .position = vec3(-1, -1,  1), .uv = vertex_t::unorm(vec2(0, 0)) },
	{ .position = vec3( 1, -1,  1), .uv = vertex_t::unorm(vec2(1, 0)) },
	{ .position = vec3( 1,  1,  1), .uv = vertex_t::unorm(vec2(1, 1)) },
	{ .position = vec3(-1, -1,  1), .uv = vertex_t::unorm(vec2(0, 0)) },
	{ .position = vec3( 1,  1,  1), .uv = vertex_t::unorm(vec2(1, 1)) },
	{ .position = vec3(-1,  1,  1), .uv = vertex_t::unorm(vec2(0, 1)) },
	{ .position = vec3( 1, -1, -1), .uv = vertex_t::unorm(vec2(0, 0)) },
	{ .position = vec3(-1, -1, -1), .uv = vertex_t::unorm(vec2(1, 0)) },
	{ .position = vec3(-1,  1, -1), .uv = vertex_t::unorm(vec2(1, 1)) },
	{ .position = vec3( 1, -1, -1), .uv = vertex_t::unorm(vec2(0, 0)) },
	{ .position = vec3(-1,  1, -1), .uv = vertex_t::unorm(vec2(1, 1)) },
	{ .position = vec3( 1,  1, -1), .uv = vertex_t::unorm(vec2(0, 1)) },
	{ .position = vec3( 1, -1,  1), .uv = vertex_t::unorm(vec2(0, 0)) },
	{ .position = vec3( 1, -1, -1), .uv = vertex_t::unorm(vec2(1, 0)) },
	{ .position = vec3( 1,  1, -1), .uv = vertex_t::unorm(vec2(1, 1)) },
	{ .position = vec3( 1, -1,  1), .uv = vertex_t::unorm(vec2(0, 0)) },
	{ .position = vec3( 1,  1, -1), .uv = vertex_t::unorm(vec2(1, 1)) },
	{ .position = vec3( 1,  1,  1), .uv = vertex_t::unorm(vec2(0, 1)) },
	{ .position = vec3(-1, -1, -1), .uv = vertex_t::unorm(vec2(0, 0)) },
	{ .position = vec3(-1, -1,  1), .uv = vertex_t::unorm(vec2(1, 0)) },
	{ .position = vec3(-1,  1,  1), .uv = vertex_t::unorm(vec2(1, 1)) },
	{ .position = vec3(-1, -1, -1), .uv = vertex_t::unorm(vec2(0, 0)) },
	{ .position = vec3(-1,  1,  1), .uv = vertex_t::unorm(vec2(1, 1)) },
	{ .position = vec3(-1,  1, -1), .uv = vertex_t::unorm(vec2(0, 1)) },
	{ .position = vec3(-1,  1,  1), .uv = vertex_t::unorm(vec2(0, 0)) },
	{ .position = vec3( 1,  1,  1), .uv = vertex_t::unorm(vec2(1, 0)) },
	{ .position = vec3( 1,  1, -1), .uv = vertex_t::unorm(vec2(1, 1)) },
	{ .position = vec3(-1,  1,  1), .uv = vertex_t::unorm(vec2(0, 0)) },
	{ .position = vec3( 1,  1, -1), .uv = vertex_t::unorm(vec2(1, 1)) },
	{ .position = vec3(-1,  1, -1), .uv = vertex_t::unorm(vec2(0, 1)) },
	{ .position = vec3(-1, -1, -1), .uv = vertex_t::unorm(vec2(0, 0)) },
	{ .position = vec3( 1, -1, -1), .uv = vertex_t::unorm(vec2(1, 0)) },
	{ .position = vec3( 1, -1,  1), .uv = vertex_t::unorm(vec2(1, 1)) },
	{ .position = vec3(-1, -1, -1), .uv = vertex_t::unorm(vec2(0, 0)) },
	{ .position = vec3( 1, -1,  1), .uv = vertex_t::unorm(vec2(1, 1)) },
	{ .position = vec3(-1, -1,  1), .uv = vertex_t::unorm(vec2(0, 1)) }
};

struct gl_scene_t
{
	pso_t pso = {};
	texture_t checker_board_texture = {};
	buffer_t vertex_buffer = {};

	float angle = 0.0f;

	void update()
	{
		angle += 0.01f;
		if (angle > (PI * 2))
		{
			angle = 0.0f;
		}
	}

	bool create();
	void clear();

	void draw(
		const highway_t& highway,
		const katha::transform_t& camera,
		const katha::vec2& size
	);
};

bool gl_scene_t::create()
{
	result_e result = gl->create_pso(&pso,
		vertex_layout_e::f3_usn2,
		vertex_shader, fragment_shader,
		blend_mode_e::none,
		depth_mode_e::less
	);
	if (!check_result(result, "gl::create_pso"))
	{
		return false;
	}

	result = gl->create_array_buffer(&vertex_buffer, sizeof(vertices), vertices);
	if (!check_result(result, "gl::create_array_buffer"))
	{
		return false;
	}

	constexpr uint8_t pixels[] = {
		255, 255, 255, 0, 0, 0, 0, 0,
		0, 0, 0, 255, 255, 255, 0, 0
	};
	result = gl->create_texture(
		&checker_board_texture,
		katha::uvec2(2), format_e::rgb8,
		pixels, false
	);
	if (!check_result(result, "gl_scene_t::create::checker_board_texture"))
	{
		return false;
	}

	return true;
}

void gl_scene_t::clear()
{
	gl->delete_texture(&checker_board_texture);
	gl->delete_buffer(&vertex_buffer);
	gl->delete_pso(&pso);
}

void gl_scene_t::draw(
	const highway_t& highway,
	const katha::transform_t& camera,
	const katha::vec2& size
)
{
	const katha::mat4 perspective = gl->perspective_matrix(
		radians(90), size, vec2(0.01, 500)
	);
	const katha::mat4 view = camera.calculate_view_matrix();

	katha::mat4 model = katha::mat4(1.0f);
	model = katha::translate(model, highway.player);
	model = katha::scale(model, vec3(1, 0.5, 1));
	
	katha::mat4 mvp = perspective * view * model;

	gl->use_pso(pso);
	gl->bind_texture(checker_board_texture);
	gl->set_uniform_texture_unit(1, 0);
	
	gl->bind_vertex_buffer(vertex_buffer, 0, 0, sizeof(vertex_t));
	
	// player
	gl->set_uniform_mat4(0, mvp.array());
	gl->draw_arrays(36);

	for (int i = 0; i < highway.traffic_count; i++)
	{
		const highway_t::traffic_t& t = highway.traffic[i];
		if (highway_t::traffic_e::none == t.type)
		{
			continue;
		}

		mat4 tm(1.0f);
		tm = translate(tm, t.position);
		tm = scale(tm, katha::highway_t::get_bb(t.type));
		mvp = perspective * view * tm;

		gl->set_uniform_mat4(0, mvp.array());
		gl->draw_arrays(36);
	}
}

int main(int argc, char** args)
{
	char* locale = setlocale(LC_ALL, "");
	log_line("current locale: {s}", locale);
	log_line("unicode test: {s}", ENGINE_NAME_UTF8);

	platform_t platform = {};
	result_e result = platform.init(argc, args);
	if (!check_result(result, "platform::init"))
	{
		platform.clear();
		return static_cast<int>(result);
	}

	gl_scene_t scene = {};
	if (!scene.create())
	{
		log_line("error: failed to create gl_scene");
		scene.clear();
		platform.clear();
		return 0;
	}

	highway_t highway = {};

	transform_t camera = { .orientation = quat_t::identity(), .position = vec3(0, 3, 9) };

	bool running = true;
	uint64_t last = 0;
	while (running)
	{
		const uint64_t start = now();
		float delta = 0.016f;
		if (last)
		{
			delta = last / static_cast<double>(1e9);
		}

		result = platform.poll_events();
		if (result_e::request_exit == result)
		{
			running = false;
			break;
		}

		const gamepad_t& gp = platform.current_input_state.gamepad;
		action_map_t action_map = platform.get_action_map();

		highway.update(action_map, delta);

		camera.position.x = lerp(camera.position.x, highway.player.x, 5 * delta);
		camera = camera.look_at(highway.player + vec3(0, 3, 0));

		if (platform.config.enable_xr)
		{
#if KATHA_XR
			xr->poll_events();
			xr_t::frame_t xr_frame = {};
			result = xr->begin_frame(&xr_frame);
			if (result_e::request_skip == result)
			{
				continue;
			}
			if (!check_result(result, "xr::begin_frame"))
			{
				running = false;
				break;
			}

			// render begin
			gl->bind_framebuffer(gl->left);
			gl->clear_screen(vec4(0.1, 0.1, 0.1, 1));
			scene.draw(highway,
				camera.offset_by(xr_frame.get_transform(xr_t::EYE_LEFT)),
				(vec2)gl->left.size
			);
			gl->blit_to_framebuffer(gl->left, xr_frame.framebuffer_left());

			gl->bind_framebuffer(gl->right);
			gl->clear_screen(vec4(0.1, 0.1, 0.1, 1));
			scene.draw(highway,
				camera.offset_by(xr_frame.get_transform(xr_t::EYE_RIGHT)),
				(vec2)gl->right.size
			);
			gl->blit_to_framebuffer(gl->right, xr_frame.framebuffer_right());
			// render end

			result = xr->end_frame(xr_frame);
			if (!check_result(result, "xr::end_frame"))
			{
				running = false;
				break;
			}

			// show left eye on window
			const uvec2 size = platform.get_drawable_size();
			gl->bind_framebuffer(framebuffer_t::empty(size));
			// TODO: scale this
			gl->blit_to_screen(gl->left, ivec4(0, 0, size.x, size.y));
			SDL_GL_SwapWindow(platform.window);
#endif
		}
		else
		{
			uvec2 size = platform.get_drawable_size();
			gl->bind_framebuffer(gl->left);
			gl->clear_screen(vec4(0.1, 0.1, 0.1, 1));
			scene.draw(highway, camera, static_cast<vec2>(size));

			gl->bind_framebuffer(framebuffer_t::empty(size));
			gl->blit_to_screen(gl->left, ivec4(0, 0, size.x, size.y));
			
			SDL_GL_SwapWindow(platform.window);
		}

		last = now() - start;
		if (platform.config.log_frame_time)
		{
			log_line("frame_time: {td}, delta: {f}", last, delta);
		}
	}

	scene.clear();
	platform.clear();
	return 0;
}