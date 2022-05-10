#include "OpenGLGLFWRenderContext.hpp"

#include <Window/Window/Impl/GLFWWindow.hpp>

#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

namespace pn
{

OpenGLGLFWRenderContext::OpenGLGLFWRenderContext()
{
    spdlog::info("Using OpenGL-GLFW Render Context");
}

OpenGLGLFWRenderContext::~OpenGLGLFWRenderContext()
{
    
}

void OpenGLGLFWRenderContext::SetWindow(Window& window)
{
    GLFWWindow& w = reinterpret_cast<GLFWWindow&>(window);
    glfwMakeContextCurrent(w.GetGLFWHandle());
}

void OpenGLGLFWRenderContext::SwapBuffers(Window& window)
{
    GLFWWindow& w = reinterpret_cast<GLFWWindow&>(window);
    glfwSwapBuffers(w.GetGLFWHandle());
}

}