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

void DX11RenderCommand::DrawArrays(std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
{
    
}

void DX11RenderCommand::DrawIndexed(std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
{
    
}

}