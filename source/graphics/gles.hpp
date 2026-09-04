#pragma once
#ifndef KATHA_GRAPHICS_GLES_H__
#define KATHA_GRAPHICS_GLES_H__ 1

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
	struct GLESFormat
	{
		uint32_t internal = 0;
		uint32_t channel = 0;
		uint32_t data_type = 0;

		ivec4 swizzle = {};
	};

	struct GLES
	{
		void* context = nullptr;
		SDL_Window* window = nullptr;

		Version version = {};
		bool is_es_context = false;

		enum class Extension : uint32_t
		{
			gl_khr_debug,

			__MAX
		};
		EnumField<Extension> extensions = {};

		uint32_t mesh_shader_program = 0;
		uint32_t mesh_vertex_array = 0;

		Result init();
		void clear();

		bool checkError(const source_t& source = source_t::current());

		bool checkVersion();
		void queryExtensions();
		void queryLimits();

		bool createResources();
		void clearResources();

		void renderWorld(const struct World& world);

		void deleteShader(const uint32_t shader);
		void deleteShaderProgram(const uint32_t program);
		void deleteVertexArray(const uint32_t vertex_array);
		void deleteBuffer(const Buffer& buffer);
		void deleteTexture(const Texture& texture);

		const char* getShaderVersionCString() const;
		uint32_t createShader(const char* source, const uint32_t type);
		uint32_t createShaderProgram(
			const char* vertex_shader_source,
			const char* fragment_shader_source
		);
		
		uint32_t createVertexArrayMesh();

		Result createBuffer(
			Buffer* out_buffer,
			EnumField<BufferUsage> usage,
			const uint32_t size,
			const void* data
		);

		Result createTexture(
			Texture* out_texture,
			const Format format,
			const uvec2 size,
			const void* pixels
		);

		void bindVertexBuffer(
			const Buffer& buffer,
			uint32_t binding,
			uint32_t stride,
			uint32_t offset = 0
		);

		template <typename T>
		void bindVertexBuffer(
			const Buffer& buffer,
			uint32_t binding,
			uint32_t offset = 0
		)
		{
			bindVertexBuffer(buffer, binding, sizeof(T), offset);
		}

		void bindTexture(const uint32_t slot, const Texture& texture);

		static GLESFormat FormatToGLESFormat(const Format format);
	};
}

#endif