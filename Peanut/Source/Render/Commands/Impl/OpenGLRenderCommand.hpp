#ifndef __OPENGLRENDERCOMMAND_H__
#define __OPENGLRENDERCOMMAND_H__

#include <Peanut/Render/Commands/RenderCommandImpl.hpp>

namespace pn {

class OpenGLRenderCommand final : public RenderCommandImpl
{
public:
    OpenGLRenderCommand();
    ~OpenGLRenderCommand() override = default;

    void SetClearColor(const glm::vec4& color) override;
    void Clear() override;

    void DrawArrays(std::shared_ptr<VertexArray>& vertexArray, uint32_t count) override;
    void DrawIndexed(std::shared_ptr<VertexArray>& vertexArray, uint32_t count) override;
    void DrawArraysInstanced(
        std::shared_ptr<VertexArray>& vertexArray, uint32_t count, uint32_t instanceCount) override;
    void DrawIndexedInstanced(
        std::shared_ptr<VertexArray>& vertexArray, uint32_t count, uint32_t instanceCount) override;

private:
    static uint32_t GetGLDataType(IndexBufferDataFormat format);
};

}

#endif