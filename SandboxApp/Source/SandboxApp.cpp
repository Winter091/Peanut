#include "SandboxApp.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

SandboxApp::SandboxApp(const pn::WindowSettings& settings)
    : pn::Application(settings)
{
    m_texture = pn::Texture2D::Create("Peanut/Assets/Textures/container.jpg", pn::Texture2DSettings()
        .UseFormat(pn::TextureFormat::RGB)
        .UseMipmapFiltering(pn::TextureMipmapFilter::LinearMipmapLinear, pn::TextureFilter::Linear));

    m_camera = std::make_shared<pn::OrthoCamera>(pn::OrthoCameraSettings()
        .SetZoom(1.0f)
        .SetAspectRatio(GetWindow().GetAspectRatio())
        .SetPosition({ 0.0f, 0.0f, 1.0f })
        .SetDirection({ 0.0f, 0.0f, -1.0f })
        .SetBoundaries(pn::OrthoCameraBoundaries()
            .SetCenter({ 0.0f, 0.0f })
            .SetWidth(150.0f).SetHeight(150.0f)));
}

void SandboxApp::OnEvent(pn::Event& event)
{
    event.Dispatch<pn::EventType::WindowSizeChanged>([this](pn::Event&) {
        m_camera->SetAspectRatio(GetWindow().GetAspectRatio());
        return true;
    });
}

void SandboxApp::OnUpdate()
{
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
            float step = 1.25f;
            for (int i = -50; i <= 50; i++) {
                for (int j = -50; j <= 50; j++) {
                    float x = step * static_cast<float>(i);
                    float y = step * static_cast<float>(j);

                    pn::Rectangle rect;
                    rect.SetPos({x, y});
                    rect.SetSize({1.0f, 1.0f});
                    rect.SetTexture(m_texture);

                    pn::Renderer2D::DrawShape(rect);
                }
            }
        }
        pn::Renderer2D::EndScene();
    }
}

pn::Application* pn::Application::CreateApplication(const CommandLineArgs& args)
{
    PN_CLIENT_DEBUG("Launch args:");
    for (std::size_t i = 0; i < args.GetSize(); i++) {
        PN_CLIENT_DEBUG("   Arg {}: {}", i, args[i]);
    }

    return new SandboxApp(WindowSettings()
        .SetTitle("Sandbox Application")
        .SetWidth(1280).SetHeight(720)
        .UseVsync(true));
}