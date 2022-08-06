#pragma once

#include <Peanut/Render/Shaders/Shader.hpp>

namespace pn {

class OpenGLShader final : public Shader
{
public:
    OpenGLShader(const ShaderPaths& paths, std::string name = "");
    OpenGLShader(const ShaderSources& sources, std::string name = "");
    ~OpenGLShader() override;

    const std::string& GetName() const override { return m_name; }

    void Bind() override;

private:
    uint32_t m_handler;
    std::string m_name;

private:
    std::string ReadFile(const std::string& filePath);
    uint32_t CreateShaderProgram(const ShaderSources& sources);
    uint32_t CompileShader(const std::string source, uint32_t glShaderType);
    void CheckShaderCompileStatus(uint32_t handler);
    void CheckLinkStatus(uint32_t handler);
};

}