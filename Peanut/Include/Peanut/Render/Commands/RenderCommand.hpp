#pragma once

#include <Peanut/Render/RenderAPI.hpp>
#include <Peanut/Render/Buffers/PipelineState.hpp>

#include <glm/vec4.hpp>

#include <memory>

namespace pn {

class RenderCommand
{
public:
    RenderCommand() = delete;

    static void Init();
    static void Shutdown();

    static RenderAPI GetRenderAPI();

    static void SetClearColor(const glm::vec4& color);
    static void Clear();

    static void SetViewport(int32_t leftX, int32_t bottomY, uint32_t width, uint32_t height);
    
    static void DrawArrays(std::shared_ptr<PipelineState>& pipelineState, uint32_t count = 0);
    static void DrawIndexed(std::shared_ptr<PipelineState>& pipelineState, uint32_t count = 0);
    static void DrawArraysInstanced(
        std::shared_ptr<PipelineState>& pipelineState, uint32_t count = 0, uint32_t instanceCount = 0);
    static void DrawIndexedInstanced(
        std::shared_ptr<PipelineState>& pipelineState, uint32_t count = 0, uint32_t instanceCount = 0);
};

}
