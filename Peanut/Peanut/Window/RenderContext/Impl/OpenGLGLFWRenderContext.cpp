#include "OpenGLGLFWRenderContext.hpp"

#include <Peanut/Window/Window/Impl/GLFWWindow.hpp>

#include <Peanut/Core/Core.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace pn
{

static void OpenGLDebugMessageCallback(
    GLenum source, GLenum type, GLuint /*id*/, GLenum severity, 
    GLsizei /*length*/, const GLchar* message, const void* /*userParam*/
)
{
    const char* _source;
    switch (source) {
        case GL_DEBUG_SOURCE_API:
            _source = "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            _source = "WINDOW SYSTEM";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            _source = "SHADER COMPILER";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            _source = "THIRD PARTY";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            _source = "APPLICATION";
            break;
        case GL_DEBUG_SOURCE_OTHER:
        default:
            _source = "UNKNOWN SOURCE";
            break;
    }

    const char* _type;
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            _type = "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            _type = "DEPRECATED BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            _type = "UDEFINED BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            _type = "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            _type = "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_OTHER:
        default:
            _type = "UNKNOWN TYPE";
            break;
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            PN_CORE_ASSERT(false, "[OpenGL] [{}] [{}]: {}", _source, _type, message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            PN_CORE_ERROR("[OpenGL] [{}] [{}]: {}", _source, _type, message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
        default:
            PN_CORE_WARN("[OpenGL] [{}] [{}]: {}", _source, _type, message);
            break;
    }
}

OpenGLGLFWRenderContext::OpenGLGLFWRenderContext()
{
    PN_CORE_INFO("Using OpenGL-GLFW Render Context");
}

void OpenGLGLFWRenderContext::PreWindowSetup()
{
    int major = 4, minor = 5;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined(PN_DEBUG)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    PN_CORE_INFO("Requesting OpenGL {}.{} with debug context", major, minor);
#else
    PN_CORE_INFO("Requesting OpenGL {}.{}", major, minor);
#endif
}

void OpenGLGLFWRenderContext::PostWindowSetup(Window& window)
{
    PN_CORE_INFO("==========================================");

    SetCurrentContext(window);
    InitializeGlad();
    SetupDebugContext();

    auto glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

    PN_CORE_INFO("\tUsing OpenGL {}", glVersion);
    PN_CORE_INFO("\tVendor: {}", vendor);
    PN_CORE_INFO("\tRenderer: {}", renderer);
    PN_CORE_INFO("==========================================");
}

void OpenGLGLFWRenderContext::SetupDebugContext()
{
    GLint context_flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
    if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        PN_CORE_INFO("\tUsing debug context");
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLDebugMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
    }
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