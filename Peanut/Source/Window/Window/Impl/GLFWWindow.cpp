#include "GLFWWindow.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Events/KeyEvents.hpp>
#include <Peanut/Events/MouseEvents.hpp>
#include <Peanut/Events/WindowEvents.hpp>

namespace pn {


int GLFWWindow::s_windowCount = 0;


GLFWWindow::GLFWWindow(const WindowSettings& settings)
    : Window()
{
    m_data.width = settings.width;
    m_data.height = settings.height;
    m_data.title = settings.title;

    OnWindowCreate();

    GetRenderContext().PreWindowSetup();
    {
        m_data.glfwHandle = glfwCreateWindow(
            m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr
        );
        PN_CORE_ASSERT(m_data.glfwHandle, "Unable to create GLFW window!");
    }
    GetRenderContext().PostWindowSetup(*this);
    
    SetupGlfwCallbacks();
    SetSwapInterval(settings.swapInterval);
}

void GLFWWindow::OnWindowCreate()
{
    if (s_windowCount == 0) {
        glfwInit();
    }
    ++s_windowCount;
}

void GLFWWindow::SetupGlfwCallbacks()
{
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

            WindowSizeChangedEvent event(width, height);
            data->eventCallbackFunc(event);
        }
    );
}

GLFWWindow::~GLFWWindow()
{
    glfwDestroyWindow(m_data.glfwHandle);
    OnWindowDestroy();
}

void GLFWWindow::OnWindowDestroy()
{
    --s_windowCount;
    if (s_windowCount == 0) {
        glfwTerminate();
    }
}

void GLFWWindow::MakeContextCurrent()
{
    glfwMakeContextCurrent(m_data.glfwHandle);
}

void GLFWWindow::SetEventCallbackFunc(const EventCallbackFunc& func)
{
    m_data.eventCallbackFunc = func;
}

void GLFWWindow::Update()
{
    glfwPollEvents();
    GetRenderContext().SwapBuffers(*this);
}

bool GLFWWindow::ShouldClose() const
{
    return static_cast<bool>(glfwWindowShouldClose(m_data.glfwHandle));
}

void GLFWWindow::OnResize(int newWidth, int newHeight)
{
    m_data.width = newWidth;
    m_data.height = newHeight;
}

void* GLFWWindow::GetNativeHandle() const 
{
    return reinterpret_cast<void*>(m_data.glfwHandle);
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

}