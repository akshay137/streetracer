#pragma once
#ifndef KATHA_GRAPHICS_GL_H__
#define KATHA_GRAPHICS_GL_H__ 1

#include "../enum/graphics.hpp"
#include "../enum/result.hpp"
#include "../type/bitfield.hpp"
#include "../type/primitive.hpp"
#include "../type/version.hpp"

struct SDL_Window;

namespace katha
{
	struct platform_t;

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

		bool create_resources();
		void clear_resources();

		void render_world(const struct world_t& world);

		void delete_shader(const uint32_t shader);
		void delete_shader_program(const uint32_t program);
		void delete_vertex_array(const uint32_t vertex_array);
		void delete_buffer(const uint32_t buffer);

		const char* get_shader_version_cstring() const;
		uint32_t create_shader(const char* source, const uint32_t type);
		uint32_t create_shader_program(
			const char* vertex_shader_source,
			const char* fragment_shader_source
		);
		
		uint32_t create_vertex_array_mesh();

		uint32_t create_buffer(
			efield_t<buffer_usage_e> usage,
			const uint32_t size,
			const void* data
		);
	};
}

#endif