#include "SandboxApp.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Render/Commands/RenderCommand.hpp>

#include <Windows.h>
#include <d3d11.h>

SandboxApp::SandboxApp(const pn::WindowSettings& settings)
    : pn::Application(settings)
{
    
}

void SandboxApp::OnEvent(const pn::Event& event)
{
    PN_CLIENT_DEBUG("Received event: {}", event.ToString());
}

void SandboxApp::OnUpdate()
{
    pn::RenderCommand::SetClearColor({0.2, 0.3, 0.4, 1.0});
    pn::RenderCommand::Clear();
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