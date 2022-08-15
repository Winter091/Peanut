#pragma once

#include <Render/Commands/RenderCommandImpl.hpp>

#include <glm/vec4.hpp>

namespace pn {

class Dx11RenderCommand final : public RenderCommandImpl
{
public:
    Dx11RenderCommand();
    ~Dx11RenderCommand() override = default;

    void SetClearColor(const glm::vec4& color) override;
    void Clear() override;

    void SetViewport(int32_t leftX, int32_t bottomY, uint32_t width, uint32_t height) override;

    void DrawArrays(std::shared_ptr<VertexArray>& pipelineState, uint32_t count) override;
    void DrawIndexed(std::shared_ptr<VertexArray>& pipelineState, uint32_t count) override;
    void DrawArraysInstanced(
        std::shared_ptr<VertexArray>& pipelineState, uint32_t count, uint32_t instanceCount) override;
    void DrawIndexedInstanced(
        std::shared_ptr<VertexArray>& pipelineState, uint32_t count, uint32_t instanceCount) override;

private:
    glm::vec4 m_clearColor;
};

}