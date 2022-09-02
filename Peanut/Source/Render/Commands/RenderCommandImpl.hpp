#pragma once

#include <Peanut/Render/Buffers/VertexArray.hpp>
#include <Peanut/Render/Framebuffers/Framebuffer.hpp>
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

        virtual void SetViewport(int32_t leftX, int32_t bottomY, uint32_t width, uint32_t height) = 0;

        virtual void Draw(std::shared_ptr<VertexArray>& vertexArray, uint32_t count) = 0;
        virtual void DrawIndexed(std::shared_ptr<VertexArray>& vertexArray, uint32_t count) = 0;
        virtual void DrawInstanced(std::shared_ptr<VertexArray>& vertexArray, uint32_t count, uint32_t instanceCount) = 0;
        virtual void DrawIndexedInstanced(std::shared_ptr<VertexArray>& vertexArray, uint32_t count, uint32_t instanceCount) = 0;

        virtual void BindFramebuffer(const std::shared_ptr<Framebuffer>& framebuffer) = 0;
        virtual void BindShader(const std::shared_ptr<Shader>& shader) = 0;
        virtual void BindConstantBuffers(const std::shared_ptr<ConstantBuffer>* constantBuffers, size_t amount, uint32_t startSlot = 0) = 0;
        virtual void BindTextures(const std::shared_ptr<Texture>* textures, size_t amount, uint32_t startSlot = 0) = 0;
    };

}
