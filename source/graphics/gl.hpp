#pragma once
#ifndef KATHA_GRAPHICS_GL_H__
#define KATHA_GRAPHICS_GL_H__ 1

#include "../enum/graphics.hpp"
#include "../enum/result.hpp"
#include "../type/bitfield.hpp"
#include "../type/primitive.hpp"
#include "../type/version.hpp"

#include "../type/buffer.hpp"
#include "../type/texture.hpp"

struct SDL_Window;

namespace katha
{
	struct platform_t;

	struct gl_format_t
	{
		uint32_t internal = 0;
		uint32_t channel = 0;
		uint32_t data_type = 0;

		ivec4 swizzle = {};
	};

	struct gl_t
	{
		void* context = nullptr;
		SDL_Window* window = nullptr;

		version_t version = {};
		bool is_es_context = false;

		enum class extension_e : uint32_t
		{
			gl_khr_debug,

			__max
		};
		efield_t<extension_e> extensions = {};

		uint32_t mesh_shader_program = 0;
		uint32_t mesh_vertex_array = 0;

		result_e init(platform_t* platform);
		void clear();

		bool check_error(const source_t& source = source_t::current());

		bool check_version();
		void query_extensions();
		void query_limits();

		gl_format_t format_to_gl_format(const format_e format);

		bool create_resources();
		void clear_resources();

		void render_world(const struct world_t& world);

		void delete_shader(const uint32_t shader);
		void delete_shader_program(const uint32_t program);
		void delete_vertex_array(const uint32_t vertex_array);
		void delete_buffer(const buffer_t& buffer);
		void delete_texture(const texture_t& texture);

		const char* get_shader_version_cstring() const;
		uint32_t create_shader(const char* source, const uint32_t type);
		uint32_t create_shader_program(
			const char* vertex_shader_source,
			const char* fragment_shader_source
		);
		
		uint32_t create_vertex_array_mesh();

		buffer_t create_buffer(
			efield_t<buffer_usage_e> usage,
			const uint32_t size,
			const void* data
		);

		texture_t create_texture(
			const format_e format,
			const uvec2 size,
			const void* pixels
		);

		void bind_vertex_buffer(
			const buffer_t& buffer,
			uint32_t binding,
			uint32_t stride,
			uint32_t offset = 0
		);

		template <typename T>
		void bind_vertex_buffer(
			const buffer_t& buffer,
			uint32_t binding,
			uint32_t offset = 0
		)
		{
			bind_vertex_buffer(buffer, binding, sizeof(T), offset);
		}

		void bind_texture(const uint32_t slot, const texture_t& texture);
	};
}

#endif