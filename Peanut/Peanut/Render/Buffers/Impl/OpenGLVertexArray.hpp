#ifndef __OPENGLVERTEXARRAY_H__
#define __OPENGLVERTEXARRAY_H__

#include <Peanut/Render/Buffers/VertexArray.hpp>

namespace pn {

class OpenGLVertexArray final : public VertexArray
{
public:
    OpenGLVertexArray();
    ~OpenGLVertexArray() override;

    void Bind() override;
    void Unbind() override;

    void SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

    bool GetIsVertexBufferInitialized() const override;
    bool GetIsIndexBufferInitialized() const override;

    uint32_t GetSize() const override;
    uint32_t GetVertexCount() const override;
    uint32_t GetIndexCount() const override;
    IndexBufferDataFormat GetIndexDataFormat() const override;

private:
    uint32_t m_handle = 0u;
    std::shared_ptr<VertexBuffer> m_vertexBuffer;
    std::shared_ptr<IndexBuffer> m_indexBuffer;

private:
    void ProcessVertexBufferLayout();
    void ProcessVertexBufferLayoutElement(const BufferLayoutElement& elem, uint32_t index, uint32_t stride);
    uint32_t MapToGLType(BufferLayoutElementType type) const;
};

}

#endif
