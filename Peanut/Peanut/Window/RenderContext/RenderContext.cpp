#include "RenderContext.hpp"

#include <Peanut/Window/RenderContext/Impl/OpenGLGLFWRenderContext.hpp>
#include <Peanut/Core/PlatformDetection.hpp>
#include <Peanut/Render/EGraphicsAPI.hpp>
#include <Peanut/Window/Window/Window.hpp>
#include <Peanut/Core/Assert.hpp>

namespace pn
{

std::unique_ptr<RenderContext> RenderContext::Create()
{
    // TODO: select proper RenderContext
    switch (Window::GetWindowProvider()) {
        case WindowProvider::None:     PN_CORE_ASSERT(false, "WindowProvider is not supported");
        case WindowProvider::GLFW:     return std::make_unique<OpenGLGLFWRenderContext>();
    }
    
    PN_CORE_ASSERT(false, "WindowProvider is not supported");
    return nullptr;
}

}