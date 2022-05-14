#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>
#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

namespace pn {

struct ShaderPaths
{
    std::string vertexPath;
    std::string fragmentPath;

    ShaderPaths() = default;

    ShaderPaths(std::string vertexPath, std::string fragmentPath)
        : vertexPath(std::move(vertexPath))
        , fragmentPath(std::move(fragmentPath))
    {}
};

struct ShaderSources
{
    std::string vertexSource;
    std::string fragmentSource;

    ShaderSources() = default;

    ShaderSources(std::string vertexSource, std::string fragmentSource)
        : vertexSource(std::move(vertexSource))
        , fragmentSource(std::move(fragmentSource))
    {}
};

class Shader
{
public:
    Shader() = default;
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    virtual ~Shader() = default;

    virtual const std::string& GetName() const = 0;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void SetFloat1(const std::string& name, float scalar) = 0;
    virtual void SetFloat2(const std::string& name, const glm::vec2& vec) = 0;
    virtual void SetFloat3(const std::string& name, const glm::vec3& vec) = 0;
    virtual void SetFloat4(const std::string& name, const glm::vec4& vec) = 0;

    virtual void SetInt1(const std::string& name, int scalar) = 0;
    virtual void SetInt2(const std::string& name, const glm::ivec2& vec) = 0;
    virtual void SetInt3(const std::string& name, const glm::ivec3& vec) = 0;
    virtual void SetInt4(const std::string& name, const glm::ivec4& vec) = 0;

    virtual void SetMat3(const std::string& name, const glm::mat3& mat) = 0;
    virtual void SetMat4(const std::string& name, const glm::mat4& mat) = 0;

    static std::shared_ptr<Shader> Create(const ShaderPaths& paths, const std::string& name = "");
    static std::shared_ptr<Shader> Create(const ShaderSources& sources, const std::string& name = "");
};

}

#endif