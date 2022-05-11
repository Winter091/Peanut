#include "Window.hpp"

#include <Peanut/Window/Window/Impl/GLFWWindow.hpp>
#include <Peanut/Core/PlatformDetection.hpp>
#include <Peanut/Core/Assert.hpp>

namespace pn {


// Static members
WindowProvider Window::s_windowProvider;
bool Window::s_isInitialized = false;


Window::Window()
    : m_renderContext(RenderContext::Create())
{

}

void Window::Init()
{
    PN_CORE_ASSERT(!s_isInitialized, "You can init window only once");
    s_isInitialized = true;

#if defined(PN_PLATFORM_WINDOWS)
    s_windowProvider = WindowProvider::GLFW;
#elif defined(PN_PLATFORM_LINUX)
    s_windowProvider = WindowProvider::GLFW;
#endif
}

std::unique_ptr<Window> Window::Create(int width, int height, const std::string& title)
{
    PN_CORE_ASSERT(s_isInitialized, "You must init window before trying to create it");

    switch (s_windowProvider) {
        case WindowProvider::None:     PN_CORE_ASSERT(false, "Window provider is not supported");
        case WindowProvider::GLFW:     return std::make_unique<GLFWWindow>(width, height, title);
    }

    PN_CORE_ASSERT(false, "Window provider is not supported");
    return nullptr;
}

WindowProvider Window::GetWindowProvider()
{
    PN_CORE_ASSERT(s_isInitialized, "You must init window before trying to get window provider");
    return s_windowProvider;
}

}