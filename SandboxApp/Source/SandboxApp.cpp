#include "SandboxApp.hpp"

#include <glm/gtc/matrix_transform.hpp>

SandboxApp::SandboxApp(const pn::WindowSettings& settings)
    : pn::Application(settings)
{
    float vertices[] = {
        -0.5f, -0.5f,  0.0f,    0.0f, 0.0f,
        -0.5f,  0.5f,  0.0f,    0.0f, 1.0f,
         0.5f,  0.5f,  0.0f,    1.0f, 1.0f,
         0.5f, -0.5f,  0.0f,    1.0f, 0.0f,
    };

    uint8_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    std::vector<float> offsets;
    float step = 1.5f;
    for (int i = -100; i <= 100; i++) {
        for (int j = -100; j <= 100; j++) {
            float x = 0.0f + step * static_cast<float>(i);
            float y = 0.0f + step * static_cast<float>(j);
            offsets.push_back(x);
            offsets.push_back(y);
        }
    }

    m_rectangleVAO = pn::VertexArray::Create();

    auto vertexBuffer = pn::VertexBuffer::Create(sizeof(vertices), vertices);
    auto layout = pn::BufferLayout::Create({
        { 0, pn::BufferLayoutElementType::Float, 3, "position" },
        { 1, pn::BufferLayoutElementType::Float, 2, "tex_coord" },
    });
    vertexBuffer->SetLayout(layout);
    m_rectangleVAO->AddVertexBuffer(vertexBuffer, pn::BufferDataUsage::PerVertex);

    auto instanceBuffer = pn::VertexBuffer::Create(
        static_cast<uint32_t>(sizeof(offsets[0]) * offsets.size()), &offsets[0]);
    auto instanceLayout = pn::BufferLayout::Create({
        { 2, pn::BufferLayoutElementType::Float, 2, "offset" },
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

    m_texture = pn::Texture2D::Create("Peanut/Assets/Textures/container.jpg", pn::Texture2DSettings()
        .UseFormat(pn::TextureFormat::RGB)
        .UseMipmapping(true)
        .UseMipmapFiltering(pn::TextureMipmapFilter::LinearMipmapLinear, pn::TextureFilter::Linear)
        .UseWrapping(pn::TextureWrap::Repeat, pn::TextureWrap::Repeat)
    );

    m_camera = std::make_shared<pn::OrthoCamera>(pn::OrthoCameraSettings()
        .SetZoom(0.5f)
        .SetPosition({ 1.0f, 1.0f, 1.0f })
        .LookAt({ 0.0f, 0.0f, 0.0f })
        .SetBoundaries(pn::OrthoCameraBoundaries()
            .SetCenter({ 0.0f, 0.0f })
            .SetWidth(1.0f)
            .SetHeight(1.0f)));
}

void SandboxApp::OnEvent(pn::Event& event)
{
    (void)event;
}

void SandboxApp::OnUpdate()
{
    m_camera->SetAspectRatio((float)GetWindow().GetWidth() / (float)GetWindow().GetHeight());

    glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));
    glm::mat4 mvpMatrix = m_camera->GetViewProjectionMatrix() * modelMatrix;

    pn::RenderCommand::SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
    pn::RenderCommand::Clear();

    m_shader->Bind();
    m_shader->SetMat4("u_viewProjMatrix", mvpMatrix);
    m_texture->BindToSlot(0);
    
    pn::RenderCommand::DrawIndexedInstanced(m_rectangleVAO);
}

pn::Application* pn::Application::CreateApplication(const CommandLineArgs& args)
{
    PN_CLIENT_DEBUG("Launch args:");
    for (std::size_t i = 0; i < args.GetSize(); i++) {
        PN_CLIENT_DEBUG("Arg {}: {}", i, args[i]);
    }

    pn::WindowSettings settings;
    settings.width = 1280;
    settings.height = 720;
    settings.title = "Sandbox Application";

    return new SandboxApp(settings);
}