#include "RenderContext.hpp"

#include <Peanut/Window/RenderContext/Impl/OpenGLGLFWRenderContext.hpp>
#include <Peanut/Window/RenderContext/Impl/DX11GLFWRenderContext.hpp>
#include <Peanut/Core/PlatformDetection.hpp>
#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Window/Window/Window.hpp>
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
                case RenderAPI::DX11:
                    return std::make_unique<DX11GLFWRenderContext>();
            }
        }
    }

    PN_CORE_ASSERT(false, "WindowProvider is not supported");
    return nullptr;
}

}