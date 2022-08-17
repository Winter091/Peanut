#pragma once

#include <Peanut/Render/Buffers/BufferMapAccess.hpp>

namespace pn {

class OpenGLBuffer
{
public:
    OpenGLBuffer(BufferMapAccess access, uint32_t size, const void* data);
    virtual ~OpenGLBuffer();

    void* Map();
    void Unmap();
    void SetData(const void* data, size_t offset, size_t size);

    uint32_t GetSize() const { return m_size; }
    uint32_t GetOpenGLHandle() const { return m_handle; }

protected:
    uint32_t m_handle = 0;
    uint32_t m_size;
    BufferMapAccess m_mapAccess;
};

}
