#pragma once
#ifndef KATHA_GRAPHICS_H__
#define KATHA_GRAPHICS_H__ 1

#include "../core/enums.hpp"
#include "../core/types.hpp"

#include "buffer.hpp"
#include "framebuffer.hpp"
#include "pso.hpp"
#include "texture.hpp"

namespace katha
{
	// thin interface for platform agnostic code to use, e.g., load_level
	struct graphics_i
	{
		virtual result_e create_array_buffer(
			buffer_t* out_buffer,
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

		virtual mat4 get_perspective_projection(
			const float vertical_fov_radians,
			const vec2 screen_size,
			const vec2 z_range
		) = 0;
	};
}

#endif