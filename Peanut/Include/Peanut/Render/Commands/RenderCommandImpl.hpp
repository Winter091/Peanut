#ifndef __RENDERCOMMANDIMPL_H__
#define __RENDERCOMMANDIMPL_H__

#include <Peanut/Render/Buffers/VertexArray.hpp>
#include <Peanut/Render/RenderAPI.hpp>
#include <glm/vec4.hpp>

#include <memory>

namespace pn {

class RenderCommandImpl
{
public:
    RenderCommandImpl() = default;
    virtual ~RenderCommandImpl() = default;
    RenderCommandImpl(const RenderCommandImpl&) = delete;
    RenderCommandImpl& operator=(const RenderCommandImpl&) = delete;

    virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void Clear() = 0;

    virtual void DrawArrays(std::shared_ptr<VertexArray>& vertexArray, uint32_t count) = 0;
    virtual void DrawIndexed(std::shared_ptr<VertexArray>& vertexArray, uint32_t count) = 0;
    virtual void DrawArraysInstanced(
        std::shared_ptr<VertexArray>& vertexArray, uint32_t count, uint32_t instanceCount) = 0;
    virtual void DrawIndexedInstanced(
        std::shared_ptr<VertexArray>& vertexArray, uint32_t count, uint32_t instanceCount) = 0;
};

}

#endif