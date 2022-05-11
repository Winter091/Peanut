#include "SandboxApp.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Core/Assert.hpp>

SandboxApp::SandboxApp()
    : pn::Application(800, 600, "Sandbox Application")
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        PN_CLIENT_ASSERT(false, "Failed to initialize GLAD!\n");
    }

    auto glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

    PN_CORE_INFO("==========================================");
    PN_CORE_INFO("\tUsing OpenGL {}", glVersion);
    PN_CORE_INFO("\tVendor: {}", vendor);
    PN_CORE_INFO("\tRenderer: {}", renderer);
    PN_CORE_INFO("==========================================");
}

void SandboxApp::OnEvent(const Event& event)
{
    PN_CLIENT_DEBUG("Received event: {}", event.GetStringType());
}

void SandboxApp::OnUpdate()
{
    // PN_CLIENT_DEBUG("OnUpdate");

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

pn::Application* pn::Application::CreateApplication()
{
    return new SandboxApp();
}