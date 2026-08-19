#pragma once
#ifndef KATHA_GRAPHICS_GL_H__
#define KATHA_GRAPHICS_GL_H__ 1

#include "../../katha/core.hpp"

#include <SDL2/SDL_video.h>

namespace katha
{
	struct gl_t
	{
		struct format_t
		{
			int32_t internal_format = 0;
			int32_t channel = 0;
			int32_t data_type = 0;
		};

		struct framebuffer_t
		{
			uint32_t framebuffer = 0;
			uint32_t color_texture = 0;
			uint32_t depth_texture = 0;
			ivec2 size = {};
		};

		void* context = nullptr;

		// framebuffers must be created after gl_t::init returns by caller
		framebuffer_t left = {}; // main framebuffer for non-vr mode
		framebuffer_t right = {};

		int32_t max_texture_size = 0;

		uint32_t ext_debug:1 = 0;

		result_e init(const config_t& config, SDL_Window* window);
		void clear();

		void check_extensions();

		void clear_screen(const vec4& color);

		uint32_t create_framebuffer_from_texture(const uint32_t texture_color);
		void delete_framebuffer(const uint32_t framebuffer);
		void bind_framebuffer(const uint32_t framebuffer);

		framebuffer_t create_framebuffer(
			const ivec2 size,
			const format_e color_texture_format,
			const format_e depth_texture_format
		);
		void delete_framebuffer(const framebuffer_t& framebuffer);
		void bind_framebuffer(const framebuffer_t& framebuffer);
		void blit_to_screen(
			const framebuffer_t& framebuffer,
			const ivec4& screen_rect,
			const bool filter_linear = false
		);
		void blit_to_framebuffer(
			const framebuffer_t& source,
			const framebuffer_t& target,
			const bool filter_linear = false
		);

		uint32_t create_texture(
			const ivec2 size,
			const format_e format,
			const void* data = nullptr,
			const bool generate_mipmaps = false
		);
		void delete_texture(const uint32_t texture);
		void bind_texture(const uint32_t texture, const uint32_t slot = 0);

		uint32_t create_shader_program(
			const char* vertex_shader_source,
			const char* fragment_shader_source
		);
		void delete_shader_program(const uint32_t program);
		void use_shader_program(const uint32_t program);

		void set_uniform_texture_unit(const int32_t location, const uint32_t unit);
		void set_uniform_vec3(const int32_t location, const vec3& v);
		void set_uniform_quat(const int32_t location, const quat_t& a);
		void set_uniform_mat4(const int32_t location, const float* m);

		uint32_t create_array_buffer(const uint32_t size, const void* data = nullptr);
		void delete_buffer(const uint32_t buffer);

		void bind_vertex_buffer(
			const uint32_t index,
			const uint32_t buffer,
			const uint32_t offset = 0,
			const uint32_t stride = 0
		);

		uint32_t create_vao(const bool bind = false);
		uint32_t create_vao_vertex_t(const bool bind = false);
		void delete_vao(const uint32_t vao);
		void bind_vao(const uint32_t vao);

		void set_vertex_attribute_float(
			const uint32_t attribute_index,
			const uint32_t binding_index,
			const uint32_t size,
			const uint32_t offset
		);
		
		void draw_arrays(uint32_t vertices);

		void set_viewport(const ivec2 size);

		static format_t format_to_gl_format(const format_e format);
		
		static mat4 perspective_matrix(
			const float vertical_fov_radians,
			const vec2 screen_size,
			const vec2 z_range
		);
	};

	extern gl_t * const gl;
}

#endif