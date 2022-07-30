#include "OpenGLShader.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <vector>

namespace pn
{

OpenGLShader::OpenGLShader(const ShaderPaths& paths, std::string name)
    : m_name(std::move(name))
{
    PN_PROFILE_FUNCTION();

    ShaderSources sources;
    sources.VertexSource = ReadFile(paths.VertexPath);
    sources.FragmentSource = ReadFile(paths.FragmentPath);

    m_handler = CreateShaderProgram(sources);
}

std::string OpenGLShader::ReadFile(const std::string& filePath)
{
    PN_PROFILE_FUNCTION();

    std::ifstream file(filePath);
	PN_CORE_ENSURE(file, "Unable to open shader source file: {}", filePath);

	std::stringstream ss;
    ss << file.rdbuf();

    return ss.str();
}

OpenGLShader::OpenGLShader(const ShaderSources& sources, std::string name)
    : m_handler(CreateShaderProgram(sources))
    , m_name(std::move(name))
{

}

uint32_t OpenGLShader::CreateShaderProgram(const ShaderSources& sources)
{
    PN_PROFILE_FUNCTION();

    uint32_t vertexHandler = CompileShader(sources.VertexSource, GL_VERTEX_SHADER);
    uint32_t fragmentHandler = CompileShader(sources.FragmentSource, GL_FRAGMENT_SHADER);

    uint32_t programHandler = glCreateProgram();
	glAttachShader(programHandler, vertexHandler);
	glAttachShader(programHandler, fragmentHandler);

	glLinkProgram(programHandler);
    CheckLinkStatus(programHandler);

	glValidateProgram(programHandler);

	glDetachShader(programHandler, vertexHandler);
	glDetachShader(programHandler, fragmentHandler);
	glDeleteShader(vertexHandler);
	glDeleteShader(fragmentHandler);

    return programHandler;
}

uint32_t OpenGLShader::CompileShader(const std::string source, uint32_t glShaderType)
{
    PN_PROFILE_FUNCTION();

    uint32_t handler = glCreateShader(glShaderType);

    const char* sourcePtr = source.c_str();
	glShaderSource(handler, 1, &sourcePtr, nullptr);
	glCompileShader(handler);

    CheckShaderCompileStatus(handler);

	return handler;
}

void OpenGLShader::CheckShaderCompileStatus(uint32_t handler)
{
    PN_PROFILE_FUNCTION();

    int result = 0;
	glGetShaderiv(handler, GL_COMPILE_STATUS, &result);
	if (result == GL_TRUE) {
        return;
    }

    int bufSize = 0;
    glGetShaderiv(handler, GL_INFO_LOG_LENGTH, &bufSize);

    std::vector<char> buffer(bufSize);
    glGetShaderInfoLog(handler, bufSize, nullptr, &buffer[0]);

    PN_CORE_CRITICAL("Shader compilation error: {}", &buffer[0]);
    exit(EXIT_FAILURE);
}

void OpenGLShader::CheckLinkStatus(uint32_t handler)
{
    PN_PROFILE_FUNCTION();

    GLint logLen = 0;
    glGetProgramiv(handler, GL_INFO_LOG_LENGTH, &logLen);

    if (logLen == 0) {
        return;
    }

    std::vector<GLchar> log(logLen);
    glGetProgramInfoLog(handler, 1024, nullptr, &log[0]);
    PN_CORE_WARN("OpenGL Shader link info: {}", &log[0]);
}

OpenGLShader::~OpenGLShader()
{
    glDeleteProgram(m_handler);
}

void OpenGLShader::Bind()
{
    glUseProgram(m_handler);
}

void OpenGLShader::Unbind()
{
    glUseProgram(0u);
}

}