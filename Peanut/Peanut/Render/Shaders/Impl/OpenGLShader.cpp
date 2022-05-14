#include "OpenGLShader.hpp"

#include <Peanut/Core/Assert.hpp>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <vector>

namespace pn
{

OpenGLShader::OpenGLShader(const ShaderPaths& paths, const std::string& name)
    : m_name(name)
{
    ShaderSources sources;
    sources.vertexSource = ReadFile(paths.vertexPath);
    sources.fragmentSource = ReadFile(paths.fragmentPath);

    m_handler = CreateShaderProgram(sources);
}

std::string OpenGLShader::ReadFile(const std::string& filePath)
{
    std::ifstream file(filePath);
	if (!file) {
		PN_CORE_CRITICAL("Unable to open shader source file: {}", filePath);
        exit(EXIT_FAILURE);
	}

	std::stringstream ss;
    ss << file.rdbuf();

    return ss.str();
}

OpenGLShader::OpenGLShader(const ShaderSources& sources, const std::string& name)
    : m_name(name)
{
    m_handler = CreateShaderProgram(sources);
}

uint32_t OpenGLShader::CreateShaderProgram(const ShaderSources& sources)
{
    uint32_t vertexHandler = CompileShader(sources.vertexSource, GL_VERTEX_SHADER);
    uint32_t fragmentHandler = CompileShader(sources.fragmentSource, GL_FRAGMENT_SHADER);

    uint32_t programHandler = glCreateProgram();
	glAttachShader(programHandler, vertexHandler);
	glAttachShader(programHandler, fragmentHandler);

	glLinkProgram(programHandler);
	glValidateProgram(programHandler);

	glDetachShader(programHandler, vertexHandler);
	glDetachShader(programHandler, fragmentHandler);
	glDeleteShader(vertexHandler);
	glDeleteShader(fragmentHandler);

    return programHandler;
}

uint32_t OpenGLShader::CompileShader(const std::string source, uint32_t glShaderType)
{
    uint32_t handler = glCreateShader(glShaderType);

    const char* sourcePtr = source.c_str();
	glShaderSource(handler, 1, &sourcePtr, nullptr);
	glCompileShader(handler);

    CheckShaderCompileStatus(handler);

	return handler;
}

void OpenGLShader::CheckShaderCompileStatus(uint32_t handler)
{
    int result = 0;
	glGetShaderiv(handler, GL_COMPILE_STATUS, &result);
	if (result == GL_TRUE) {
        return;
    }

    int bufSize;
    glGetShaderiv(handler, GL_INFO_LOG_LENGTH, &bufSize);

    std::vector<char> buffer(bufSize);
    glGetShaderInfoLog(handler, bufSize, nullptr, &buffer[0]);

    PN_CORE_CRITICAL("Shader compilation error: {}", &buffer[0]);
    exit(EXIT_FAILURE);
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

int32_t OpenGLShader::GetUniformLocation(const std::string& name)
{
    if (m_uniformLocations.find(name) == m_uniformLocations.end()) {
		m_uniformLocations[name] = glGetUniformLocation(m_handler, name.c_str());
	}

    int32_t location = m_uniformLocations[name];
    if (location == -1) {
        PN_CORE_WARN("Shader \"{}\": unable to find uniform \"{}\"", m_name, name);
    }

	return m_uniformLocations[name];
}

void OpenGLShader::SetFloat1(const std::string& name, float scalar)
{
    glProgramUniform1f(m_handler, GetUniformLocation(name), scalar);
}

void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& vec)
{
    glProgramUniform2f(m_handler, GetUniformLocation(name), vec.x, vec.y);
}

void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& vec)
{
    glProgramUniform3f(m_handler, GetUniformLocation(name), vec.x, vec.y, vec.z);
}

void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& vec)
{
    glProgramUniform4f(m_handler, GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w);   
}

void OpenGLShader::SetInt1(const std::string& name, int scalar)
{
    glProgramUniform1i(m_handler, GetUniformLocation(name), scalar);
}

void OpenGLShader::SetInt2(const std::string& name, const glm::ivec2& vec)
{
    glProgramUniform2i(m_handler, GetUniformLocation(name), vec.x, vec.y);
}

void OpenGLShader::SetInt3(const std::string& name, const glm::ivec3& vec)
{
    glProgramUniform3i(m_handler, GetUniformLocation(name), vec.x, vec.y, vec.z);
}

void OpenGLShader::SetInt4(const std::string& name, const glm::ivec4& vec)
{
    glProgramUniform4i(m_handler, GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
}

void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& mat)
{
    glProgramUniformMatrix3fv(m_handler, GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& mat)
{
    glProgramUniformMatrix4fv(m_handler, GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

}