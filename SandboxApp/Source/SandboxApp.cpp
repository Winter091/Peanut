#include "SandboxApp.hpp"

#include <Peanut/Peanut.hpp>

SandboxApp::SandboxApp(const pn::WindowSettings& settings)
    : pn::Application(settings)
{
    float vertices[] = {
        -0.5f, -0.5f,  0.0f,
        -0.5f,  0.5f,  0.0f,
         0.5f,  0.5f,  0.0f,
         0.5f, -0.5f,  0.0f
    };

    uint8_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    auto vertexBuffer = pn::VertexBuffer::Create(sizeof(vertices), vertices);
    auto indexBuffer = pn::IndexBuffer::Create(pn::IndexBufferDataFormat::Uint8, sizeof(indices), indices);

    auto layout = pn::BufferLayout::Create({
        { pn::BufferLayoutElementType::Float, 3, "position" },
    });
    vertexBuffer->SetLayout(layout);

    m_rectangleVAO = pn::VertexArray::Create();
    m_rectangleVAO->SetVertexBuffer(vertexBuffer);
    m_rectangleVAO->SetIndexBuffer(indexBuffer);

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

    m_shader->Bind();
    pn::RenderCommand::DrawIndexed(m_rectangleVAO);
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