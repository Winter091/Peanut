#pragma once

#include <Peanut/Render/Buffers/VertexBuffer.hpp>

namespace pn {

class OpenGLVertexBuffer final : public VertexBuffer
{
public:
    OpenGLVertexBuffer(const std::shared_ptr<BufferLayout>& layout, BufferMapAccess access, uint32_t size, const void* data);
    ~OpenGLVertexBuffer() override;

    void* Map() override;
    void Unmap() override;
    
    const std::shared_ptr<BufferLayout>& GetLayout() const override { return m_layout; }

public:
    uint32_t GetSize() const override { return m_size; }
    uint32_t GetVertexSize() const override { return m_layout->GetStride(); }
    uint32_t GetVertexCount() const override { return GetSize() / GetVertexSize(); }

    uint32_t GetOpenGLHandle() const { return m_handle; }

private:
    uint32_t m_handle = 0;
    uint32_t m_size = 0;
    BufferMapAccess m_mapAccess;
    std::shared_ptr<BufferLayout> m_layout;
};

}
