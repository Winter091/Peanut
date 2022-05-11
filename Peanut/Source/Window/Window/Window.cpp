#include "Window.hpp"

#include "Window/Window/Impl/GLFWWindow.hpp"
#include "Core/PlatformDetection.hpp"
#include <Core/Assert.hpp>

namespace pn {


// Static members
EWindowProvider Window::s_windowProvider;
bool Window::s_isInitialized = false;


Window::Window(int width, int height, std::string title)
    : m_width(width)
    , m_height(height)
    , m_title(std::move(title))
    , m_renderContext(RenderContext::Create())
{

}

void Window::Init()
{
    PN_CORE_ASSERT(!s_isInitialized, "You can init window only once");
    s_isInitialized = true;

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
    PN_CORE_ASSERT(s_isInitialized, "You must init window before trying to create it");

    switch (s_windowProvider) {
        case EWindowProvider::None:     PN_CORE_ASSERT(false, "Window provider is not supported");
        case EWindowProvider::GLFW:     return std::make_unique<GLFWWindow>(width, height, title);
    }

    PN_CORE_ASSERT(false, "Window provider is not supported");
    return nullptr;
}

EWindowProvider Window::GetWindowProvider()
{
    PN_CORE_ASSERT(s_isInitialized, "You must init window before trying to get window provider");
    return s_windowProvider;
}

}