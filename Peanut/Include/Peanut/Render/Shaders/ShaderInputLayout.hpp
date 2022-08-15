#pragma once

#include <Peanut/Render/Buffers/VertexBuffer.hpp>
#include <Peanut/Render/Shaders/Shader.hpp>

#include <vector>

namespace pn {

class ShaderInputLayout
{
public:
    ShaderInputLayout() = default;
    ShaderInputLayout(const Shader&) = delete;
    ShaderInputLayout& operator=(const Shader&) = delete;
    virtual ~ShaderInputLayout() = default;

    static std::shared_ptr<ShaderInputLayout> Create(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers, const std::shared_ptr<Shader>& shader);

private:
    static bool CheckIfLayoutIsCorrect(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers);
};

}
