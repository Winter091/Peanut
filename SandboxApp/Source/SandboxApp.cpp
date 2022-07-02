#include "SandboxApp.hpp"

SandboxApp::SandboxApp(const pn::WindowSettings& settings)
    : pn::Application(settings)
{
    float vertices[] = {
        -0.05f, -0.05f,  0.0f,    1.0f, 0.0f, 0.0f,
        -0.05f,  0.05f,  0.0f,    0.0f, 1.0f, 0.0f,
         0.05f,  0.05f,  0.0f,    0.0f, 0.0f, 1.0f,
         0.05f, -0.05f,  0.0f,    1.0f, 0.0f, 1.0f,
    };

    uint8_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    std::vector<float> offsets;
    float step = 1.8f / 9.0f;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            float x = -0.9f + step * i;
            float y = -0.9f + step * j;
            offsets.push_back(x);
            offsets.push_back(y);
            offsets.push_back(0);
        }
    }

    m_rectangleVAO = pn::VertexArray::Create();

    auto vertexBuffer = pn::VertexBuffer::Create(sizeof(vertices), vertices);
    auto layout = pn::BufferLayout::Create({
        { 0, pn::BufferLayoutElementType::Float, 3, "position" },
        { 1, pn::BufferLayoutElementType::Float, 3, "color" },
    });
    vertexBuffer->SetLayout(layout);
    m_rectangleVAO->AddVertexBuffer(vertexBuffer, pn::BufferDataUsage::PerVertex);

    auto instanceBuffer = pn::VertexBuffer::Create(sizeof(float) * offsets.size(), &offsets[0]);
    auto instanceLayout = pn::BufferLayout::Create({
        { 2, pn::BufferLayoutElementType::Float, 3, "offset" },
    });
    instanceBuffer->SetLayout(instanceLayout);
    m_rectangleVAO->AddVertexBuffer(instanceBuffer, pn::BufferDataUsage::PerInstance);

    auto indexBuffer = pn::IndexBuffer::Create(pn::IndexBufferDataFormat::Uint8, sizeof(indices), indices);
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

    m_rectangleVAO->Bind();
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr, 100);
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