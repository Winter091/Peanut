#pragma once

#include <Peanut/Render/Buffers/Buffer.hpp>
#include <Peanut/Render/Buffers/BufferMapAccess.hpp>

#include <cstdint>
#include <memory>

namespace pn {

class ConstantBuffer : public Buffer
{
public:
    ConstantBuffer() = default;
    ConstantBuffer(const ConstantBuffer&) = delete;
    ConstantBuffer& operator=(const ConstantBuffer&) = delete;
    virtual ~ConstantBuffer() = default;

    static std::shared_ptr<ConstantBuffer> Create(BufferMapAccess access, uint32_t size, const void* data = nullptr);
};

}
