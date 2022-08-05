#include "SandboxApp.hpp"

SandboxApp::SandboxApp(const pn::WindowSettings& settings)
    : pn::Application(settings)
{
    PN_PROFILE_FUNCTION();

    float positions[] = {
        -0.5f, -0.5f,  0.0f,
        -0.5f,  0.5f,  0.0f,
         0.5f,  0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
    };

    float colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
    };

    uint16_t indices[] = { 0, 1, 2, 2, 3, 0 };

    auto posVB = pn::VertexBuffer::Create(pn::BufferMapAccess::NoAccess, sizeof(positions), positions);
    auto posVBLayout = pn::BufferLayout::Create({
        { 0, pn::BufferLayoutElementType::Float, 3, "position" },
    });
    posVB->SetLayout(posVBLayout);

    auto colorVB = pn::VertexBuffer::Create(pn::BufferMapAccess::NoAccess, sizeof(colors), colors);
    auto colorVBLayout = pn::BufferLayout::Create({
        { 1, pn::BufferLayoutElementType::Float, 3, "color" },
    });
    colorVB->SetLayout(colorVBLayout);

    auto indexBuffer = pn::IndexBuffer::Create(pn::IndexBufferDataFormat::Uint16, pn::BufferMapAccess::NoAccess, sizeof(indices), indices);

    m_vertexArray = pn::VertexArray::Create();
    m_vertexArray->AddVertexBuffer(posVB, pn::BufferAttributeUsage::PerVertex);
    m_vertexArray->AddVertexBuffer(colorVB, pn::BufferAttributeUsage::PerVertex);
    m_vertexArray->SetIndexBuffer(indexBuffer);

    m_shader = pn::Shader::Create(pn::ShaderPaths()
        .SetVertexPath("E:/Projects/C++/Peanut/Peanut/Assets/Shaders/test.vert")
        .SetFragmentPath("E:/Projects/C++/Peanut/Peanut/Assets/Shaders/test.frag"),
        "Test Shader");
}

void SandboxApp::OnEvent(pn::Event& event)
{
    PN_PROFILE_FUNCTION();

    // PN_CORE_TRACE("Event: {}", event.ToString());
}

void SandboxApp::OnUpdate()
{
    PN_PROFILE_FUNCTION();

    pn::RenderCommand::SetClearColor({ 0.5f, 0.3f, 0.2f, 1.0f });
    pn::RenderCommand::Clear();

    m_shader->Bind();
    pn::RenderCommand::DrawIndexed(m_vertexArray);
}

pn::Application* pn::Application::CreateApplication(const CommandLineArgs& args)
{
    PN_PROFILE_FUNCTION();

    PN_CLIENT_DEBUG("Launch args:");
    for (std::size_t i = 0; i < args.GetSize(); i++) {
        PN_CLIENT_DEBUG("   Arg {}: {}", i, args[i]);
    }

    return new SandboxApp(WindowSettings()
        .SetTitle("Sandbox Application")
        .SetWidth(1280).SetHeight(720)
        .UseVsync(true));
}