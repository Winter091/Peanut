#ifndef __VERTEXARRAY_H__
#define __VERTEXARRAY_H__

#include <Peanut/Render/Buffers/VertexBuffer.hpp>

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

    virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;

    static std::shared_ptr<VertexArray> Create();
};

}

#endif