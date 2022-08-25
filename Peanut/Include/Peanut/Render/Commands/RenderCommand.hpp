#pragma once

#include <Peanut/Render/RenderAPI.hpp>
#include <Peanut/Render/Buffers/VertexArray.hpp>

#include <glm/vec4.hpp>

#include <vector>
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
    
    static void Draw(std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0);
    static void DrawIndexed(std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0);
    static void DrawInstanced(std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0, uint32_t instanceCount = 0);
    static void DrawIndexedInstanced(std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0, uint32_t instanceCount = 0);

    static void BindShader(const std::shared_ptr<Shader>& shader);
    static void BindConstantBuffers(const std::shared_ptr<ConstantBuffer>* constantBuffers, size_t amount, uint32_t startSlot = 0);
    static void BindTextures(const std::shared_ptr<Texture>* textures, size_t numTextures, uint32_t startSlot = 0);
};

}
