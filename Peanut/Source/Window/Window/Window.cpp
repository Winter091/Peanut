#include "Window.hpp"

#include "Window/Window/Impl/GLFWWindow.hpp"
#include "Core/PlatformDetection.hpp"
#include <cassert>

namespace pn {


// Static members
EWindowProvider Window::s_windowProvider;
bool Window::s_initted = false;


Window::Window(int width, int height, const std::string& title)
    : m_width(width)
    , m_height(height)
    , m_title(title)
    , m_renderContext(RenderContext::Create())
{

}

void Window::Init()
{
    assert(!s_initted && "You can init window only once");
    s_initted = true;

#if defined(PN_PLATFORM_WINDOWS)
    s_windowProvider = EWindowProvider::GLFW;
    glfwInit();
#elif defined(PN_PLATFORM_LINUX)
    s_windowProvider = EWindowProvider::GLFW;
    glfwInit();
#endif
}

std::unique_ptr<Window> Window::Create(int width, int height, const char* title)
{
    assert(s_initted && "You must init window before trying to create it");

    switch (s_windowProvider) {
        case EWindowProvider::None:     assert(0 && "Window provider is not supported");
        case EWindowProvider::GLFW:     return std::make_unique<GLFWWindow>(width, height, title);
    }

    assert(0 && "Window provider is not supported");
    return nullptr;
}

EWindowProvider Window::GetWindowProvider()
{
    assert(s_initted && "You must init window before trying to get window provider");
    return s_windowProvider;
}

}