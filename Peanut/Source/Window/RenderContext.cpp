#include <Peanut/Window/RenderContext.hpp>

#include <Window/RenderContextImpl/OpenGLGLFWRenderContext.hpp>
#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Window/Window.hpp>
#include <Peanut/Core/Assert.hpp>

namespace pn
{

std::unique_ptr<RenderContext> RenderContext::Create()
{
    auto windowProvider = Window::GetWindowProvider();
    auto renderAPI = RenderCommand::GetRenderAPI();

    switch (windowProvider) {
        case WindowProvider::None:
        {
            PN_CORE_ASSERT(false, "WindowProvider is not supported");
            return nullptr;
        }
        case WindowProvider::GLFW:
        {
            switch (renderAPI) {
                case RenderAPI::None:
                    PN_CORE_ASSERT(false, "RenderAPI is not supported");
                    return nullptr;
                case RenderAPI::OpenGL:
                    return std::make_unique<OpenGLGLFWRenderContext>();
            }
        }
    }

    PN_CORE_ASSERT(false, "WindowProvider is not supported");
    return nullptr;
}

}