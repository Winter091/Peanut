#pragma once

#include <Peanut/Render/Buffers/VertexBuffer.hpp>

namespace pn {

class OpenGLVertexBuffer final : public VertexBuffer
{
public:
    OpenGLVertexBuffer(uint32_t size, const void* data, BufferDataUsage usage);
    ~OpenGLVertexBuffer() override;

    void Bind() override;
    void BindToBindingIndex(uint32_t index) override;
    void Unbind() override;
    
    void ReplaceData(uint32_t size, const void* data, BufferDataUsage usage) override;
    void UpdateData(uint32_t size, const void* data, uint32_t offset = 0) override;

    void* Map(BufferMapAccess access = BufferMapAccess::ReadWrite) override;
    void Unmap() override;
    
    const std::shared_ptr<BufferLayout>& GetLayout() const override;
    void SetLayout(const std::shared_ptr<BufferLayout>& layout) override;

    uint32_t GetSize() const override { return m_size; }
    uint32_t GetVertexSize() const override;
    uint32_t GetVertexCount() const override;

    int GetHandle() const override { return m_handle; }

private:
    uint32_t m_handle;
    uint32_t m_size;
    std::shared_ptr<BufferLayout> m_layout;

private:
    static uint32_t ToGlMapAccess(BufferMapAccess access);
    static uint32_t ToGlUsage(BufferDataUsage usage);
};

}
