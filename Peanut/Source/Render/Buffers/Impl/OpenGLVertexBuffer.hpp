#pragma once

#include <Peanut/Render/Buffers/VertexBuffer.hpp>

namespace pn {

class OpenGLVertexBuffer final : public VertexBuffer
{
public:
    OpenGLVertexBuffer(BufferMapAccess access, uint32_t size, const void* data);
    ~OpenGLVertexBuffer() override;

    void Bind() override;
    void BindToBindingIndex(uint32_t index) override;
    void Unbind() override;

    void* Map() override;
    void Unmap() override;
    
    const std::shared_ptr<BufferLayout>& GetLayout() const override;
    void SetLayout(const std::shared_ptr<BufferLayout>& layout) override;

    uint32_t GetSize() const override { return m_size; }
    uint32_t GetVertexSize() const override;
    uint32_t GetVertexCount() const override;

private:
    uint32_t m_handle;
    uint32_t m_size;
    BufferMapAccess m_mapAccess;
    std::shared_ptr<BufferLayout> m_layout;
};

}
