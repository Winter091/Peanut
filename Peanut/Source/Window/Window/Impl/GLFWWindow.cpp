#include "GLFWWindow.hpp"

#include <Core/Assert.hpp>
#include <Events/Impl/KeyEvent.hpp>

namespace pn {

GLFWWindow::GLFWWindow(int width, int height, const std::string& title)
    : Window()
{
    PN_CORE_INFO("Using GLFW Window");

    m_renderContext->Setup();

    m_data.m_glfwHandle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    PN_CORE_ASSERT(m_data.m_glfwHandle, "Unable to create GLFW window!");

    m_renderContext->SetCurrentContext(*this);

    glfwSetWindowUserPointer(m_data.m_glfwHandle, &m_data);
    SetupGlfwCallbacks();
}

void GLFWWindow::SetupGlfwCallbacks()
{
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
                case GLFW_RELEASE:
                default:
                    break;
            }
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