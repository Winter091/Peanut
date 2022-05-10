#include "RenderContext.hpp"

#include <Window/RenderContext/Impl/OpenGLGLFWRenderContext.hpp>
#include <Core/PlatformDetection.hpp>
#include <Render/EGraphicsAPI.hpp>
#include <Window/Window/Window.hpp>
#include <cassert>

namespace pn
{

std::unique_ptr<RenderContext> RenderContext::Create()
{
    // TODO: select proper RenderContext
    switch (Window::GetWindowProvider()) {
        case EWindowProvider::None:     assert(0 && "WindowProvider is not supported");
        case EWindowProvider::GLFW:     return std::make_unique<OpenGLGLFWRenderContext>();
    }
    
    assert(0 && "WindowProvider is not supported");
    return nullptr;
}

}