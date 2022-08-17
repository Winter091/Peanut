#pragma once

#include <Peanut/Render/Buffers/ConstantBuffer.hpp>
#include "OpenGLBuffer.hpp"

namespace pn {

class OpenGLConstantBuffer final : public ConstantBuffer, public OpenGLBuffer
{
public:
    OpenGLConstantBuffer(BufferMapAccess access, uint32_t size, const void* data)
        : OpenGLBuffer(access, size, data)
    {}

    ~OpenGLConstantBuffer() override = default;

    void* Map() override { return OpenGLBuffer::Map(); }
    void Unmap() override { OpenGLBuffer::Unmap(); }
    void SetData(const void* data, size_t offset = 0, size_t size = 0) { OpenGLBuffer::SetData(data, offset, size); }

    uint32_t GetSize() const override { return OpenGLBuffer::GetSize(); }
    uint32_t GetOpenGLHandle() const { return OpenGLBuffer::GetOpenGLHandle(); }
};

}
