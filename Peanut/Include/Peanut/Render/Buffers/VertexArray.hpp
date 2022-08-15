#pragma once

#include <Peanut/Render/Buffers/VertexBuffer.hpp>
#include <Peanut/Render/Buffers/IndexBuffer.hpp>
#include <Peanut/Render/Buffers/ConstantBuffer.hpp>
#include <Peanut/Render/Buffers/BufferLayout.hpp>
#include <Peanut/Render/Shaders/Shader.hpp>
#include <Peanut/Render/Shaders/ShaderInputLayout.hpp>

namespace pn {

struct VertexArrayDescription
{
    std::vector<std::shared_ptr<VertexBuffer>> VertexBuffers;
    std::shared_ptr<Shader> _Shader;
    std::shared_ptr<ShaderInputLayout> _ShaderInputLayout;

    std::shared_ptr<IndexBuffer> _IndexBuffer;
    std::vector<std::shared_ptr<ConstantBuffer>> ConstantBuffers;
};

class VertexArray
{
public:
    VertexArray() = default;
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    virtual ~VertexArray() = default;

    virtual void Bind() = 0;

    virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
    virtual std::shared_ptr<Shader> GetShader() const = 0;
    virtual std::shared_ptr<ShaderInputLayout> GetShaderInputLayout() const = 0;

    virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() const = 0;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

    virtual const std::vector<std::shared_ptr<ConstantBuffer>>& GetConstantBuffers() const = 0;
    virtual void SetConstantBuffers(const std::vector<std::shared_ptr<ConstantBuffer>>& constantBuffers) = 0;
    
    virtual uint32_t GetVertexCount() const = 0;
    virtual uint32_t GetIndexCount() const = 0;
    virtual uint32_t GetInstanceCount() const = 0;
    virtual IndexBufferDataFormat GetIndexDataFormat() const = 0;

    static std::shared_ptr<VertexArray> Create(const VertexArrayDescription& description);
};

}
