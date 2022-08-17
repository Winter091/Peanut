#pragma once

#include <Render/Buffers/Impl/OpenGLEnumConversions.hpp>
#include <Peanut/Render/Buffers/IndexBuffer.hpp>
#include "OpenGLBuffer.hpp"

namespace pn {

class OpenGLIndexBuffer final : public IndexBuffer, public OpenGLBuffer
{
public:
    OpenGLIndexBuffer(IndexBufferDataFormat format, BufferMapAccess access, uint32_t size, const void* data = nullptr)
        : OpenGLBuffer(access, size, data)
        , m_indexCount(size / GetIndexBufferDataFormatSize(format))
        , m_format(format)
    {}  

    ~OpenGLIndexBuffer() override = default;

    void* Map() override { return OpenGLBuffer::Map(); }
    void Unmap() override { OpenGLBuffer::Unmap(); }

    uint32_t GetSize() const override { return OpenGLBuffer::GetSize(); }
    uint32_t GetIndexCount() const override { return m_indexCount; }
    IndexBufferDataFormat GetDataFormat() const override { return m_format; }

private:
    uint32_t m_indexCount;
    IndexBufferDataFormat m_format;
};

}
