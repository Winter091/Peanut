#include "GLFWWindow.hpp"

#include <spdlog/spdlog.h>

namespace pn {

GLFWWindow::GLFWWindow(int width, int height, const std::string& title)
    : Window(width, height, title)
{
    spdlog::info("Using GLFW Window");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_glfwHandle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (m_glfwHandle == NULL) {
        spdlog::critical("Unable to create GLFW window!\n");
        glfwTerminate();
        exit(1);
    }

    m_renderContext->SetWindow(*this);
}

GLFWWindow::~GLFWWindow()
{
    glfwTerminate();
}

void GLFWWindow::SetEventCallbackFunc()
{
    
}

void GLFWWindow::Update()
{
    glfwPollEvents();
    m_renderContext->SwapBuffers(*this);
}

bool GLFWWindow::ShouldClose() const
{
    return static_cast<bool>(glfwWindowShouldClose(m_glfwHandle));
}

}