#include "gl.hpp"
#include "../type/vertex.hpp"
#include "../utility.hpp"

#include <glad/glad.h>

constexpr const char* mesh_vertex_shader = ""
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec2 tex_coords;\n"
"out vec2 uv;\n"
"void main()\n"
"{\n"
	"gl_Position = vec4(position, 1);\n"
	"uv = tex_coords;\n"
"}"
;

constexpr const char* mesh_fragment_shader = ""
"in MEDIUMP vec2 uv;\n"
"layout (location = 0) out MEDIUMP vec4 color;\n"
"void main()\n"
"{\n"
	"color = vec4(uv.xy, 0, 1);"
"}"
;

bool katha::gl_t::create_resources()
{
	mesh_shader_program = create_shader_program(
		mesh_vertex_shader,
		mesh_fragment_shader
	);
	if (0 == mesh_shader_program)
	{
		return false;
	}

	mesh_vertex_array = create_vertex_array_mesh();
	if (0 == mesh_vertex_array)
	{
		return false;
	}

	return true;
}

void katha::gl_t::clear_resources()
{
	delete_shader_program(mesh_shader_program);
	delete_vertex_array(mesh_vertex_array);

	mesh_shader_program = 0;
	mesh_vertex_array = 0;
}

void katha::gl_t::delete_shader(const uint32_t shader)
{
	if (shader)
	{
		log_line("gl: delete_shader({u:x})", shader);
		glDeleteShader(shader);
	}
}

void katha::gl_t::delete_shader_program(const uint32_t program)
{
	if (program)
	{
		log_line("gl: delete_program({u:x})", program);
		glDeleteShader(program);
	}
}

void katha::gl_t::delete_vertex_array(const uint32_t vertex_array)
{
	if (vertex_array)
	{
		log_line("gl: delete_vertex_array({u:x})", vertex_array);
		glDeleteVertexArrays(1, &vertex_array);
	}
}


void katha::gl_t::delete_buffer(const uint32_t buffer)
{
	if (buffer)
	{
		log_line("gl: delete_buffer({u:x})", buffer);
		glDeleteBuffers(1, &buffer);
	}
}

const char* katha::gl_t::get_shader_version_cstring() const
{
	if (is_es_context)
	{
		return "#version 310 es\n"
			"#define LOWP lowp\n"
			"#define MEDIUMP mediump\n"
			"#define HIGHP highp\n";
	}

	return "#version 450 core\n"
		"#define LOWP\n"
		"#define MEDIUMP\n"
		"#define HIGHP\n";
}

uint32_t katha::gl_t::create_shader(const char* source, const uint32_t type)
{
	const char* sources[] = {
		get_shader_version_cstring(),
		source
	};

	GLuint shader = glCreateShader(type);
	if (0 == shader)
	{
		return 0;
	}
	if (check_error())
	{
		glDeleteShader(shader);
		return 0;
	}

	glShaderSource(shader, 2, sources, nullptr);
	glCompileShader(shader);

	GLint compile_ok = GL_TRUE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
	if (GL_TRUE == compile_ok)
	{
		return shader;
	}

	GLint log_length = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
	char* log = alloc<char>(static_cast<uint32_t>(log_length));
	if (log)
	{
		glGetShaderInfoLog(shader,
			static_cast<GLsizei>(log_length),
			nullptr,
			log
		);
		log_line("error-gl: failed to compile shader: {s}", log);
		release(log);
	}

	glDeleteShader(shader);
	return 0;
}

uint32_t katha::gl_t::create_shader_program(
	const char* vertex_shader_source,
	const char* fragment_shader_source
)
{
	const uint32_t vertex_shader = create_shader(vertex_shader_source, GL_VERTEX_SHADER);
	if (0 == vertex_shader)
	{
		log_line("error-gl: failed to create vertex shader");
		return 0;
	}

	const uint32_t fragment_shader = create_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
	if (0 == fragment_shader)
	{
		log_line("error-gl: failed to create fragment shader");
		glDeleteShader(vertex_shader);
		return 0;
	}

	GLuint program = glCreateProgram();
	if (0 == program)
	{
		return 0;
	}
	if (check_error())
	{
		glDeleteProgram(program);
		return 0;
	}

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	GLint link_status = GL_TRUE;
	glGetProgramiv(program, GL_LINK_STATUS, &link_status);
	if (GL_TRUE == link_status)
	{
		log_line("gl: program {u:x}", program);
		return program;
	}

	GLint log_length = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
	char* log = alloc<char>(static_cast<uint32_t>(log_length));
	if (log)
	{
		glGetProgramInfoLog(program,
			static_cast<GLsizei>(log_length),
			nullptr,
			log
		);
		log_line("error-gl: failed to link shader program: {s}", log);
		release(log);
	}

	glDeleteProgram(program);
	return 0;
}

uint32_t katha::gl_t::create_vertex_array_mesh()
{
	uint32_t vao = 0;
	glGenVertexArrays(1, &vao);
	if (0 == vao)
	{
		return 0;
	}
	if (check_error())
	{
		glDeleteVertexArrays(1, &vao);
		return 0;
	}

	glBindVertexArray(vao);

	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(vertex_t, position));
	glVertexAttribBinding(0, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribFormat(1, 2, GL_UNSIGNED_SHORT, GL_TRUE, offsetof(vertex_t, uv));
	glVertexAttribBinding(1, 0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	if (check_error())
	{
		glDeleteVertexArrays(1, &vao);
		return 0;
	}

	log_line("gl: vertex_array {u:x}", vao);
	return vao;
}

uint32_t katha::gl_t::create_buffer(
	efield_t<buffer_usage_e> usage,
	const uint32_t size,
	const void* data
)
{
	GLenum type = 0;
	if (usage.has_enum(buffer_usage_e::array))
	{
		type = GL_ARRAY_BUFFER;
	}
	else if (usage.has_enum(buffer_usage_e::element))
	{
		type = GL_ELEMENT_ARRAY_BUFFER;
	}
	else
	{
		log_line("error-gl: buffer usage must be array or element");
		return 0;
	}

	GLuint buffer = 0;
	glGenBuffers(1, &buffer);
	if (0 == buffer)
	{
		log_line("error-gl: failed to create buffer");
		return 0;
	}
	if (check_error())
	{
		return 0;
	}

	glBindBuffer(type, buffer);
	glBufferData(type, static_cast<GLsizeiptr>(size), data, GL_STATIC_DRAW);

	if (check_error())
	{
		glDeleteBuffers(1, &buffer);
		return 0;
	}

	log_line("gl: buffer {u:x}, {u}", buffer, size);
	return buffer;
}