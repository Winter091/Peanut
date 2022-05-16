#ifndef __DX11RENDERCOMMAND_H__
#define __DX11RENDERCOMMAND_H__

#include <Peanut/Render/Commands/RenderCommandImpl.hpp>

#include <glm/vec4.hpp>

namespace pn {

class DX11RenderCommand final : public RenderCommandImpl
{
public:
    DX11RenderCommand();
    ~DX11RenderCommand() override = default;

    void SetClearColor(const glm::vec4& color) override;
    void Clear() override;

    void DrawArrays(std::shared_ptr<VertexArray>& vertexArray, uint32_t count) override;
    void DrawIndexed(std::shared_ptr<VertexArray>& vertexArray, uint32_t count) override;

private:
    glm::vec4 m_clearColor;
};

}


#endif