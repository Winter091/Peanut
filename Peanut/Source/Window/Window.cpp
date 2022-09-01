#include <Peanut/Window/Window.hpp>

#include <Window/WindowImpl/GLFWWindow.hpp>
#include <Peanut/Core/Assert.hpp>

namespace pn {

    WindowProvider Window::s_windowProvider;
    bool Window::s_isInitialized = false;

    void Window::Init()
    {
        PN_CORE_ASSERT(!s_isInitialized, "You can init window only once");
        s_isInitialized = true;

#if defined(PN_WINDOW_GLFW)
        s_windowProvider = WindowProvider::GLFW;
        PN_CORE_INFO("Using GLFW Window");
#else
        PN_CORE_ASSERT(false, "Window: cannot determine rendering backend")
#endif
    }

    void Window::Shutdown()
    {
        PN_CORE_ASSERT(s_isInitialized, "Window is not initialized, unable to shutdown");
        s_windowProvider = WindowProvider::None;
        s_isInitialized = false; 
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