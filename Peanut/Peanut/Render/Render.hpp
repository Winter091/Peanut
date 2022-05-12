#ifndef __RENDER_H__
#define __RENDER_H__

#include <Peanut/Render/RenderAPI.hpp>
#include <Peanut/Render/RenderCommand.hpp>
#include <glm/vec4.hpp>
#include <memory>

namespace pn {

class Render
{
public:
    static void Init();
    static RenderAPI GetRenderAPI();

    static void SetClearColor(const glm::vec4& color);
    static void Clear();

private:
    Render() = delete;

private:
    static RenderAPI s_renderAPI;
    static bool s_isInitialized;
    static std::unique_ptr<RenderCommand> s_renderCommand;
};

}

#endif