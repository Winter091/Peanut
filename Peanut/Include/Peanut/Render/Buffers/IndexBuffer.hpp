#pragma once

#include <Peanut/Render/Buffers/Buffer.hpp>
#include <Peanut/Render/Buffers/BufferMapAccess.hpp>

#include <cstdint>
#include <memory>

namespace pn {

enum class IndexBufferDataFormat : uint32_t
{
    Uint16 = 0,
    Uint32,
};

class IndexBuffer : public Buffer
{
public:
    IndexBuffer() = default;
    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;
    virtual ~IndexBuffer() = default;

    virtual uint32_t GetIndexCount() const = 0;
    virtual IndexBufferDataFormat GetDataFormat() const = 0;

    static std::shared_ptr<IndexBuffer> Create(IndexBufferDataFormat format, BufferMapAccess access, uint32_t size, const void* data = nullptr);
};

}
