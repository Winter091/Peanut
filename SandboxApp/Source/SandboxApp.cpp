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
    auto constantBuffer = pn::ConstantBuffer::Create(pn::BufferMapAccess::WriteDiscard, sizeof(color), color);

    auto shader = pn::Shader::Create(pn::ShaderPaths()
        .SetVertexPath(pn::StoragePath::GetAssetsPath() + "/Shaders/test.vert")
        .SetFragmentPath(pn::StoragePath::GetAssetsPath() + "/Shaders/test.frag"),
        "Test Shader");

    pn::PipelineStateDescription desc;
    desc.VertexBuffers = { posVB, colorVB };
    desc._IndexBuffer = indexBuffer;
    desc.ConstantBuffers = { constantBuffer };
    desc._Shader = shader;

    m_pipelineState = pn::PipelineState::Create(desc);

    m_camera = std::make_shared<pn::OrthoCamera>(pn::OrthoCameraSettings()
        .SetZoom(1.0f)
        .SetAspectRatio(GetWindow().GetAspectRatio())
        .SetPosition({ 0.0f, 0.0f, 1.0f })
        .SetDirection({ 0.0f, 0.0f, -1.0f })
        .SetBoundaries(pn::OrthoCameraBoundaries()
            .SetCenter({ 0.0f, 0.0f })
            .SetWidth(150.0f).SetHeight(150.0f)));

    float step = 1.25f;
    for (int i = -500; i <= 500; i++) {
        for (int j = -500; j <= 500; j++) {
            float x = step * static_cast<float>(i);
            float y = step * static_cast<float>(j);
            pn::Rectangle rect;
            rect.SetPosition({ x, y });
            m_rectangles.push_back(rect);
        }
    }
}

void SandboxApp::OnEvent(pn::Event& event)
{
    (void)event;    
    // PN_CORE_TRACE("Event: {}", event.ToString());
}

void SandboxApp::OnUpdate()
{
    PN_PROFILE_FUNCTION();

    {
        pn::Window& window = GetWindow();
        glm::vec3 pos = m_camera->GetPosition();
        float zoom = m_camera->GetZoom();
        float delta = 2.0f;
        float zoomDelta = 0.01f;

        if (window.IsKeyPressed(pn::KeyCode::W)) { pos.y += delta * zoom; }
        if (window.IsKeyPressed(pn::KeyCode::S)) { pos.y -= delta * zoom; }
        if (window.IsKeyPressed(pn::KeyCode::A)) { pos.x -= delta * zoom; }
        if (window.IsKeyPressed(pn::KeyCode::D)) { pos.x += delta * zoom; }
        if (window.IsKeyPressed(pn::KeyCode::PageUp)) { zoom -= zoomDelta; }
        if (window.IsKeyPressed(pn::KeyCode::PageDown)) { zoom += zoomDelta; }

        m_camera->SetPosition(pos);
        m_camera->SetZoom(zoom);
    }

    {
        pn::RenderCommand::SetClearColor({ 0.05f, 0.05f, 0.05f, 1.0f });
        pn::RenderCommand::Clear();

        pn::Renderer2D::BeginScene(*m_camera);
        {
            for (const auto& rect : m_rectangles) {
                pn::Renderer2D::DrawRectangle(rect);
            }
        }
        pn::Renderer2D::EndScene();
    }

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
        .UseVsync(false));
}