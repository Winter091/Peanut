#pragma once

#include <Peanut/Render/Buffers/VertexArray.hpp>

#include <vector>

namespace pn {

class OpenGLVertexArray final : public VertexArray
{
public:
    OpenGLVertexArray(const VertexArrayDescription& description);
    ~OpenGLVertexArray() override;

    void Bind() override;

private:
    void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

public:

    uint32_t GetVertexCount() const override;
    uint32_t GetIndexCount() const override;
    IndexBufferDataFormat GetIndexDataFormat() const override;
    uint32_t GetInstanceCount() const override;

private:
    uint32_t m_vaoHandle = 0u;
    uint32_t m_instanceCount = 0u;
    std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
    std::shared_ptr<IndexBuffer> m_indexBuffer;
    std::shared_ptr<Shader> m_shader;

private:
    void ProcessVertexBufferLayout(VertexBuffer* vertexBuffer, uint32_t bindingIndex);
    uint32_t MapToGLType(BufferLayoutElementType type) const;
    void AssertAllAttributeIndicesAreUnique() const;
    void UpdateInstanceCount(const VertexBuffer& vertexBuffer);
};

}
