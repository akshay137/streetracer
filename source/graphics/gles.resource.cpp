#include "gles.hpp"
#include "../type/vertex.hpp"
#include "../utility.hpp"

#include <glad/glad.h>

constexpr const char* MESH_VERTEX_SHADER = ""
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 tex_coords;\n"
"layout (location = 2) in vec3 normal;\n"
"out vec2 uv;\n"
"void main()\n"
"{\n"
	"vec3 pos = position * vec3(9.0 / 16.0, 1, 1) * 0.5;\n"
	"gl_Position = vec4(pos, 1.0);\n"
	"uv = vec2(tex_coords.x, 1.0 - tex_coords.y);\n"
"}"
;

constexpr const char* MESH_FRAGMENT_SHADER = ""
"in MEDIUMP vec2 uv;\n"
"layout (location = 0) out MEDIUMP vec4 color;\n"
"layout (binding = 0) uniform sampler2D diffuse;\n"
"void main()\n"
"{\n"
	"color = texture(diffuse, uv);\n"
"}"
;

bool katha::GLES::createResources()
{
	mesh_shader_program = createShaderProgram(MESH_VERTEX_SHADER, MESH_FRAGMENT_SHADER);
	if (0 == mesh_shader_program) {
		return false;
	}

	mesh_vertex_array = createVertexArrayMesh();
	if (0 == mesh_vertex_array) {
		return false;
	}

	return true;
}

void katha::GLES::clearResources()
{
	deleteShaderProgram(mesh_shader_program);
	deleteVertexArray(mesh_vertex_array);

	mesh_shader_program = 0;
	mesh_vertex_array = 0;
}

void katha::GLES::deleteShader(const uint32_t shader)
{
	if (shader)
	{
		LogLine("gles: deleteShader({u:x})", shader);
		glDeleteShader(shader);
	}
}

void katha::GLES::deleteShaderProgram(const uint32_t program)
{
	if (program)
	{
		LogLine("gles: deleteShaderProgram({u:x})", program);
		glDeleteProgram(program);
	}
}

void katha::GLES::deleteVertexArray(const uint32_t vertex_array)
{
	if (vertex_array)
	{
		LogLine("gles: deleteVertexArray({u:x})", vertex_array);
		glDeleteVertexArrays(1, &vertex_array);
	}
}


void katha::GLES::deleteBuffer(const Buffer& buffer)
{
	if (buffer.handle)
	{
		GLuint gl_buffer = static_cast<GLuint>(buffer.handle);
		LogLine("gles: deleteBuffer({u:x})", gl_buffer);
		glDeleteBuffers(1, &gl_buffer);
	
	}
}

void katha::GLES::deleteTexture(const Texture& texture)
{
	if (texture.handle)
	{
		GLuint gl_texture = static_cast<GLuint>(texture.handle);
		LogLine("gles: deleteTexture({u:x})", gl_texture);
		glDeleteTextures(1, &gl_texture);
	}
}

const char* katha::GLES::getShaderVersionCString() const
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

uint32_t katha::GLES::createShader(const char* source, const uint32_t type)
{
	const char* sources[] = {
		getShaderVersionCString(),
		source
	};

	GLuint shader = glCreateShader(type);
	if (0 == shader)
	{
		return 0;
	}
	if (checkError())
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
	char* log = Alloc<char>(static_cast<uint32_t>(log_length));
	if (log)
	{
		glGetShaderInfoLog(shader,
			static_cast<GLsizei>(log_length),
			nullptr,
			log
		);
		LogLine("error-gles: failed to compile shader: {s}", log);
		Release(log);
	}

	glDeleteShader(shader);
	return 0;
}

uint32_t katha::GLES::createShaderProgram(
	const char* vertex_shader_source,
	const char* fragment_shader_source
)
{
	const uint32_t vertex_shader = createShader(vertex_shader_source, GL_VERTEX_SHADER);
	if (0 == vertex_shader)
	{
		LogLine("error-gles: failed to create vertex shader");
		return 0;
	}

	const uint32_t fragment_shader = createShader(fragment_shader_source, GL_FRAGMENT_SHADER);
	if (0 == fragment_shader)
	{
		LogLine("error-gles: failed to create fragment shader");
		glDeleteShader(vertex_shader);
		return 0;
	}

	GLuint program = glCreateProgram();
	if (0 == program)
	{
		return 0;
	}
	if (checkError())
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
		LogLine("gles: program {u:x}", program);
		return program;
	}

	GLint log_length = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
	char* log = Alloc<char>(static_cast<uint32_t>(log_length));
	if (log)
	{
		glGetProgramInfoLog(program,
			static_cast<GLsizei>(log_length),
			nullptr,
			log
		);
		LogLine("error-gles: failed to link shader program: {s}", log);
		Release(log);
	}

	glDeleteProgram(program);
	return 0;
}

uint32_t katha::GLES::createVertexArrayMesh()
{
	uint32_t vao = 0;
	glGenVertexArrays(1, &vao);
	if (0 == vao)
	{
		return 0;
	}
	if (checkError())
	{
		glDeleteVertexArrays(1, &vao);
		return 0;
	}

	glBindVertexArray(vao);

	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
	glVertexAttribBinding(0, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribFormat(1, 3, GL_UNSIGNED_SHORT, GL_TRUE, offsetof(Vertex, uv));
	glVertexAttribBinding(1, 0);
	glEnableVertexAttribArray(1);

	glVertexAttribFormat(2, 3, GL_SHORT, GL_TRUE, offsetof(Vertex, normal));
	glVertexAttribBinding(2, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	if (checkError())
	{
		glDeleteVertexArrays(1, &vao);
		return 0;
	}

	LogLine("gles: vertex_array {u:x}", vao);
	return vao;
}

katha::Result katha::GLES::createBuffer(
	Buffer* out_buffer,
	EnumField<BufferUsage> usage,
	const uint32_t size,
	const void* data
)
{
	if (nullptr == out_buffer)
	{
		LogLine("error: gles::create_buffer called with null `out_buffer`");
		return Result::ERROR_VALUE_NULL;
	}

	GLenum target = GL_ARRAY_BUFFER;
	if (usage.hasEnum(BufferUsage::ELEMENT))
	{
		target = GL_ELEMENT_ARRAY_BUFFER;
	}

	GLuint gl_buffer = 0;
	glGenBuffers(1, &gl_buffer);
	if (0 == gl_buffer)
	{
		LogLine("error-gles: failed to create buffer");
		return {};
	}
	if (checkError())
	{
		Result::ERROR_GLES;
	}

	GLenum type = GL_STATIC_DRAW;
	if (usage.hasEnum(BufferUsage::STREAM))
	{
		type = GL_STREAM_DRAW;
	}

	glBindBuffer(target, gl_buffer);
	glBufferData(target, static_cast<GLsizeiptr>(size), data, type);

	if (checkError())
	{
		glDeleteBuffers(1, &gl_buffer);
		Result::ERROR_GLES;
	}

	out_buffer->handle = gl_buffer;
	out_buffer->size = size;
	LogLine("gles: buffer {u:x}, {u}", gl_buffer, size);
	return Result::SUCCESS;
}

katha::Result katha::GLES::createTexture(
	Texture* out_texture,
	const Format format,
	const uvec2 size,
	const void* pixels
)
{
	if (nullptr == out_texture)
	{
		LogLine("error: gles::create_texture called with null `out_texture`");
		return Result::ERROR_VALUE_NULL;
	}

	const GLESFormat gl_format = FormatToGLESFormat(format);
	if (0 == gl_format.internal)
	{
		return Result::ERROR_VALUE_UNEXPECTED;
	}

	GLuint gl_texture = 0;
	glGenTextures(1, &gl_texture);
	if (0 == gl_texture)
	{
		LogLine("error-gles: failed to create texture");
		Result::ERROR_GLES;
	}
	if (checkError())
	{
		Result::ERROR_GLES;
	}

	glBindTexture(GL_TEXTURE_2D, gl_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, gl_format.internal,
		static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y),
		0,
		gl_format.channel, gl_format.data_type,
		pixels
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if (ShouldMipmap(format))
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	// FIXME
	if (Format::DEPTH24_STENCIL8 != format)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, gl_format.swizzle.x);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, gl_format.swizzle.y);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, gl_format.swizzle.z);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, gl_format.swizzle.w);
	}

	if (checkError())
	{
		glDeleteTextures(1, &gl_texture);
		return Result::ERROR_GLES;
	}

	out_texture->handle = gl_texture;
	out_texture->size = size;
	out_texture->format = format;
	LogLine("gles: texture {u:x}, {u}x{u}, {s}",
		gl_texture,
		size.x, size.y,
		FormatToCString(format)
	);
	return Result::SUCCESS;
}

void katha::GLES::bindVertexBuffer(
	const Buffer& buffer,
	uint32_t binding,
	uint32_t stride,
	uint32_t offset
)
{
	glBindVertexBuffer(
		binding,
		static_cast<GLuint>(buffer.handle),
		static_cast<GLintptr>(offset),
		static_cast<GLsizei>(stride)
	);
}

void katha::GLES::bindTexture(const uint32_t slot, const Texture& texture)
{
	if (texture.handle)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(texture.handle));
	}
}