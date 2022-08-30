#include "GLFWWindow.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Core/TimeProfiler.hpp>
#include <Peanut/Events/KeyEvents.hpp>
#include <Peanut/Events/MouseEvents.hpp>
#include <Peanut/Events/WindowEvents.hpp>

namespace pn {

    int GLFWWindow::s_windowCount = 0;

    GLFWWindow::GLFWWindow(const WindowSettings& settings)
        : Window()
    {
        PN_PROFILE_FUNCTION();

        m_data.thisPtr = this;
        m_data.renderContext = RenderContext::Create();
        m_data.width =  static_cast<int>(settings.Width);
        m_data.height = static_cast<int>(settings.Height);
        m_data.title = settings.Title;
        m_data.isFullScreen = settings.IsFullScreen;

        OnWindowCreate();

        m_data.renderContext->PreWindowSetup();
        {
            GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
            GLFWmonitor* fullscreenMonitor = settings.IsFullScreen ? primaryMonitor : nullptr;

            const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

            m_data.glfwHandle = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), fullscreenMonitor, nullptr);
            PN_CORE_ASSERT(m_data.glfwHandle, "Unable to create GLFW window!");
        }
        m_data.renderContext->PostWindowSetup(*this);
    
        SetupGlfwCallbacks();
        SetSwapInterval(static_cast<int>(settings.SwapInterval));
    }

    void GLFWWindow::OnWindowCreate()
    {
        PN_PROFILE_FUNCTION();

        if (s_windowCount == 0) {
            glfwInit();
        }
        ++s_windowCount;
    }

    void GLFWWindow::SetupGlfwCallbacks()
    {
        PN_PROFILE_FUNCTION();

        glfwSetWindowUserPointer(m_data.glfwHandle, &m_data);
    
        glfwSetKeyCallback(m_data.glfwHandle,
            [](GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/) {
                auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                switch (action) {
                    case GLFW_PRESS:
                    {
                        KeyPressedEvent event(static_cast<KeyCode>(key));
                        data->eventCallbackFunc(event);
                        break;
                    }
                    case GLFW_REPEAT:
                    {
                        KeyRepeatedEvent event(static_cast<KeyCode>(key));
                        data->eventCallbackFunc(event);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        KeyReleasedEvent event(static_cast<KeyCode>(key));
                        data->eventCallbackFunc(event);
                        break;
                    }
                    default:
                        break;
                }
            }
        );

        glfwSetMouseButtonCallback(m_data.glfwHandle,
            [](GLFWwindow* window, int button, int action, int /*mods*/) {
                auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                switch (action) {
                    case GLFW_PRESS:
                    {
                        MouseButtonPressedEvent event(static_cast<MouseButtonCode>(button));
                        data->eventCallbackFunc(event);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        MouseButtonReleasedEvent event(static_cast<MouseButtonCode>(button));
                        data->eventCallbackFunc(event);
                        break;
                    }
                    default:
                        break;
                }
            }
        );

        glfwSetCursorPosCallback(m_data.glfwHandle,
            [](GLFWwindow* window, double xpos, double ypos) {
                auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                MousePosChangedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
                data->eventCallbackFunc(event);
            }
        );

        glfwSetScrollCallback(m_data.glfwHandle,
            [](GLFWwindow* window, double /*xoffset*/, double yoffset) {
                auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                MouseScrolledEvent event(static_cast<float>(yoffset));
                data->eventCallbackFunc(event);
            }
        );

        glfwSetFramebufferSizeCallback(m_data.glfwHandle, 
            [](GLFWwindow* window, int width, int height) {
                auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            
                data->width = width;
                data->height = height;
                data->renderContext->OnWindowResize(pn::WindowSizeSettings()
                    .SetWidth(width)
                    .SetHeight(height)
                    .SetIsFullScreen(data->thisPtr->GetIsFullScreen()));

                WindowSizeChangedEvent event(width, height);
                data->eventCallbackFunc(event);
            }
        );
    }

    GLFWWindow::~GLFWWindow()
    {
        PN_PROFILE_FUNCTION();

        glfwDestroyWindow(m_data.glfwHandle);
        OnWindowDestroy();
    }

    void GLFWWindow::OnWindowDestroy()
    {
        PN_PROFILE_FUNCTION();

        --s_windowCount;
        if (s_windowCount == 0) {
            glfwTerminate();
        }
    }

    void GLFWWindow::MakeContextCurrent()
    {
        PN_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_data.glfwHandle);
    }

    void GLFWWindow::SetEventCallbackFunc(const EventCallbackFunc& func)
    {
        m_data.eventCallbackFunc = func;
    }

    void GLFWWindow::Update()
    {
        PN_PROFILE_FUNCTION();

        glfwPollEvents();
        GetRenderContext().SwapBuffers(*this);
    }

    bool GLFWWindow::ShouldClose() const
    {
        return static_cast<bool>(glfwWindowShouldClose(m_data.glfwHandle));
    }

    void* GLFWWindow::GetNativeHandle() const 
    {
        return reinterpret_cast<void*>(m_data.glfwHandle);
    }

    void GLFWWindow::SetSize(const WindowSizeSettings& settings)
    {
        if (settings.IsFullScreen == m_data.isFullScreen) {
            glfwSetWindowSize(m_data.glfwHandle, settings.Width, settings.Height);
        } else {
            GLFWmonitor* primaryMonitor = nullptr;
            int prevXPos = 0, prevYPos = 0, refreshRate = GLFW_DONT_CARE;
            
            if (settings.IsFullScreen) {
                primaryMonitor = glfwGetPrimaryMonitor();
                refreshRate = glfwGetVideoMode(primaryMonitor)->refreshRate;
                glfwGetWindowPos(m_data.glfwHandle, &m_data.prevWindowedXPos, &m_data.prevWindowedYPos);
            } else {
                prevXPos = m_data.prevWindowedXPos;
                prevYPos = m_data.prevWindowedYPos;
            }

            glfwSetWindowMonitor(m_data.glfwHandle, primaryMonitor, prevXPos, prevYPos, settings.Width, settings.Height, GLFW_DONT_CARE);//refreshRate);
        }

        SetSwapInterval(m_data.swapInterval);

        m_data.width = settings.Width;
        m_data.height = settings.Height;
        m_data.isFullScreen = settings.IsFullScreen;
        m_data.renderContext->OnWindowResize(settings);

        WindowSizeChangedEvent event(m_data.width, m_data.height);
        m_data.eventCallbackFunc(event);
    }

    void GLFWWindow::SetTitle(const std::string& title) 
    {
        m_data.title = title;
        glfwSetWindowTitle(m_data.glfwHandle, m_data.title.c_str());
    }

    void GLFWWindow::SetSwapInterval(int interval)
    {
        PN_CORE_ASSERT(interval >= 0, "Negative swap interval is provided: {}", interval);
        m_data.swapInterval = interval;
        GetRenderContext().SetSwapInterval(interval);
    }

    bool GLFWWindow::IsKeyPressed(KeyCode key) const 
    {
        return (glfwGetKey(m_data.glfwHandle, KeyCodeToGlfwInt(key)) == GLFW_PRESS);
    }

    bool GLFWWindow::IsMouseButtonPressed(MouseButtonCode button) const 
    {
        return (glfwGetMouseButton(m_data.glfwHandle, MouseButtonCodeToGlfwInt(button)) == GLFW_PRESS);
    }

    int GLFWWindow::KeyCodeToGlfwInt(KeyCode key)
    {
        return static_cast<int>(key);
    }

    int GLFWWindow::MouseButtonCodeToGlfwInt(MouseButtonCode button)
    {
        return static_cast<int>(button);
    }

    glm::vec2 GLFWWindow::GetCursorPosition() const 
    {
        double x, y;
        glfwGetCursorPos(m_data.glfwHandle, &x, &y);
        return { static_cast<float>(x), static_cast<float>(y) };
    }

}