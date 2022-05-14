#include "SandboxApp.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Render/RenderCommand.hpp>

SandboxApp::SandboxApp(const pn::WindowSettings& settings)
    : pn::Application(settings)
{
    float triangleVertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    auto buffer = pn::VertexBuffer::Create(sizeof(triangleVertices), triangleVertices);
    buffer->SetLayout({
        { pn::BufferLayoutElementType::Float, 3, "position" }
    });

    m_triangleVAO = pn::VertexArray::Create();
    m_triangleVAO->AddVertexBuffer(buffer);
}

void SandboxApp::OnEvent(const pn::Event& event)
{
    // PN_CLIENT_DEBUG(event.ToString());
}

void SandboxApp::OnUpdate()
{
    pn::RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
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