#ifndef __RENDER_H__
#define __RENDER_H__

#include <Peanut/Render/RenderAPI.hpp>
#include <Peanut/Render/RenderCommandImpl.hpp>
#include <glm/vec4.hpp>
#include <memory>

namespace pn {

class RenderCommand
{
public:
    static void Init();
    static RenderAPI GetRenderAPI();

    static void SetClearColor(const glm::vec4& color);
    static void Clear();

private:
    RenderCommand() = delete;

private:
    static RenderAPI s_renderAPI;
    static bool s_isInitialized;
    static std::unique_ptr<RenderCommandImpl> s_renderCommandImpl;
};

}

#endif