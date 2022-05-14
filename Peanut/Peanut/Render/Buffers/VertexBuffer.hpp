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
    virtual void Unbind() = 0;

    virtual void ReplaceData(const void* data, uint32_t size) = 0;
    virtual void UpdateData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

    virtual const BufferLayout& GetLayout() const = 0;
    virtual void SetLayout(const BufferLayout& layout) = 0;

    static std::shared_ptr<VertexBuffer> Create(uint32_t size, const void* data = nullptr);
};

}

#endif
