#pragma once

#include <Peanut/Render/Buffers/BufferLayout.hpp>
#include <Peanut/Render/Buffers/BufferMapAccess.hpp>
#include <cstdint>
#include <memory>

namespace pn {

class VertexBuffer
{
public:
    VertexBuffer() = default;
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    virtual ~VertexBuffer() = default;

    virtual void Bind() = 0;
    virtual void BindToBindingIndex(uint32_t index) = 0;
    virtual void Unbind() = 0;

    virtual void* Map() = 0;
    virtual void Unmap() = 0;

    virtual const std::shared_ptr<BufferLayout>& GetLayout() const = 0;
    virtual void SetLayout(const std::shared_ptr<BufferLayout>& layout) = 0;

    virtual uint32_t GetSize() const = 0;
    virtual uint32_t GetVertexSize() const = 0;
    virtual uint32_t GetVertexCount() const = 0;

    static std::shared_ptr<VertexBuffer> Create(BufferMapAccess access, uint32_t size, const void* data = nullptr);
};

}
