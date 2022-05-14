#include "SandboxApp.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Render/RenderCommand.hpp>

SandboxApp::SandboxApp(const pn::WindowSettings& settings)
    : pn::Application(settings)
{
    float triangleVertives[] = {
        -0.5f, -0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
         0.0f,  0.5f,  0.0f
    };  

    auto buffer = pn::VertexBuffer::Create(sizeof(triangleVertives), triangleVertives);
    buffer->SetLayout({
        { pn::BufferLayoutElementType::Float, 3, "position" },
    });

    m_triangleVAO = pn::VertexArray::Create();
    m_triangleVAO->AddVertexBuffer(buffer);

    pn::ShaderPaths paths(
        "Peanut/Assets/Shaders/test.vert", 
        "Peanut/Assets/Shaders/test.frag"
    );
    m_shader = pn::Shader::Create(paths, "Test Shader");
}

void SandboxApp::OnEvent(const pn::Event& event)
{
    PN_CLIENT_DEBUG(event.ToString());
}

void SandboxApp::OnUpdate()
{
    pn::RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
    pn::RenderCommand::Clear();

    m_triangleVAO->Bind();
    m_shader->Bind();
    pn::RenderCommand::DrawArrays(0, 3);
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