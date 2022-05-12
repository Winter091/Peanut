#include "SandboxApp.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Peanut/Core/Assert.hpp>
#include <Peanut/Render/Render.hpp>

SandboxApp::SandboxApp(const pn::WindowSettings& settings)
    : pn::Application(settings)
{
    auto glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

    PN_CLIENT_INFO("==========================================");
    PN_CLIENT_INFO("\tUsing OpenGL {}", glVersion);
    PN_CLIENT_INFO("\tVendor: {}", vendor);
    PN_CLIENT_INFO("\tRenderer: {}", renderer);
    PN_CLIENT_INFO("==========================================");
}

void SandboxApp::OnEvent(const pn::Event& event)
{
    PN_CLIENT_DEBUG(event.ToString());
}

void SandboxApp::OnUpdate()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    pn::Render::ClearColor();
}

pn::Application* pn::Application::CreateApplication(const CommandLineArgs& args)
{
    PN_CLIENT_DEBUG("Launch args:");
    for (std::size_t i = 0; i < args.GetSize(); i++) {
        PN_CLIENT_DEBUG("Arg {}: {}", i, args[i]);
    }

    pn::WindowSettings settings;
    settings.title = "Sandbox Application";
    
    return new SandboxApp(settings);
}