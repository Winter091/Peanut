#pragma once

#include <Peanut/Render/Buffers/VertexBuffer.hpp>
#include "OpenGLBuffer.hpp"

namespace pn {

class OpenGLVertexBuffer final : public VertexBuffer, public OpenGLBuffer
{
public:
    OpenGLVertexBuffer(const std::shared_ptr<BufferLayout>& layout, BufferMapAccess access,
                       VertexBufferDataUsage dataUsage, uint32_t size, const void* data)
        : OpenGLBuffer(access, size, data)
        , m_dataUsage(dataUsage)
        , m_layout(layout)
    {}

    ~OpenGLVertexBuffer() override = default;

    void* Map() override { return OpenGLBuffer::Map(); }
    void Unmap() override { OpenGLBuffer::Unmap(); }

    const std::shared_ptr<BufferLayout>& GetLayout() const override { return m_layout; }

    uint32_t GetSize() const override { return OpenGLBuffer::GetSize(); }
    uint32_t GetVertexSize() const override { return m_layout->GetVertexSize(); }
    uint32_t GetVertexCount() const override { return OpenGLBuffer::GetSize() / GetVertexSize(); }
    VertexBufferDataUsage GetDataUsage() const override { return m_dataUsage; }

private:
    VertexBufferDataUsage m_dataUsage;
    std::shared_ptr<BufferLayout> m_layout;
};

}
