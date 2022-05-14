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
    int major = 4, minor = 5;
    PN_CORE_INFO("Requesting OpenGL {}.{}", major, minor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void OpenGLGLFWRenderContext::PostWindowSetup(Window& window)
{
    SetCurrentContext(window);
    
    InitializeGlad();

    auto glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

    PN_CORE_INFO("==========================================");
    PN_CORE_INFO("\tUsing OpenGL {}", glVersion);
    PN_CORE_INFO("\tVendor: {}", vendor);
    PN_CORE_INFO("\tRenderer: {}", renderer);
    PN_CORE_INFO("==========================================");
}

void OpenGLGLFWRenderContext::InitializeGlad()
{
    static bool isGladInitialized = false;

    if (!isGladInitialized) {
        if (!gladLoadGL()) {
            PN_CLIENT_ASSERT(false, "Failed to initialize GLAD!");
        }
    }

    isGladInitialized = true;
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