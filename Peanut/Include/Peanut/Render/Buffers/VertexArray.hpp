#pragma once

#include <Peanut/Render/Buffers/VertexBuffer.hpp>
#include <Peanut/Render/Buffers/IndexBuffer.hpp>
#include <Peanut/Render/Shaders/Shader.hpp>
#include <Peanut/Render/Buffers/BufferLayout.hpp>

namespace pn {

struct VertexArrayDescription
{
public:
    std::vector<std::shared_ptr<VertexBuffer>> VertexBuffers;
    std::shared_ptr<IndexBuffer> IndexBuffer;
    std::shared_ptr<Shader> Shader;

public:

};

class VertexArray
{
public:
    VertexArray() = default;
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    virtual ~VertexArray() = default;

    virtual void Bind() = 0;

    virtual uint32_t GetVertexCount() const = 0;
    virtual uint32_t GetIndexCount() const = 0;
    virtual IndexBufferDataFormat GetIndexDataFormat() const = 0;
    virtual uint32_t GetInstanceCount() const = 0;

    static std::shared_ptr<VertexArray> Create(const VertexArrayDescription& description);
};

}
