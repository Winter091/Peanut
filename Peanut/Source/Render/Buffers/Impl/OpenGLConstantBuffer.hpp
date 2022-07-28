#pragma once

#include <Peanut/Render/Buffers/ConstantBuffer.hpp>

namespace pn {

class OpenGLConstantBuffer final : public ConstantBuffer
{
public:
    OpenGLConstantBuffer(BufferMapAccess access, uint32_t size, const void* data);
    ~OpenGLConstantBuffer() override;

    void BindToBindingIndex(uint32_t index) override;
    void Unbind() override;

    void* Map() override;
    void Unmap() override;
    
    uint32_t GetSize() const override { return m_size; }

private:
    uint32_t m_handle;
    uint32_t m_size;
    BufferMapAccess m_mapAccess;
};

}
