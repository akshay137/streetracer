#include "gl.hpp"
#include "../../math/vector2.hpp"
#include "../../utility.hpp"

#include "glad/glad.h"

katha::result_e katha::gl_t::create_buffer(
	buffer_t* out_buffer,
	eflags_t<buffer_usage_e> usage,
	const uint32_t size,
	const void* data
)
{
	if (nullptr == out_buffer)
	{
		return result_e::error_value_null;
	}

	GLenum buffer_type = 0;
	if (usage.has(buffer_usage_e::data))
	{
		buffer_type = GL_ARRAY_BUFFER;
	}
	else if (usage.has(buffer_usage_e::index))
	{
		buffer_type = GL_ELEMENT_ARRAY_BUFFER;
	}

	if (0 == buffer_type)
	{
		log_line("gl::create_buffer buffer_usage must be buffer_usage_e::data or buffer_usage_e::index");
		return result_e::error_value_unexpected;
	}

	GLuint buffer = 0;
	glGenBuffers(1, &buffer);
	if (0 == buffer)
	{
		log_line("error-gl: failed to create buffer of size {u}", size);
		return result_e::error_gl;
	}

	glBindBuffer(buffer_type, buffer);
	glBufferData(buffer_type, static_cast<GLsizeiptr>(size), data, GL_STATIC_DRAW);

	out_buffer->id = buffer;
	out_buffer->size = size;

	return result_e::success;
}

void katha::gl_t::delete_buffer(buffer_t* buffer)
{
	if (nullptr == buffer)
	{
		return;
	}

	GLuint gl_buffer = static_cast<GLuint>(buffer->id);
	log_line("gl::delete_buffer({u:x})", gl_buffer);
	glDeleteBuffers(1, &gl_buffer);

	*buffer = {};
}

katha::result_e katha::gl_t::create_pso(
	pso_t* out_pso,
	const vertex_layout_e vertex_layout,
	const char* vertex_shader_source,
	const char* fragment_shader_source,
	const blend_mode_e blend_mode,
	const depth_mode_e depth_mode
)
{
	if (!(out_pso && vertex_shader_source && fragment_shader_source))
	{
		return result_e::error_value_null;
	}

	GLuint vao = 0;
	if (vertex_layout_e::mesh == vertex_layout)
	{
		vao = create_vao_vertex_t(false);
		if (0 == vao)
		{
			return result_e::error_gl;
		}
	}
	else
	{
		log_line("error-gl: unknown vertex layout {u}", static_cast<uint32_t>(vertex_layout));
		return result_e::error_gl;
	}

	constexpr GLenum SHADER_TYPES[2] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
	const char* shader_sources[2] = {
		vertex_shader_source,
		fragment_shader_source
	};
	uint32_t shaders[2] = {};
	for (uint32_t i = 0; i < 2; i++)
	{
		shaders[i] = glCreateShader(SHADER_TYPES[i]);
		if (0 == shaders[i])
		{
			log_line("error-gl: failed to create shader");
			break;
		}

		// TODO: add version string based on version
		const char* sources[] = {
			get_shader_version_string(),
			shader_sources[i]
		};
		glShaderSource(shaders[i], 2, sources, nullptr);
		glCompileShader(shaders[i]);

		GLint success = GL_TRUE;
		glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &success);
		if (GL_TRUE == success)
		{
			continue;
		}
		
		// compile failed
		GLint log_length = 0;
		glGetShaderiv(shaders[i], GL_INFO_LOG_LENGTH, &log_length);
		if (log_length)
		{
			char* log = alloc<char>(log_length);
			glGetShaderInfoLog(shaders[i], log_length, nullptr, log);
			log_line("error-gl: shader compile error: {s}", log);
			release(log);
		}

		glDeleteShader(shaders[i]);
		shaders[i] = 0;
		break;
	}
	if (!(shaders[0] && shaders[1]))
	{
		if (shaders[0]) { glDeleteShader(shaders[0]); }
		if (shaders[1]) { glDeleteShader(shaders[1]); }
		
		glDeleteVertexArrays(1, &vao);

		return result_e::error_gl;
	}

	GLuint program = glCreateProgram();
	if (0 == program)
	{
		log_line("error-gl: failed to create shader program");
		glDeleteShader(shaders[0]);
		glDeleteShader(shaders[1]);
		
		glDeleteVertexArrays(1, &vao);
		
		return result_e::error_gl;
	}

	glAttachShader(program, shaders[0]);
	glAttachShader(program, shaders[1]);
	glLinkProgram(program);
	
	glDeleteShader(shaders[0]);
	glDeleteShader(shaders[1]);

	GLint success = GL_TRUE;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (GL_TRUE == success)
	{
		out_pso->id = program;
		out_pso->layout = vao;
		out_pso->blend_mode = blend_mode;
		out_pso->depth_mode = depth_mode;

		log_line("gl::pso {u:x}, vertex_array {u:x}", program, vao);
		return result_e::success;
	}

	// link failed
	GLint log_length = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
	if (log_length)
	{
		char* log = alloc<char>(log_length);
		glGetProgramInfoLog(program, log_length, nullptr, log);
		log_line("error-gl: program link error: {s}", log);
		release(log);
	}
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);

	return result_e::error_gl;
}

void katha::gl_t::delete_pso(pso_t* pso)
{
	if (nullptr == pso)
	{
		return;
	}

	GLuint program = static_cast<GLuint>(pso->id);
	log_line("gl: delete_pso({u:x})", program);
	if (program)
	{
		glDeleteProgram(program);
	}

	GLuint vao = static_cast<GLuint>(pso->layout);
	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
	}

	*pso = {};
}

katha::result_e katha::gl_t::create_texture(
	texture_t* out_texture,
	const uvec2 size,
	const format_e format,
	const void* data
)
{
	if (nullptr == out_texture)
	{
		return result_e::error_value_null;
	}

	format_t gl_format = format_to_gl_format(format);
	if (0 == gl_format.internal_format)
	{
		return result_e::error_value_unexpected;
	}

	uint32_t texture = 0;
	glGenTextures(1, &texture);
	if (0 == texture)
	{
		log_line("gl: failed to create texture of size {iv2}", size.array());
		return result_e::error_gl;
	}

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, gl_format.internal_format,
		static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y),
		0, gl_format.channel,
		gl_format.data_type,
		data
	);

	// TODO: make this parameterized
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	if (should_mipmap(format))
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST
		);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	out_texture->id = texture;
	out_texture->size = size;
	out_texture->format = format;

	log_line("gl::texture {u:x}, {u}x{u}, {s}",
		texture,
		size.x, size.y,
		format_to_cstring(format)
	);
	return result_e::success;
}

void katha::gl_t::delete_texture(texture_t* texture)
{
	if ((nullptr == texture) || (0 == texture->id))
	{
		return;
	}

	GLuint gl_texture = static_cast<GLuint>(texture->id);
	log_line("gl::delete_texture({u:x})", gl_texture);
	glDeleteTextures(1, &gl_texture);

	*texture = {};
}

katha::result_e katha::gl_t::create_framebuffer_from_texture(
	framebuffer_t* out_framebuffer,
	const texture_t& color_0,
	const texture_t& depth_stencil
)
{
	if (nullptr == out_framebuffer)
	{
		return result_e::error_value_null;
	}

	GLuint framebuffer = 0;
	glGenFramebuffers(1, &framebuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		static_cast<GLuint>(color_0.id), 0
	);

	if (depth_stencil.id)
	{
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
			GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
			static_cast<GLuint>(depth_stencil.id), 0
		);
	}

	GLenum gl_result = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (GL_FRAMEBUFFER_COMPLETE != gl_result)
	{
		glDeleteFramebuffers(1, &framebuffer);
		log_line("error-gl: framebuffer is incomplete {x32}", gl_result);
		return result_e::error_gl;
	}

	out_framebuffer->id = framebuffer;
	out_framebuffer->color_0 = color_0;
	out_framebuffer->depth_stencil = depth_stencil;

	// `size` from color_0
	// depth/stencil only framebuffers not supported yet
	log_line("gl::framebuffer {u:x} {u}x{u}",
		framebuffer,
		color_0.size.x, color_0.size.y
	);
	return result_e::success;
}

katha::result_e katha::gl_t::create_framebuffer(
	framebuffer_t* out_framebuffer,
	const uvec2 size,
	const format_e color_format,
	const format_e depth_stencil_format
)
{
	if (nullptr == out_framebuffer)
	{
		return result_e::error_value_null;
	}

	log_line("gl: create_framebuffer("
		"size={iv2}, color_texture_format={s}, depth_texture_format={s}"
		")",
		size.array(),
		format_to_cstring(color_format),
		format_to_cstring(depth_stencil_format)
	);

	framebuffer_t framebuffer = {};
	framebuffer.size = size;
	
	result_e result = create_texture(
		&(framebuffer.color_0), size, color_format
	);
	if (!check_result(result, "gl::create_framebuffer::create_texture::color_0"))
	{
		return result_e::error_gl;
	}

	if (format_e::none != depth_stencil_format)
	{
		result = create_texture(
			&(framebuffer.depth_stencil), size, depth_stencil_format
		);
		if (!check_result(result, "gl::create_framebuffer::create_texture::depth_stencil"))
		{
			delete_texture(&(framebuffer.color_0));
			return result_e::error_gl;
		}
	}

	result = create_framebuffer_from_texture(&framebuffer,
		framebuffer.color_0,
		framebuffer.depth_stencil
	);
	if (!check_result(result, "create_framebuffer_from_texture"))
	{
		return result;
	}

	*out_framebuffer = framebuffer;
	return result_e::success;
}

void katha::gl_t::delete_framebuffer(framebuffer_t* framebuffer)
{
	if (nullptr == framebuffer)
	{
		return;
	}

	GLuint fbo = static_cast<GLuint>(framebuffer->id);
	log_line("gl::delete_framebuffer({u:x})", fbo);

	if (fbo)
	{
		glDeleteFramebuffers(1, &fbo);
	}
	delete_texture(&(framebuffer->color_0));
	delete_texture(&(framebuffer->depth_stencil));

	*framebuffer = {};
}

katha::result_e katha::gl_t::create_mesh(
	mesh_t* out_mesh,
	const vertex_t* vertices,
	const index_t* indices,
	const uint32_t vertex_count,
	const uint32_t index_count
)
{
	if (nullptr == out_mesh)
	{
		return result_e::error_value_null;
	}

	mesh_t mesh = {};
	eflags_t<buffer_usage_e> buffer_usage = {};
	
	buffer_usage.set(buffer_usage_e::data);
	result_e result = create_buffer(
		&(mesh.vertex_buffer),
		buffer_usage,
		vertex_count * sizeof(vertex_t),
		vertices
	);
	if (!check_result(result, "gl::create mesh::vertex_buffer"))
	{
		delete_mesh(&mesh);
		return result;
	}

	if (index_count)
	{
		buffer_usage.unset(buffer_usage_e::data);
		buffer_usage.set(buffer_usage_e::index);
		result = create_buffer(
			&(mesh.index_buffer),
			buffer_usage,
			index_count * sizeof(index_t),
			indices
		);
		if (!check_result(result, "gl::create mesh::index_buffer"))
		{
			delete_mesh(&mesh);
			return result;
		}
	}

	*out_mesh = mesh;
	return result_e::success;
}

void katha::gl_t::delete_mesh(mesh_t* mesh)
{
	if (nullptr == mesh)
	{
		return;
	}

	delete_buffer(&(mesh->vertex_buffer));
	delete_buffer(&(mesh->index_buffer));
	*mesh = {};
}

katha::mat4 katha::gl_t::get_perspective_projection(
	const float vertical_fov_radians,
	const vec2 screen_size,
	const vec2 z_range
)
{
	const float half_fov = std::tan(vertical_fov_radians * 0.5f);
	const float aspect = aspect_xy(screen_size);
	const float f = 1.0f / half_fov;

	mat4 perspective = mat4::from_columns(
		vec4(f / aspect, 0, 0, 0),
		vec4(0, f, 0, 0),
		vec4(0, 0, -(z_range.x + z_range.y) / (z_range.y - z_range.x), -1.0f),
		vec4(0, 0, (-2.0f * z_range.x * z_range.y) / (z_range.y - z_range.x), 0)
	);
	return perspective;
}