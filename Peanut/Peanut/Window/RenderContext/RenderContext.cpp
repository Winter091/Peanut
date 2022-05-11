#include "RenderContext.hpp"

#include <Window/RenderContext/Impl/OpenGLGLFWRenderContext.hpp>
#include <Core/PlatformDetection.hpp>
#include <Render/EGraphicsAPI.hpp>
#include <Window/Window/Window.hpp>
#include <Core/Assert.hpp>

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