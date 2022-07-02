#ifndef __VERTEXBUFFER_H__
#define __VERTEXBUFFER_H__

#include <Peanut/Render/Buffers/BufferLayout.hpp>
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
    virtual void BindToBindingIndex(int index) = 0;
    virtual void Unbind() = 0;

    virtual void ReplaceData(uint32_t size, const void* data) = 0;
    virtual void UpdateData(uint32_t size, const void* data, uint32_t offset = 0) = 0;

    virtual const std::shared_ptr<BufferLayout>& GetLayout() const = 0;
    virtual void SetLayout(const std::shared_ptr<BufferLayout>& layout) = 0;

    virtual uint32_t GetSize() const = 0;
    virtual uint32_t GetVertexSize() const = 0;
    virtual uint32_t GetVertexCount() const = 0;

    static std::shared_ptr<VertexBuffer> Create(uint32_t size, const void* data = nullptr);
};

}

#endif
