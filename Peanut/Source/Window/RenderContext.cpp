#include <Peanut/Window/RenderContext.hpp>

#include <Peanut/Render/Commands/RenderCommand.hpp>
#include <Peanut/Window/Window.hpp>
#include <Peanut/Core/Assert.hpp>

#if defined(PN_RENDERING_OPENGL)
#include <Window/RenderContextImpl/OpenGLGLFWRenderContext.hpp>
#endif

#if defined(PN_RENDERING_DX11)
#include <Window/RenderContextImpl/Dx11GLFWRenderContext.hpp>
#endif

namespace pn {

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

#if defined(PN_RENDERING_OPENGL)
                    case RenderAPI::OpenGL:
                        return std::make_unique<OpenGLGLFWRenderContext>();
#endif

#if defined(PN_RENDERING_DX11)
                    case RenderAPI::Dx11:
                        return std::make_unique<Dx11GLFWRenderContext>();
#endif

                    default:
                        break;
                }
            }
        }

        PN_CORE_ASSERT(false, "WindowProvider is not supported");
        return nullptr;
    }

}
