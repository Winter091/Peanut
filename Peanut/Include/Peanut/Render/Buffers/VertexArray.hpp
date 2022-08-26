#pragma once

#include <Peanut/Core/Assert.hpp>

#include <Peanut/Render/Buffers/VertexBuffer.hpp>
#include <Peanut/Render/Buffers/IndexBuffer.hpp>
#include <Peanut/Render/Buffers/ConstantBuffer.hpp>
#include <Peanut/Render/Buffers/BufferLayout.hpp>
#include <Peanut/Render/Shaders/Shader.hpp>
#include <Peanut/Render/Shaders/ShaderInputLayout.hpp>
#include <Peanut/Render/Textures/Texture.hpp>

namespace pn {

struct VertexArrayDescription
{
public:
    VertexArrayDescription& SetVertexBuffers(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers)
    {
        PN_CORE_ASSERT(!vertexBuffers.empty(), "Can't set an empty array of vertex buffers");
        m_vertexBuffers = vertexBuffers;
        return *this;
    }

    VertexArrayDescription& AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
    {
        PN_CORE_ASSERT(vertexBuffer, "Pointer to vertex buffer is nullptr");
        m_vertexBuffers.push_back(vertexBuffer);
        return *this;
    }

    VertexArrayDescription& SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        PN_CORE_ASSERT(indexBuffer, "Pointer to index buffer is nullptr");
        m_indexBuffer = indexBuffer;
        return *this;
    }

    VertexArrayDescription& SetShader(const std::shared_ptr<Shader>& shader)
    {
        PN_CORE_ASSERT(shader, "Pointer to shader is nullptr");
        m_shader = shader;
        return *this;
    }

    VertexArrayDescription& SetShaderInputLayout(const std::shared_ptr<ShaderInputLayout>& shaderInputLayout)
    {
        m_shaderInputLayout = shaderInputLayout;
        return *this;
    }

    const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_vertexBuffers; }
    const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_indexBuffer; }
    const std::shared_ptr<Shader>& GetShader() const { return m_shader; }
    const std::shared_ptr<ShaderInputLayout>& GetShaderInputLayout() const { return m_shaderInputLayout; }

private:
    std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
    std::shared_ptr<IndexBuffer> m_indexBuffer;
    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<ShaderInputLayout> m_shaderInputLayout;
};

class VertexArray
{
public:
    VertexArray() = default;
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    virtual ~VertexArray() = default;

    virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
    virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() const = 0;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;
    virtual std::shared_ptr<ShaderInputLayout> GetShaderInputLayout() const = 0;
    
    virtual uint32_t GetVertexCount() const = 0;
    virtual uint32_t GetIndexCount() const = 0;
    virtual uint32_t GetInstanceCount() const = 0;
    virtual IndexBufferDataFormat GetIndexDataFormat() const = 0;

    static std::shared_ptr<VertexArray> Create(const VertexArrayDescription& description);
};

}
