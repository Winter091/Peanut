#pragma once

#include <Peanut/Render/Buffers/ConstantBuffer.hpp>

namespace pn {

class OpenGLConstantBuffer final : public ConstantBuffer
{
public:
    OpenGLConstantBuffer(BufferMapAccess access, uint32_t size, const void* data);
    ~OpenGLConstantBuffer() override;

    void* Map() override;
    void Unmap() override;
    
    uint32_t GetSize() const override { return m_size; }
    uint32_t GetOpenGLHandle() const { return m_handle; }

private:
    uint32_t m_handle = 0;
    uint32_t m_size;
    BufferMapAccess m_mapAccess;
};

}
