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

        void Draw(std::shared_ptr<VertexArray>& vertexArray, uint32_t count) override;
        void DrawIndexed(std::shared_ptr<VertexArray>& vertexArray, uint32_t count) override;
        void DrawInstanced(std::shared_ptr<VertexArray>& vertexArray, uint32_t count, uint32_t instanceCount) override;
        void DrawIndexedInstanced(std::shared_ptr<VertexArray>& vertexArray, uint32_t count, uint32_t instanceCount) override;

        void BindShader(const std::shared_ptr<Shader>& shader) override;
        void BindConstantBuffers(const std::shared_ptr<ConstantBuffer>* constantBuffers, size_t amount, uint32_t startSlot = 0) override;
        void BindTextures(const std::shared_ptr<Texture>* textures, size_t amount, uint32_t startSlot = 0) override;

    private:
        glm::vec4 m_clearColor;

    private:
        void BindVertexBuffers(const std::shared_ptr<VertexBuffer>* vertexBuffers, size_t amount, uint32_t startSlot = 0);
        void BindVertexArray(const std::shared_ptr<VertexArray>& vertexArray);
    };

}
