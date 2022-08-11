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

    uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

    auto posVB = pn::VertexBuffer::Create(
        pn::BufferMapAccess::NoAccess, 
        sizeof(positions), 
        pn::BufferLayout::Create(
            pn::BufferLayoutAttributeUsage::PerVertex, {
            { 0, "Position", pn::BufferLayoutElementType::Float, 3, }
        }), 
        positions
    );

    auto colorVB = pn::VertexBuffer::Create(
        pn::BufferMapAccess::NoAccess, 
        sizeof(colors), 
        pn::BufferLayout::Create(
            pn::BufferLayoutAttributeUsage::PerVertex, {
            { 1, "Color", pn::BufferLayoutElementType::Float, 3, }
        }), 
        colors
    );

    auto indexBuffer = pn::IndexBuffer::Create(pn::IndexBufferDataFormat::Uint32, pn::BufferMapAccess::NoAccess, sizeof(indices), indices);
    
    float color[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
    m_constantBuffer = pn::ConstantBuffer::Create(pn::BufferMapAccess::WriteDiscard, sizeof(color), color);

    m_shader = pn::Shader::Create(pn::ShaderPaths()
        .SetVertexPath(pn::StoragePath::GetAssetsPath() + "/Shaders/test.vert")
        .SetFragmentPath(pn::StoragePath::GetAssetsPath() + "/Shaders/test.frag"),
        "Test Shader");

    pn::PipelineStateDescription desc;
    desc.VertexBuffers = { posVB, colorVB };
    desc.IndexBuffer = indexBuffer;
    desc.ConstantBuffers = { m_constantBuffer };
    desc.Shader = m_shader;

    m_pipelineState = pn::PipelineState::Create(desc);
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

    float* bufferData = static_cast<float*>(m_constantBuffer->Map());
    bufferData[0] = (rand() % 256) / 255.0f;
    bufferData[1] = (rand() % 256) / 255.0f;
    bufferData[2] = (rand() % 256) / 255.0f;
    bufferData[3] = 1.0f;
    m_constantBuffer->Unmap();

    pn::RenderCommand::DrawIndexed(m_pipelineState);
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