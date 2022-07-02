#ifndef __OPENGLVERTEXARRAY_H__
#define __OPENGLVERTEXARRAY_H__

#include <Peanut/Render/Buffers/VertexArray.hpp>

#include <vector>

namespace pn {

class OpenGLVertexArray final : public VertexArray
{
public:
    OpenGLVertexArray();
    ~OpenGLVertexArray() override;

    void Bind() override;
    void Unbind() override;

    void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer, BufferDataUsage usage) override;
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

    uint32_t GetSize() const override;
    uint32_t GetVertexCount() const override;
    uint32_t GetIndexCount() const override;
    IndexBufferDataFormat GetIndexDataFormat() const override;

private:
    uint32_t m_handle = 0u;
    std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
    std::shared_ptr<IndexBuffer> m_indexBuffer;

private:
    void ProcessVertexBufferLayout(VertexBuffer* vertexBuffer, int bindingIndex);
    uint32_t MapToGLType(BufferLayoutElementType type) const;
    void AssertAllAttributeIndicesAreUnique() const;
};

}

#endif
