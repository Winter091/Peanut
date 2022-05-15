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

    uint32_t GetSize() const override;
    uint32_t GetVertexCount() const override;

private:
    uint32_t m_handle = 0u;
    std::shared_ptr<VertexBuffer> m_vertexBuffer;

private:
    void ProcessVertexBufferLayout();
    void ProcessVertexBufferLayoutElement(const BufferLayoutElement& elem, uint32_t index, uint32_t stride);
    uint32_t MapToGLType(BufferLayoutElementType type) const;
};

}

#endif
