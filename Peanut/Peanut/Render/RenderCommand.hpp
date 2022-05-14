#ifndef __RENDERCOMMAND_H__
#define __RENDERCOMMAND_H__

#include <Peanut/Render/RenderAPI.hpp>
#include <Peanut/Render/RenderCommandImpl.hpp>
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
    
    static void DrawArrays(uint32_t first, uint32_t count);

private:
    static RenderAPI s_renderAPI;
    static bool s_isInitialized;
    static std::unique_ptr<RenderCommandImpl> s_renderCommandImpl;
};

}

#endif