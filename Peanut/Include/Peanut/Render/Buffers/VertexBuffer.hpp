#pragma once

#include <Peanut/Render/Buffers/BufferLayout.hpp>
#include <cstdint>
#include <memory>

namespace pn {

enum class BufferMapAccess : uint32_t
{
    ReadOnly = 0,
    WriteOnly,
    ReadWrite,
};

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

    virtual void* Map(BufferMapAccess access = BufferMapAccess::ReadWrite) = 0;
    virtual void Unmap() = 0;

    virtual const std::shared_ptr<BufferLayout>& GetLayout() const = 0;
    virtual void SetLayout(const std::shared_ptr<BufferLayout>& layout) = 0;

    virtual uint32_t GetSize() const = 0;
    virtual uint32_t GetVertexSize() const = 0;
    virtual uint32_t GetVertexCount() const = 0;

    static std::shared_ptr<VertexBuffer> Create(uint32_t size, const void* data = nullptr);
};

}
