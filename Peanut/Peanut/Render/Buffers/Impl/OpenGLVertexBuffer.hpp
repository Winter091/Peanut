#ifndef __OPENGLVERTEXBUFFER_H__
#define __OPENGLVERTEXBUFFER_H__

#include <Peanut/Render/Buffers/VertexBuffer.hpp>

namespace pn {

class OpenGLVertexBuffer final : public VertexBuffer
{
public:
    OpenGLVertexBuffer(uint32_t size, const void* data = nullptr);
    ~OpenGLVertexBuffer() override;

    void Bind() override;
    void Unbind() override;
    
    void ReplaceData(const void* data, uint32_t size) override;
    void UpdateData(const void* data, uint32_t size, uint32_t offset = 0) override;
    
    const BufferLayout& GetLayout() const override;
    void SetLayout(const BufferLayout& layout) override { m_layout = layout; }

private:
    uint32_t m_handle;
    uint32_t m_size;
    BufferLayout m_layout;
};

}

#endif