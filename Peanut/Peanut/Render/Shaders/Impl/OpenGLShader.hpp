#ifndef __OPENGLSHADER_H__
#define __OPENGLSHADER_H__

#include <Peanut/Render/Shaders/Shader.hpp>
#include <unordered_map>

namespace pn {

class OpenGLShader final : public Shader
{
public:
    OpenGLShader(const ShaderPaths& paths, const std::string& name = "");
    OpenGLShader(const ShaderSources& sources, const std::string& name = "");
    ~OpenGLShader() override;

    const std::string& GetName() const override { return m_name; }

    void Bind() override;
    void Unbind() override;
    
    void SetFloat1(const std::string& name, float scalar) override;
    void SetFloat2(const std::string& name, const glm::vec2& vec) override;
    void SetFloat3(const std::string& name, const glm::vec3& vec) override;
    void SetFloat4(const std::string& name, const glm::vec4& vec) override;
    
    void SetInt1(const std::string& name, int scalar);
    void SetInt2(const std::string& name, const glm::ivec2& vec) override;
    void SetInt3(const std::string& name, const glm::ivec3& vec) override;
    void SetInt4(const std::string& name, const glm::ivec4& vec) override;
    
    void SetMat3(const std::string& name, const glm::mat3& mat) override;
    void SetMat4(const std::string& name, const glm::mat4& mat) override;

private:
    uint32_t m_handler;
    std::string m_name;
	std::unordered_map<std::string, int32_t> m_uniformLocations;

private:
    std::string ReadFile(const std::string& filePath);
    uint32_t CreateShaderProgram(const ShaderSources& sources);
    uint32_t CompileShader(const std::string source, uint32_t glShaderType);
    void CheckShaderCompileStatus(uint32_t handler);

    int32_t GetUniformLocation(const std::string& name);
};

}

#endif