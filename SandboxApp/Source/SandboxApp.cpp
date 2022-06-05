#include "SandboxApp.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Render/Commands/RenderCommand.hpp>

#include <Windows.h>
#include <d3d11.h>
#include <Peanut/Window/RenderContext/Impl/DX11GLFWRenderContext.hpp>

SandboxApp::SandboxApp(const pn::WindowSettings& settings)
    : pn::Application(settings)
{
    const float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    auto vertexBuffer = pn::VertexBuffer::Create(sizeof(vertices), vertices);

    ID3D11Device* device = pn::DX11GLFWRenderContext::GetCurrentContext().GetDevice();
    ID3D11Debug* debug = nullptr;
    device->QueryInterface(IID_ID3D11Debug, (void**)&debug);
    if (!debug) {
        PN_CLIENT_ASSERT(false, "No debug interface!");
    }
    HRESULT res = debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
    if (FAILED(res)) {
        PN_CLIENT_ERROR("Can't debug! Error = {}", res);
    }

    // auto vertexArray = pn::VertexArray::Create();
    // vertexArray->SetVertexBuffer(vertexBuffer);
}

void SandboxApp::OnEvent(const pn::Event& event)
{
    PN_CLIENT_DEBUG("Received event: {}", event.ToString());
}

void SandboxApp::OnUpdate()
{
    ID3D11Device* device = pn::DX11GLFWRenderContext::GetCurrentContext().GetDevice();
    device->AddRef();
    pn::RenderCommand::SetClearColor({0.2, 0.3, 0.4, 1.0});
    pn::RenderCommand::Clear();
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