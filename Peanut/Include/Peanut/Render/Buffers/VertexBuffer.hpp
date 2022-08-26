#pragma once

#include <Peanut/Render/Buffers/Buffer.hpp>
#include <Peanut/Render/Buffers/BufferLayout.hpp>
#include <Peanut/Render/Buffers/BufferMapAccess.hpp>

#include <cstdint>
#include <memory>

namespace pn {

enum class VertexBufferDataUsage
{
    PerVertex,
    PerInstance
};

class VertexBuffer : public Buffer
{
public:
    VertexBuffer() = default;
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    virtual ~VertexBuffer() = default;

    virtual const std::shared_ptr<BufferLayout>& GetLayout() const = 0;

    virtual uint32_t GetVertexSize() const = 0;
    virtual uint32_t GetVertexCount() const = 0;
    virtual VertexBufferDataUsage GetDataUsage() const = 0;

    static std::shared_ptr<VertexBuffer> Create(
        const std::shared_ptr<BufferLayout>& layout, BufferMapAccess access, 
        VertexBufferDataUsage dataUsage, uint32_t size, const void* data = nullptr);
};

}
