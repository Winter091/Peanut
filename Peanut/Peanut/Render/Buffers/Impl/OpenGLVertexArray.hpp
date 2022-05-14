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

    void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;

private:
    uint32_t m_handle;
    std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;

private:
    void ProcessVertexBufferLayout(const std::shared_ptr<VertexBuffer>& vertexBuffer);
    void ProcessVertexBufferLayoutElement(const BufferLayoutElement& elem, uint32_t index, uint32_t stride);
    uint32_t MapToGLType(BufferLayoutElementType type) const;
};

}

#endif
