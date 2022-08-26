#include "SandboxApp.hpp"

SandboxApp::SandboxApp(const pn::WindowSettings& settings)
    : pn::Application(settings)
{
    PN_PROFILE_FUNCTION();

    m_camera = std::make_shared<pn::OrthoCamera>(pn::OrthoCameraSettings()
        .SetZoom(1.0f)
        .SetAspectRatio(GetWindow().GetAspectRatio())
        .SetPosition({ 0.0f, 0.0f, 1.0f })
        .SetDirection({ 0.0f, 0.0f, -1.0f })
        .SetBoundaries(pn::OrthoCameraBoundaries()
            .SetCenter({ 0.0f, 0.0f })
            .SetWidth(15.0f).SetHeight(15.0f)));

    auto sampler = pn::TextureSampler::Create(pn::TextureSamplerSettings()
        .SetFilter(pn::TextureFilter::MinAnisotropicMagAnisotropicMipAnisotropic)
        .SetWrap(pn::TextureWrap::Border)
        .SetBorderColor({ 0.0f, 0.0f, 0.0f, 1.0f })
        .SetMaxAnisotropy(16));

    int x = 512, y = 512;
    int levels = 10;

    m_texture = pn::Texture2D::Create(
        nullptr,
        pn::Texture2DSettings()
            .SetSampler(sampler)
            .UseFormat(pn::TextureFormat::RGBA)
            .SetSize({ x, y })
            .SetNumLevels(levels)
            .DoGenerateMipmaps(false));

    for (int i = 0; i < levels; i++, x /= 2, y /= 2) {
        std::vector<uint8_t> pixels(x * y * 4);

        for (uint32_t j = 0; j < pixels.size(); j += 4) {
            pixels[j + 0] = static_cast<uint32_t>((1234567 * (i + 1)) % 256);
            pixels[j + 1] = static_cast<uint32_t>((2345678 * (i + 1)) % 256);
            pixels[j + 2] = static_cast<uint32_t>((3456789 * (i + 1)) % 256);
            pixels[j + 3] = 255;
        }

        m_texture->SetLevelData(pixels.data(), i);
    }
    
    float step = 1.25f;
    for (int i = -5; i <= 5; i++) {
        for (int j = -5; j <= 5; j++) {
            float x = step * static_cast<float>(i);
            float y = step * static_cast<float>(j);
            pn::Rectangle rect;
            rect.SetPosition({ x, y });
            rect.SetColor({ 146, 226, 253, 255 });
            rect.SetTexture(m_texture);
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
        float delta = 0.15f;
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
        .UseVsync(true));
}