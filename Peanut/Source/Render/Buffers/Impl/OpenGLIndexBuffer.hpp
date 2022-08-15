#pragma once

#include <Peanut/Render/Buffers/IndexBuffer.hpp>

namespace pn {

class OpenGLIndexBuffer final : public IndexBuffer
{
public:
    OpenGLIndexBuffer(IndexBufferDataFormat format, BufferMapAccess access, uint32_t size, const void* data = nullptr);
    ~OpenGLIndexBuffer() override;

    void* Map() override;
    void Unmap() override;

    uint32_t GetIndexCount() const override { return m_indexCount; }
    IndexBufferDataFormat GetDataFormat() const override { return m_format; }

    uint32_t GetOpenGLHandle() const { return m_handle; }

private:
    uint32_t m_handle = 0;
    uint32_t m_size = 0;
    uint32_t m_indexCount = 0;
    BufferMapAccess m_mapAccess;
    IndexBufferDataFormat m_format;
};

}
