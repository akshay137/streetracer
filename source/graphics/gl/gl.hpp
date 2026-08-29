#pragma once
#ifndef KATHA_GRAPHICS_GL_H__
#define KATHA_GRAPHICS_GL_H__ 1

#include "../graphics.hpp"
#include "../../type/version.hpp"
#include "../../config.hpp"

struct SDL_Window;

namespace katha
{
	struct gl_t : public graphics_i
	{
		struct format_t
		{
			int32_t internal_format = 0;
			int32_t channel = 0;
			int32_t data_type = 0;
		};

		void* context = nullptr;
		void* window = nullptr;
		version_t version = {};

		int32_t max_texture_size = 0;

		uint32_t es_context:1 = 0;
		uint32_t ext_debug:1 = 0;

		result_e init(const config_t& config, SDL_Window* window);
		void clear();

		void check_extensions();

		void clear_screen(const vec4& color);

		/*
			graphics_i overrides - begin
		*/
		
		result_e create_buffer(
			buffer_t* out_buffer,
			eflags_t<buffer_usage_e> usage,
			const uint32_t size,
			const void* data = nullptr
		) override;
		void delete_buffer(buffer_t* buffer) override;

		result_e create_pso(
			pso_t* out_pso,
			const vertex_layout_e vertex_layout,
			const char* vertex_shader_source,
			const char* fragment_shader_source,
			const blend_mode_e blend_mode,
			const depth_mode_e depth_mode
		) override;
		void delete_pso(pso_t* pso) override;

		result_e create_texture(
			texture_t* out_texture,
			const uvec2 size,
			const format_e format,
			const void* data = nullptr
		) override;
		void delete_texture(texture_t* texture) override;

		result_e create_framebuffer_from_texture(
			framebuffer_t* out_framebuffer,
			const texture_t& color_0,
			const texture_t& depth_stencil
		) override;

		result_e create_framebuffer(
			framebuffer_t* out_framebuffer,
			const uvec2 size,
			const format_e color_format,
			const format_e depth_stencil_format
		) override;
		void delete_framebuffer(framebuffer_t* framebuffer) override;

		result_e create_mesh(
			mesh_t* out_mesh,
			const vertex_t* vertices,
			const index_t* indices,
			const uint32_t vertex_count,
			const uint32_t index_count
		) override;
		void delete_mesh(mesh_t* mesh) override;

		mat4 get_perspective_projection(
			const float vertical_fov_radians,
			const vec2 screen_size,
			const vec2 z_range
		) override;

		void render(
			const world_t& world,
			const render_mode_e render_mode,
			const transform_t& camera_left_offset,
			const framebuffer_t& framebuffer_left,
			const transform_t& camera_right_offset,
			const framebuffer_t& framebuffer_right
		) override;

		result_e present_to_screen() override;

		/*
			graphics_i overrides - end
		*/

		const char* get_shader_version_string() const;

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

		void bind_texture(const texture_t& texture, const uint32_t slot = 0);

		bool set_blend_mode(const blend_mode_e mode);
		bool set_depth_mode(const depth_mode_e mode);

		void use_pso(const pso_t& pso);

		void set_uniform_texture_unit(const int32_t location, const uint32_t unit);
		void set_uniform_vec3(const int32_t location, const vec3& v);
		void set_uniform_quat(const int32_t location, const quat_t& a);
		void set_uniform_mat4(const int32_t location, const float* m);

		void set_vertex_buffer(
			const buffer_t& buffer,
			const uint32_t index,
			const uint32_t offset = 0,
			const uint32_t stride = 0
		);

		uint32_t create_vao_vertex_t(const bool bind = false);
		
		void draw_arrays(uint32_t vertices);

		void draw_mesh(const mesh_t& mesh, const mat4& mvp);

		void set_viewport(const ivec2 size);

		static format_t format_to_gl_format(const format_e format);
	};

	extern gl_t * const gl;
}

#endif