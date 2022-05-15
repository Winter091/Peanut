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
    
    const std::shared_ptr<BufferLayout>& GetLayout() const override;
    void SetLayout(const std::shared_ptr<BufferLayout>& layout) override;

    uint32_t GetSize() const override { return m_size; }
    uint32_t GetVertexSize() const override;
    uint32_t GetVertexCount() const override;

private:
    uint32_t m_handle;
    uint32_t m_size;
    std::shared_ptr<BufferLayout> m_layout;
};

}

#endif