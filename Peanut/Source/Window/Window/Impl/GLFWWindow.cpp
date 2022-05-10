#include "GLFWWindow.hpp"

#include <spdlog/spdlog.h>

namespace pn {

GLFWWindow::GLFWWindow(int width, int height, const std::string& title)
    : Window(width, height, title)
{
    spdlog::info("Using GLFW Window");

    m_renderContext->Setup();

    m_glfwHandle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (m_glfwHandle == NULL) {
        spdlog::critical("Unable to create GLFW window!\n");
        glfwTerminate();
        exit(1);
    }

    m_renderContext->SetCurrentContext(*this);
}

GLFWWindow::~GLFWWindow()
{
    glfwDestroyWindow(m_glfwHandle);
}

void GLFWWindow::MakeContextCurrent()
{
    glfwMakeContextCurrent(m_glfwHandle);
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

void* GLFWWindow::GetNativeHandle() const 
{
    return reinterpret_cast<void*>(m_glfwHandle);
}

}