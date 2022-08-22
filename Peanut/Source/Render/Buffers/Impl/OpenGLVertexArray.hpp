#pragma once

#include <Peanut/Render/Buffers/VertexArray.hpp>

#include <vector>

namespace pn {

class OpenGLVertexArray final : public VertexArray
{
public:
    OpenGLVertexArray(const VertexArrayDescription& description);
    ~OpenGLVertexArray() override;

    const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_vertexBuffers; }
    std::shared_ptr<IndexBuffer> GetIndexBuffer() const override { return m_indexBuffer; }
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
    std::shared_ptr<ShaderInputLayout> GetShaderInputLayout() const override { return nullptr; }

    uint32_t GetVertexCount() const override;
    uint32_t GetIndexCount() const override;
    uint32_t GetInstanceCount() const override;
    IndexBufferDataFormat GetIndexDataFormat() const override;

    uint32_t GetOpenGLHandle() const { return m_vaoHandle; }

private:
    uint32_t m_vaoHandle = 0u;
    uint32_t m_instanceCount = 0u;
    std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
    std::shared_ptr<IndexBuffer> m_indexBuffer;

private:
    void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
    void ProcessVertexBufferLayout(VertexBuffer* vertexBuffer, uint32_t bindingIndex);
    void UpdateInstanceCount(const VertexBuffer& vertexBuffer);
};

}
