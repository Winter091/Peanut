#pragma once

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
    std::string VertexPath;
    std::string FragmentPath;

    ShaderPaths& SetVertexPath(const std::string& path) { VertexPath = path; return *this; }
    ShaderPaths& SetFragmentPath(const std::string& path) { FragmentPath = path; return *this; }
};

struct ShaderSources
{
    std::string VertexSource;
    std::string FragmentSource;

    ShaderSources& SetVertexSource(const std::string& source) { VertexSource = source; return *this; }
    ShaderSources& SetFragmentSource(const std::string& source) { FragmentSource = source; return *this; }
};

class Shader
{
public:
    Shader() = default;
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    virtual ~Shader() = default;

    virtual const std::string& GetName() const = 0;
    virtual uint32_t GetNativeHandle() const = 0;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    static std::shared_ptr<Shader> Create(const ShaderPaths& paths, const std::string& name = "");
    static std::shared_ptr<Shader> Create(const ShaderSources& sources, const std::string& name = "");
};

}
