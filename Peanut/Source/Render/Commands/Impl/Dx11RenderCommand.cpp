#include "DX11RenderCommand.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Application/Application.hpp>
#include <Window/RenderContextImpl/Dx11GLFWRenderContext.hpp>

#include <d3d11.h>

namespace pn {

Dx11RenderCommand::Dx11RenderCommand()
    : m_clearColor(0.0f, 0.0f, 0.0f, 1.0f)
{
}

void Dx11RenderCommand::SetClearColor(const glm::vec4& color)
{
    m_clearColor = color;
}

void Dx11RenderCommand::Clear()
{
    auto& context = Dx11GLFWRenderContext::GetCurrentContext();
    auto* deviceContext = context.GetDeviceContext();

    auto* renderTargetView = context.GetRenderTargetView();
    deviceContext->ClearRenderTargetView(renderTargetView, &m_clearColor[0]);

    auto* depthStencilView = context.GetDepthStencilView();
    deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 1);
}

void Dx11RenderCommand::SetViewport(int32_t leftX, int32_t bottomY, uint32_t width, uint32_t height)
{
    Application& app = Application::GetInstance();

    D3D11_VIEWPORT viewport;
    viewport.TopLeftX = static_cast<float>(leftX);
    viewport.TopLeftY = static_cast<float>(app.GetWindow().GetHeight() - bottomY - height);
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    auto& context = Dx11GLFWRenderContext::GetCurrentContext();
    auto* deviceContext = context.GetDeviceContext();
    deviceContext->RSSetViewports(1, &viewport);
}

void Dx11RenderCommand::DrawArrays(std::shared_ptr<PipelineState>& pipelineState, uint32_t count)
{
    auto* deviceContext = Dx11GLFWRenderContext::GetCurrentContext().GetDeviceContext();

    if (count == 0) {
        count = pipelineState->GetVertexCount();
    }

    pipelineState->Bind();
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceContext->Draw(count, 0);
}

void Dx11RenderCommand::DrawIndexed(std::shared_ptr<PipelineState>& pipelineState, uint32_t count)
{
    auto* deviceContext = Dx11GLFWRenderContext::GetCurrentContext().GetDeviceContext();

    if (count == 0) {
        count = pipelineState->GetIndexCount();
    }

    pipelineState->Bind();
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceContext->DrawIndexed(count, 0, 0);
}

void Dx11RenderCommand::DrawArraysInstanced(
    std::shared_ptr<PipelineState>& pipelineState, uint32_t count, uint32_t instanceCount)
{

}

void Dx11RenderCommand::DrawIndexedInstanced(
    std::shared_ptr<PipelineState>& pipelineState, uint32_t count, uint32_t instanceCount)
{

}

}