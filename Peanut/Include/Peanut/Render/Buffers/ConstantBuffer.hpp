#pragma once

#include <Peanut/Render/Buffers/BufferMapAccess.hpp>

#include <cstdint>
#include <memory>

namespace pn {

class ConstantBuffer
{
public:
    ConstantBuffer() = default;
    ConstantBuffer(const ConstantBuffer&) = delete;
    ConstantBuffer& operator=(const ConstantBuffer&) = delete;
    virtual ~ConstantBuffer() = default;

    virtual void* Map() = 0;
    virtual void Unmap() = 0;

    virtual uint32_t GetSize() const = 0;
    virtual void* GetNativeHandle() const = 0;

    static std::shared_ptr<ConstantBuffer> Create(BufferMapAccess access, uint32_t size, const void* data);
};

}
