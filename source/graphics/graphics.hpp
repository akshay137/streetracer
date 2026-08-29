#pragma once
#ifndef KATHA_GRAPHICS_H__
#define KATHA_GRAPHICS_H__ 1

#include "../enum/result.hpp"
#include "../enum/graphics.hpp"
#include "../type/flags.hpp"
#include "../type/transform.hpp"
#include "../type/vertex.hpp"

#include "buffer.hpp"
#include "framebuffer.hpp"
#include "mesh.hpp"
#include "pso.hpp"
#include "texture.hpp"

namespace katha
{
	struct transform_t;
	
	struct world_t;

	// thin interface for platform agnostic code to use, e.g., load_level
	// and some common resources used throughout
	struct graphics_i
	{
		using index_t = uint16_t;

		struct
		{
			// `left` doubles as main framebuffer for non-xr mode
			framebuffer_t left = {};
			framebuffer_t right = {};
		} framebuffers = {};

		// standard mesh pso
		pso_t pso_mesh = {};

		virtual result_e create_buffer(
			buffer_t* out_buffer,
			eflags_t<buffer_usage_e> usage,
			const uint32_t size,
			const void* data = nullptr
		) = 0;
		virtual void delete_buffer(buffer_t* buffer) = 0;

		virtual result_e create_pso(
			pso_t* out_pso,
			const vertex_layout_e vertex_layout,
			const char* vertex_shader_source,
			const char* fragment_shader_source,
			const blend_mode_e blend_mode,
			const depth_mode_e depth_mode
		) = 0;
		virtual void delete_pso(pso_t* pso);

		virtual result_e create_texture(
			texture_t* out_texture,
			const uvec2 size,
			const format_e format,
			const void* data = nullptr
		) = 0;
		virtual void delete_texture(texture_t* texture) = 0;

		virtual result_e create_framebuffer_from_texture(
			framebuffer_t* out_framebuffer,
			const texture_t& color_0,
			const texture_t& depth_stencil
		) = 0;
		virtual result_e create_framebuffer(
			framebuffer_t* out_framebuffer,
			const uvec2 size,
			const format_e color_format,
			const format_e depth_stencil_format
		) = 0;
		virtual void delete_framebuffer(framebuffer_t* framebuffer) = 0;

		virtual result_e create_mesh(
			mesh_t* out_mesh,
			const vertex_t* vertices,
			const index_t* indices,
			const uint32_t vertex_count,
			const uint32_t index_count
		) = 0;
		virtual void delete_mesh(mesh_t* mesh) = 0;

		virtual mat4 get_perspective_projection(
			const float vertical_fov_radians,
			const vec2 screen_size,
			const vec2 z_range
		) = 0;

		virtual void render(
			const world_t& world,
			const render_mode_e render_mode,
			const transform_t& camera_left_offset,
			const framebuffer_t& framebuffer_left,
			const transform_t& camera_right_offset,
			const framebuffer_t& framebuffer_right
		) = 0;

		void render(const world_t& world)
		{
			render(
				world,
				render_mode_e::mono,
				{}, // camera_left_offset
				{}, // framebuffer_left (blit to screen)
				{}, // camera_right
				{} // framebuffer_right
			);
		}

		virtual result_e present_to_screen() = 0;
	};
}

#endif