#pragma once

#include <Peanut/Render/Buffers/BufferMapAccess.hpp>
#include <cstdint>
#include <memory>

namespace pn {

enum class IndexBufferDataFormat : uint32_t
{
    Uint8 = 0,
    Uint16,
    Uint32,
};

class IndexBuffer
{
public:
    IndexBuffer() = default;
    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;
    virtual ~IndexBuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void* Map() = 0;
    virtual void Unmap() = 0;

    virtual uint32_t GetIndexCount() const = 0;
    virtual IndexBufferDataFormat GetDataFormat() const = 0;

    static std::shared_ptr<IndexBuffer> Create(IndexBufferDataFormat format, BufferMapAccess access, uint32_t size, const void* data = nullptr);
};

}
