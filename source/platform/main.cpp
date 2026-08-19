#include "platform.hpp"
#include "gl/gl.hpp"
#include "../katha/graphics/vertex.hpp"
#include "../katha/time/time.hpp"
#include "../katha/physics/transform.hpp"
#include "../katha/math/vector2.hpp"
#include "../katha/math/quaternion.hpp"
#include "../katha/math/matrix4.hpp"

#if KATHA_XR
#include "xr/xr.hpp"
#endif

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
	uint32_t shader_program = 0;
	uint32_t vao = 0;
	uint32_t vertex_buffer = 0;
	uint32_t checker_board_texture = 0;

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

	void draw(const katha::transform_t& camera, const katha::vec2& size);
};

bool gl_scene_t::create()
{
	shader_program = gl->create_shader_program(vertex_shader, fragment_shader);
	if (0 == shader_program)
	{
		return false;
	}

	vertex_buffer = gl->create_array_buffer(sizeof(vertices), vertices);
	if (0 == vertex_buffer)
	{
		return false;
	}

	vao = gl->create_vao_vertex_t();
	if (0 == vao)
	{
		return false;
	}

	constexpr uint8_t pixels[] = {
		255, 255, 255, 0, 0, 0, 0, 0,
		0, 0, 0, 255, 255, 255, 0, 0
	};
	checker_board_texture = gl->create_texture(
		katha::ivec2(2), format_e::rgb8,
		pixels, false
	);
	if (0 == checker_board_texture)
	{
		return false;
	}

	return true;
}

void gl_scene_t::clear()
{
	gl->delete_texture(checker_board_texture);
	gl->delete_vao(vao);
	gl->delete_buffer(vertex_buffer);
	gl->delete_shader_program(shader_program);
}

void gl_scene_t::draw(const katha::transform_t& camera, const katha::vec2& size)
{
	const katha::mat4 perspective = gl->perspective_matrix(
		radians(90), size, vec2(0.01, 100)
	);
	const katha::mat4 view = camera.calculate_view_matrix();

	katha::mat4 model = katha::mat4(1.0f);
	model = katha::translate(model, katha::vec3(0, 0, 1));
	const float s = sin(angle * 0.5f);
	const float c = cos(angle * 0.5f);
	model = model * to_mat4(quat_t(0, s, 0, c));
	
	katha::mat4 mvp = perspective * view * model;

	gl->use_shader_program(shader_program);
	gl->bind_texture(checker_board_texture);
	gl->set_uniform_texture_unit(1, 0);
	gl->set_uniform_mat4(0, mvp.array());

	gl->bind_vao(vao);
	gl->bind_vertex_buffer(0, vertex_buffer, 0, sizeof(vertex_t));

	gl->draw_arrays(36);
}

#include "file.hpp"

int main(int argc, char** args)
{
	char* locale = setlocale(LC_ALL, nullptr);
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

	transform_t camera = { .orientation = quat_t::identity(), .position = vec3(0, -2, 5) };
	camera = camera.look_at(vec3(0));

	bool running = true;
	while (running)
	{
		const uint64_t start = now();
		result = platform.poll_events();
		if (result_e::request_exit == result)
		{
			running = false;
			break;
		}

		scene.update();

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
			scene.draw(camera.offset_by(xr_frame.get_transform(xr_t::EYE_LEFT)), (vec2)gl->left.size);
			gl->blit_to_framebuffer(gl->left, xr->get_gl_framebuffer(xr_t::EYE_LEFT, xr_frame));

			gl->bind_framebuffer(gl->right);
			gl->clear_screen(vec4(0.1, 0.1, 0.1, 1));
			scene.draw(camera.offset_by(xr_frame.get_transform(xr_t::EYE_RIGHT)), (vec2)gl->right.size);
			gl->blit_to_framebuffer(gl->right, xr->get_gl_framebuffer(xr_t::EYE_RIGHT, xr_frame));
			// render end

			result = xr->end_frame(xr_frame);
			if (!check_result(result, "xr::end_frame"))
			{
				running = false;
				break;
			}

			// show left eye on window
			const ivec2 size = platform.get_drawable_size();
			gl->set_viewport(size);
			// TODO: scale this
			gl->blit_to_screen(gl->left, ivec4(0, 0, size.x, size.y));
			SDL_GL_SwapWindow(platform.window);
#endif
		}
		else
		{
			vec2 size = (vec2)platform.get_drawable_size();
			gl->bind_framebuffer(gl->left);
			gl->clear_screen(vec4(0.1, 0.1, 0.1, 1));
			scene.draw(camera, size);

			gl->bind_framebuffer(0);
			gl->set_viewport((ivec2)size);
			gl->blit_to_screen(gl->left, ivec4(0, 0, size.x, size.y));
			
			SDL_GL_SwapWindow(platform.window);
		}

		if (platform.config.log_frame_time)
		{
			log_line("frame_time: {td}", now() - start);
		}
	}

	scene.clear();
	platform.clear();
	return 0;
}