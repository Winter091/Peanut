#ifndef __VERTEXARRAY_H__
#define __VERTEXARRAY_H__

#include <Peanut/Render/Buffers/VertexBuffer.hpp>
#include <Peanut/Render/Buffers/IndexBuffer.hpp>

namespace pn {

class VertexArray
{
public:
    VertexArray() = default;
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    virtual ~VertexArray() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
    virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

    virtual bool GetIsVertexBufferInitialized() const = 0;
    virtual bool GetIsIndexBufferInitialized() const = 0;

    virtual uint32_t GetSize() const = 0;
    virtual uint32_t GetVertexCount() const = 0;
    virtual uint32_t GetIndexCount() const = 0;
    virtual IndexBufferDataFormat GetIndexDataFormat() const = 0;

    static std::shared_ptr<VertexArray> Create();
};

}

#endif
