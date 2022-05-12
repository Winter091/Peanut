#include "OpenGLGLFWRenderContext.hpp"

#include <Peanut/Window/Window/Impl/GLFWWindow.hpp>

#include <Peanut/Core/Log.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace pn
{

OpenGLGLFWRenderContext::OpenGLGLFWRenderContext()
{
    PN_CORE_INFO("Using OpenGL-GLFW Render Context");
}

void OpenGLGLFWRenderContext::PreWindowSetup()
{
    int major = 4, minor = 4;
    PN_CORE_INFO("Requesting OpenGL {}.{}", major, minor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void OpenGLGLFWRenderContext::PostWindowSetup(Window& window)
{
    SetCurrentContext(window);
    
    if (!gladLoadGL()) {
        PN_CLIENT_ASSERT(false, "Failed to initialize GLAD!");
    }
}

void OpenGLGLFWRenderContext::SetCurrentContext(Window& window)
{
    auto handle = reinterpret_cast<GLFWwindow*>(window.GetNativeHandle());
    glfwMakeContextCurrent(handle);
}

void OpenGLGLFWRenderContext::SwapBuffers(Window& window)
{
    auto handle = reinterpret_cast<GLFWwindow*>(window.GetNativeHandle());
    glfwSwapBuffers(handle);
}

}