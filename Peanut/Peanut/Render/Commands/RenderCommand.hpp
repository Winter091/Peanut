#ifndef __RENDERCOMMAND_H__
#define __RENDERCOMMAND_H__

#include <Peanut/Render/RenderAPI.hpp>
#include <Peanut/Render/Commands/RenderCommandImpl.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <memory>

namespace pn {

class RenderCommand
{
public:
    RenderCommand() = delete;

    static void Init();
    static RenderAPI GetRenderAPI();

    static void SetClearColor(const glm::vec4& color);
    static void Clear();

    static void SetViewport(int32_t leftX, int32_t bottomY, uint32_t width, uint32_t height);
    
    static void DrawArrays(std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0);
    static void DrawIndexed(std::shared_ptr<VertexArray>& vertexArray, uint32_t count = 0);

private:
    static RenderAPI s_renderAPI;
    static bool s_isInitialized;
    static std::unique_ptr<RenderCommandImpl> s_renderCommandImpl;
};

}

#endif