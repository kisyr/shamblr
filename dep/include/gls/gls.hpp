#pragma once

#include <vector>
#include <cstring>

namespace gls {

// Common

#define GLS_HANDLE(Expr) \
	Expr; \
	gLastResult = glGetError(); \
	if (gLastResult != GL_NO_ERROR)

static GLenum gLastResult = GL_NO_ERROR;

static inline const GLchar* ErrorString(const GLuint code) {
	switch(code) {
		case GL_NO_ERROR:           return "No error";
		case GL_INVALID_ENUM:       return "Invalid enum";
		case GL_INVALID_VALUE:      return "Invalid value";
		case GL_INVALID_OPERATION:  return "Invalid operation";
		case GL_OUT_OF_MEMORY:      return "Out of memory";
		default:                    return "Unknown error";
	}
}

static inline GLenum HandleError(
	const GLuint result,
	const char* expr,
	const char* info = ""
) {
	if (result != GL_NO_ERROR) {
		std::stringstream ss;
		ss << expr << " : " << ErrorString(result) << " (" << result << ") - " << info;
		throw std::runtime_error(ss.str());
	}
	return result;
}

static inline GLenum Enable(const GLenum capability) {
	GLS_HANDLE(glEnable(capability)) {
		return HandleError(gLastResult, "glEnable");
	}
	return GL_NO_ERROR;
}

static inline GLenum Clear(const GLenum mask) {
	GLS_HANDLE(glClear(mask)) {
		return HandleError(gLastResult, "glClear");
	}
	return GL_NO_ERROR;
}

static inline GLenum DrawArrays(
	const GLenum topology,
	const GLsizei offset,
	const GLsizei elements
) {
	GLS_HANDLE(glDrawArrays(topology, offset, elements)) {
		return HandleError(gLastResult, "glDrawArrays");
	}
	return GL_NO_ERROR;
}

// Buffer

static inline GLuint BufferCreate(GLenum* error = NULL) {
	GLuint buffer;
	GLS_HANDLE(glGenBuffers(1, &buffer)) {
		HandleError(gLastResult, "glGenBuffers");
		if (error) *error = gLastResult;
		return 0;
	}
	return buffer;
}

static inline GLenum BufferDestroy(const GLuint buffer) {
	GLS_HANDLE(glDeleteBuffers(1, &buffer)) {
		return HandleError(gLastResult, "glDeleteBuffers");
	}
	return GL_NO_ERROR;
}

static inline GLenum BufferBind(
	const GLenum target,
	const GLuint buffer
) {
	GLS_HANDLE(glBindBuffer(target, buffer)) {
		return HandleError(gLastResult, "glBindBuffer");
	}
	return GL_NO_ERROR;
}

static inline GLenum BufferData(
	const GLenum target,
	const GLenum usage,
	const GLsizei size,
	const GLvoid* data
) {
	GLS_HANDLE(glBufferData(target, size, data, usage)) {
		return HandleError(gLastResult, "glBufferData");
	}
	return GL_NO_ERROR;
}

static inline GLenum BufferMap(
	const GLenum target,
	const GLenum access,
	GLvoid** memory
) {
	GLS_HANDLE(*memory = glMapBuffer(target, access)) {
		return HandleError(gLastResult, "glMapBuffer");
	}
	return GL_NO_ERROR;
}

static inline GLenum BufferUnmap(const GLenum target) {
	GLS_HANDLE(glUnmapBuffer(target)) {
		return HandleError(gLastResult, "glUnmapBuffer");
	}
	return GL_NO_ERROR;
}

static inline GLenum BufferWrite(
	const GLenum target,
	const GLsizei size,
	const GLvoid* data
) {
	GLvoid* memory;
	GLuint result;
	result = BufferMap(target, GL_WRITE_ONLY, &memory);
	if (result != GL_NO_ERROR) {
		return result;
	}
	memcpy(memory, data, size);
	result = BufferUnmap(target);
	return result;
}

static inline GLenum BufferRead(
	const GLenum target,
	const GLsizei size,
	GLvoid* data
) {
	GLvoid* memory;
	GLuint result;
	result = BufferMap(target, GL_READ_ONLY, &memory);
	if (result != GL_NO_ERROR) {
		return result;
	}
	memcpy(data, memory, size);
	result = BufferUnmap(target);
	return result;
}

static inline GLuint BufferCreate(
	const GLenum usage,
	const GLsizei size,
	const GLvoid* data,
	GLenum* error = NULL
) {
	auto buffer = BufferCreate(error);
	if (!buffer) return 0;
	BufferBind(GL_ARRAY_BUFFER, buffer);
	BufferData(GL_ARRAY_BUFFER, usage, size, data);
	BufferBind(GL_ARRAY_BUFFER, 0);
	return buffer;
}

// TEXTURE

struct TextureImageInfo {
	GLenum format;
	GLenum type;
	GLint width;
	GLint height;
	GLint depth;
	const GLvoid* data;
};

static inline GLuint TextureCreate(GLenum* error = NULL) {
	GLuint texture;
	GLS_HANDLE(glGenTextures(1, &texture)) {
		HandleError(gLastResult, "glGenTextures");
		if (error) *error = gLastResult;
		return 0;
	}
	return texture;
}

static inline GLenum TextureDestroy(const GLuint texture) {
	GLS_HANDLE(glDeleteTextures(1, &texture)) {
		return HandleError(gLastResult, "glDeleteTextures");
	}
	return GL_NO_ERROR;
}

static inline GLenum TextureActive(const GLenum unit) {
	GLS_HANDLE(glActiveTexture(unit)) {
		return HandleError(gLastResult, "glActiveTexture");
	}
	return GL_NO_ERROR;
}

static inline GLenum TextureBind(
	const GLenum target,
	const GLuint texture
) {
	GLS_HANDLE(glBindTexture(target, texture)) {
		return HandleError(gLastResult, "glBindTexture");
	}
	return GL_NO_ERROR;
}

static inline GLenum TextureParameter(
	const GLenum target,
	const GLenum name,
	const GLint value
) {
	GLS_HANDLE(glTexParameteri(target, name, value)) {
		return HandleError(gLastResult, "glTexParameteri");
	}
	return GL_NO_ERROR;
}

static inline GLenum TextureGenerateMipmap(const GLenum target) {
	GLS_HANDLE(glGenerateMipmap(target)) {
		return HandleError(gLastResult, "glGenerateMipmap");
	}
	return GL_NO_ERROR;
}

static inline GLenum TextureImage2D(
	const GLenum target,
	const GLint format,
	const TextureImageInfo& image
) {
	GLS_HANDLE(glTexImage2D(
		target,
		0,
		format,
		image.width,
		image.height,
		0,
		image.format,
		image.type,
		image.data
	)) {
		return HandleError(gLastResult, "glTexImage2D");
	}
	return GL_NO_ERROR;
}

static inline GLuint TextureCreate2D(
	const GLenum format,
	const TextureImageInfo& image,
	GLenum* error = NULL
) {
	auto texture = TextureCreate(error);
	if (!texture) return 0;
	TextureBind(GL_TEXTURE_2D, texture);
	TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	TextureGenerateMipmap(GL_TEXTURE_2D);
	TextureImage2D(GL_TEXTURE_2D, format, image);
	return texture;
}

// Vertex Array

static inline GLuint VertexArrayCreate(GLenum* error = NULL) {
	GLuint array;
	GLS_HANDLE(glGenVertexArrays(1, &array)) {
		HandleError(gLastResult, "glGenVertexArrays");
		if (error) *error = gLastResult;
		return 0;
	}
	return array;
}

static inline GLenum VertexArrayDestroy(const GLuint array) {
	GLS_HANDLE(glDeleteVertexArrays(1, &array)) {
		return HandleError(gLastResult, "glDeleteVertexArrays");
	}
	return GL_NO_ERROR;
}

static inline GLenum VertexArrayBind(const GLuint array) {
	GLS_HANDLE(glBindVertexArray(array)) {
		return HandleError(gLastResult, "glBindVertexArray");
	}
	return GL_NO_ERROR;
}

static inline GLenum VertexArrayAttributeEnable(const GLuint index) {
	GLS_HANDLE(glEnableVertexAttribArray(index)) {
		return HandleError(gLastResult, "glEnableVertexAttribArray");
	}
	return GL_NO_ERROR;
}

static inline GLenum VertexArrayAttributeDisable(const GLuint index) {
	GLS_HANDLE(glDisableVertexAttribArray(index)) {
		return HandleError(gLastResult, "glEnableVertexAttribArray");
	}
	return GL_NO_ERROR;
}

static inline GLenum VertexArrayAttributePointer(
	const GLuint index,
	const GLint size,
	const GLenum type,
	const GLboolean normalized,
	const GLsizei stride,
	const GLvoid* pointer
) {
	GLS_HANDLE(glVertexAttribPointer(index, size, type, normalized, stride, pointer)) {
		return HandleError(gLastResult, "glVertexAttribPointer");
	}
	return GL_NO_ERROR;
}

// Shader/Program

struct ProgramShaderInfo {
	GLenum type;
	const GLchar* source;
};

static inline GLuint ShaderCreate(const GLenum type, GLenum* error = NULL) {
	GLuint shader;
	GLS_HANDLE(shader = glCreateShader(type)) {
		HandleError(gLastResult, "glCreateShader");
		if (error) *error = gLastResult;
		return 0;
	}
	return shader;
}

static inline GLenum ShaderDestroy(const GLuint shader) {
	GLS_HANDLE(glDeleteShader(shader)) {
		return HandleError(gLastResult, "glDeleteShader");
	}
	return GL_NO_ERROR;
}

static inline GLenum ShaderSource(
	const GLuint shader,
	const GLchar* source
) {
	const GLsizei length = strlen(source);
	GLS_HANDLE(glShaderSource(shader, 1, (const GLchar**)&source, &length)) {
		return HandleError(gLastResult, "glShaderSource");
	}
	return GL_NO_ERROR;
}

static inline GLenum ShaderCompile(const GLuint shader) {
	GLS_HANDLE(glCompileShader(shader)) {
		return HandleError(gLastResult, "glCompileShader");
	}
	return GL_NO_ERROR;
}

template <GLenum Name>
static inline GLint ProgramInfo(const GLuint program) {
	GLint result;
	GLS_HANDLE(glGetProgramiv(program, Name, &result)) {
		return HandleError(gLastResult, "glGetProgramiv");
	}
	return result;
}

static inline std::string ProgramInfoLog(const GLuint program) {
	std::string log(ProgramInfo<GL_INFO_LOG_LENGTH>(program), '\0');
	GLS_HANDLE(glGetProgramInfoLog(program, log.size(), NULL, &log[0])) {
		return "";
	}
	return log;
}

static inline GLuint ProgramCreate(GLenum* error = NULL) {
	GLuint program;
	GLS_HANDLE(program = glCreateProgram()) {
		HandleError(gLastResult, "glCreateProgram");
		if (error) *error = gLastResult;
		return 0;
	}
	return program;
}

static inline GLenum ProgramDestroy(const GLuint program) {
	GLS_HANDLE(glDeleteProgram(program)) {
		return HandleError(gLastResult, "glDeleteProgram");
	}
	return GL_NO_ERROR;
}

static inline GLenum ProgramAttach(
	const GLuint program,
	const GLuint shader
) {
	GLS_HANDLE(glAttachShader(program, shader)) {
		return HandleError(gLastResult, "glAttachShader");
	}
	return GL_NO_ERROR;
}

static inline GLenum ProgramLink(const GLuint program) {
	GLS_HANDLE(glLinkProgram(program)) {
		return HandleError(gLastResult, "glLinkProgram");
	}
	if (ProgramInfo<GL_LINK_STATUS>(program) == GL_FALSE) {
		const auto log = ProgramInfoLog(program);
		return HandleError(GL_INVALID_OPERATION, "ProgramBuild", log.c_str());
	}
	return GL_NO_ERROR;
}

static inline GLenum ProgramBind(const GLuint program) {
	GLS_HANDLE(glUseProgram(program)) {
		return HandleError(gLastResult, "glUseProgram");
	}
	return GL_NO_ERROR;
}

static inline GLuint ProgramCreate(const std::vector<ProgramShaderInfo>& shaders, GLenum* error = NULL) {
	auto program = ProgramCreate(error);
	if (!program) return 0;
	for (auto& s : shaders) {
		auto shader = ShaderCreate(s.type);
		if (!shader) return 0;
		ShaderSource(shader, s.source);
		ShaderCompile(shader);
		ProgramAttach(program, shader);
		ShaderDestroy(shader);
	}
	ProgramLink(program);
	return program;
}

static inline GLint ProgramUniformLocation(
	const GLuint program,
	const GLchar* name
) {
	GLint location;
	GLS_HANDLE(location = glGetUniformLocation(program, name)) {
		return HandleError(gLastResult, "glGetUniformLocation");
	}
	return location;
}

template <class T>
struct ProgramUniformFn {
	static GLuint set(const GLint, const T&) {
		return GL_INVALID_OPERATION;
	}
};

template <>
struct ProgramUniformFn<GLint> {
	static GLuint set(const GLint location, const GLint& value, const GLsizei count) {
		GLS_HANDLE(glUniform1iv(location, count, &value)) {
			return HandleError(gLastResult, "glUniform1iv");
		}
		return GL_NO_ERROR;
	}
};

template <>
struct ProgramUniformFn<GLuint> {
	static GLuint set(const GLint location, const GLuint& value, const GLsizei count) {
		GLS_HANDLE(glUniform1uiv(location, count, &value)) {
			return HandleError(gLastResult, "glUniform1uiv");
		}
		return GL_NO_ERROR;
	}
};

template <>
struct ProgramUniformFn<GLfloat> {
	static GLuint set(const GLint location, const GLfloat& value, const GLsizei count) {
		GLS_HANDLE(glUniform1fv(location, count, &value)) {
			return HandleError(gLastResult, "glUniform1fv");
		}
		return GL_NO_ERROR;
	}
};

#ifdef GLS_ENABLE_GLM_INTEROP
template <>
struct ProgramUniformFn<glm::vec2> {
	static GLuint set(const GLint location, const glm::vec2& value, const GLsizei count) {
		GLS_HANDLE(glUniform2fv(location, count, (const GLfloat*)&value)) {
			return HandleError(gLastResult, "glUniform2fv");
		}
		return GL_NO_ERROR;
	}
};

template <>
struct ProgramUniformFn<glm::vec3> {
	static GLuint set(const GLint location, const glm::vec3& value, const GLsizei count) {
		GLS_HANDLE(glUniform3fv(location, count, (const GLfloat*)&value)) {
			return HandleError(gLastResult, "glUniform3fv");
		}
		return GL_NO_ERROR;
	}
};

template <>
struct ProgramUniformFn<glm::vec4> {
	static GLuint set(const GLint location, const glm::vec4& value, const GLsizei count) {
		GLS_HANDLE(glUniform4fv(location, count, (const GLfloat*)&value)) {
			return HandleError(gLastResult, "glUniform4fv");
		}
		return GL_NO_ERROR;
	}
};
#endif

template <class T>
static inline GLenum ProgramUniform(
	const GLint location,
	const T& value,
	const GLsizei count = 1
) {
	return ProgramUniformFn<T>::set(location, value, count);
}

template <class T>
static inline GLenum ProgramUniform(
	const GLchar* name,
	const T& value,
	const GLsizei count = 1
) {
	GLint program;
	GLS_HANDLE(glGetIntegerv(GL_CURRENT_PROGRAM, &program)) {
		return HandleError(gLastResult, "glGetIntegerv");
	}
	const GLint location = ProgramUniformLocation(program, name);
	return ProgramUniformFn<T>::set(location, value, count);
}

} // namespace gls

