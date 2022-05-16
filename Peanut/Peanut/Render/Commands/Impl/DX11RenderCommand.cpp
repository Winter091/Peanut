#include "DX11RenderCommand.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Window/RenderContext/Impl/DX11GLFWRenderContext.hpp>

#include <d3d11.h>

namespace pn {

DX11RenderCommand::DX11RenderCommand()
    : m_clearColor(0.0f, 0.0f, 0.0f, 1.0f)
{
    PN_CORE_INFO("Using DirectX Renderer");
}

void DX11RenderCommand::SetClearColor(const glm::vec4& color)
{
    m_clearColor = color;
}

void DX11RenderCommand::Clear()
{
    auto& context = DX11GLFWRenderContext::GetCurrentContext();
    auto* deviceContext = context.GetDeviceContext();
    auto* renderTargetView = context.GetRenderTargetView();

	deviceContext->ClearRenderTargetView(renderTargetView, &m_clearColor[0]);
}

void DX11RenderCommand::SetViewport(int32_t leftX, int32_t bottomY, uint32_t width, uint32_t height)
{
    D3D11_VIEWPORT viewport;
	viewport.TopLeftX = static_cast<float>(leftX);
	viewport.TopLeftY = static_cast<float>(bottomY + height);
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

    auto& context = DX11GLFWRenderContext::GetCurrentContext();
    auto* deviceContext = context.GetDeviceContext();
	deviceContext->RSSetViewports(1, &viewport);
}

void DX11RenderCommand::DrawArrays(std::shared_ptr<VertexArray>& /*vertexArray*/, uint32_t /*count*/)
{
    PN_CORE_ASSERT(false, "DX11RenderCommand::DrawArrays() is not implemented");
}

void DX11RenderCommand::DrawIndexed(std::shared_ptr<VertexArray>& /*vertexArray*/, uint32_t /*count*/)
{
    PN_CORE_ASSERT(false, "DX11RenderCommand::DrawIndexed() is not implemented");
}

}