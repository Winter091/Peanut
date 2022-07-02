#include <Peanut/Window/Window/Window.hpp>

#include <Window/Window/Impl/GLFWWindow.hpp>
#include <Core/PlatformDetection.hpp>
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

std::unique_ptr<Window> Window::Create(const WindowSettings& settings)
{
    PN_CORE_ASSERT(s_isInitialized, "You must init window before trying to create it");

    switch (s_windowProvider) {
        case WindowProvider::None:     PN_CORE_ASSERT(false, "Window provider is not supported");
        case WindowProvider::GLFW:     return std::make_unique<GLFWWindow>(settings);
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