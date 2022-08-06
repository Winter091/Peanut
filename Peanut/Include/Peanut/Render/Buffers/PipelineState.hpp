#pragma once

#include <Peanut/Render/Buffers/VertexBuffer.hpp>
#include <Peanut/Render/Buffers/IndexBuffer.hpp>
#include <Peanut/Render/Shaders/Shader.hpp>
#include <Peanut/Render/Buffers/BufferLayout.hpp>

namespace pn {

struct PipelineStateDescription
{
public:
    std::vector<std::shared_ptr<VertexBuffer>> VertexBuffers;
    std::shared_ptr<IndexBuffer> IndexBuffer;
    std::shared_ptr<Shader> Shader;

public:

};

class PipelineState
{
public:
    PipelineState() = default;
    PipelineState(const PipelineState&) = delete;
    PipelineState& operator=(const PipelineState&) = delete;
    virtual ~PipelineState() = default;

    virtual void Bind() = 0;

    virtual uint32_t GetVertexCount() const = 0;
    virtual uint32_t GetIndexCount() const = 0;
    virtual IndexBufferDataFormat GetIndexDataFormat() const = 0;
    virtual uint32_t GetInstanceCount() const = 0;

    static std::shared_ptr<PipelineState> Create(const PipelineStateDescription& description);
};

}
