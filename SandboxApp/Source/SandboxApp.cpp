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

    auto posVB = pn::VertexBuffer::Create(
        pn::BufferMapAccess::NoAccess, 
        sizeof(positions), 
        pn::BufferLayout::Create(
            pn::BufferLayoutAttributeUsage::PerVertex, {
            { 0, "position", pn::BufferLayoutElementType::Float, 3, }
        }), 
        positions
    );

    auto colorVB = pn::VertexBuffer::Create(
        pn::BufferMapAccess::NoAccess, 
        sizeof(colors), 
        pn::BufferLayout::Create(
            pn::BufferLayoutAttributeUsage::PerVertex, {
            { 1, "color", pn::BufferLayoutElementType::Float, 3 }
        }), 
        colors
    );

    auto indexBuffer = pn::IndexBuffer::Create(pn::IndexBufferDataFormat::Uint16, pn::BufferMapAccess::NoAccess, sizeof(indices), indices);
    
    m_shader = pn::Shader::Create(pn::ShaderPaths()
        .SetVertexPath("E:/Projects/C++/Peanut/Peanut/Assets/Shaders/test.vert")
        .SetFragmentPath("E:/Projects/C++/Peanut/Peanut/Assets/Shaders/test.frag"),
        "Test Shader");

    pn::VertexArrayDescription desc;
    desc.VertexBuffers = { posVB, colorVB };
    desc.IndexBuffer = indexBuffer;
    desc.Shader = m_shader;

    m_vertexArray = pn::VertexArray::Create(desc);
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