#include "OpenGLGLFWRenderContext.hpp"

#include <Window/Window/Impl/GLFWWindow.hpp>

#include <Core/Log.hpp>
#include <GLFW/glfw3.h>

namespace pn
{

OpenGLGLFWRenderContext::OpenGLGLFWRenderContext()
{
    PN_CORE_INFO("Using OpenGL-GLFW Render Context");
}

void OpenGLGLFWRenderContext::Setup()
{
    int major = 4, minor = 4;
    PN_CORE_INFO("Requesting OpenGL {}.{}", major, minor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void OpenGLGLFWRenderContext::SetCurrentContext(Window& window)
{
    GLFWwindow* handle = reinterpret_cast<GLFWwindow*>(window.GetNativeHandle());
    glfwMakeContextCurrent(handle);
}

void OpenGLGLFWRenderContext::SwapBuffers(Window& window)
{
    GLFWwindow* handle = reinterpret_cast<GLFWwindow*>(window.GetNativeHandle());
    glfwSwapBuffers(handle);
}

}