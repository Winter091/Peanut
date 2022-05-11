#include "GLFWWindow.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Events/Impl/KeyEvents.hpp>
#include <Peanut/Events/Impl/MouseEvents.hpp>
#include <Peanut/Events/Impl/WindowEvents.hpp>

namespace pn {

GLFWWindow::GLFWWindow(int width, int height, const std::string& title)
    : Window()
{
    PN_CORE_INFO("Using GLFW Window");

    m_renderContext->Setup();

    m_data.m_glfwHandle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    PN_CORE_ASSERT(m_data.m_glfwHandle, "Unable to create GLFW window!");

    m_renderContext->SetCurrentContext(*this);

    SetupGlfwCallbacks();
}

void GLFWWindow::SetupGlfwCallbacks()
{
    glfwSetWindowUserPointer(m_data.m_glfwHandle, &m_data);
    
    glfwSetKeyCallback(m_data.m_glfwHandle, 
        [](GLFWwindow *window, int key, int /*scancode*/, int action, int /*mods*/) {
            WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

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

    glfwSetMouseButtonCallback(m_data.m_glfwHandle, 
        [](GLFWwindow* window, int button, int action, int /*mods*/) {
            WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

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

    glfwSetCursorPosCallback(m_data.m_glfwHandle, 
        [](GLFWwindow* window, double xpos, double ypos) {
            WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MousePosChangedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
            data->eventCallbackFunc(event);
        }
    );

    glfwSetScrollCallback(m_data.m_glfwHandle, 
        [](GLFWwindow* window, double /*xoffset*/, double yoffset) {
            WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            MouseScrolledEvent event(static_cast<float>(yoffset));
            data->eventCallbackFunc(event);
        }
    );

    glfwSetWindowSizeCallback(m_data.m_glfwHandle, 
        [](GLFWwindow* window, int width, int height) {
            WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            WindowSizeChangedEvent event(width, height);
            data->eventCallbackFunc(event);
        }
    );

    glfwSetFramebufferSizeCallback(m_data.m_glfwHandle, 
        [](GLFWwindow* window, int width, int height) {
            WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            FramebufferSizeChangedEvent event(width, height);
            data->eventCallbackFunc(event);
        }
    );
}

GLFWWindow::~GLFWWindow()
{
    glfwDestroyWindow(m_data.m_glfwHandle);
}

void GLFWWindow::MakeContextCurrent()
{
    glfwMakeContextCurrent(m_data.m_glfwHandle);
}

void GLFWWindow::SetEventCallbackFunc(const EventCallbackFunc& func)
{
    m_data.eventCallbackFunc = func;
}

void GLFWWindow::Update()
{
    glfwPollEvents();
    m_renderContext->SwapBuffers(*this);
}

bool GLFWWindow::ShouldClose() const
{
    return static_cast<bool>(glfwWindowShouldClose(m_data.m_glfwHandle));
}

void* GLFWWindow::GetNativeHandle() const 
{
    return reinterpret_cast<void*>(m_data.m_glfwHandle);
}

}