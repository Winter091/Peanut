#include "Dx11RenderCommand.hpp"
#include "DX11RenderCommand.hpp"

#include <Peanut/Core/Assert.hpp>
#include <Peanut/Application/Application.hpp>
#include <Window/RenderContextImpl/Dx11renderContext.hpp>
#include <Render/Buffers/Impl/Dx11VertexBuffer.hpp>
#include <Render/Buffers/Impl/Dx11IndexBuffer.hpp>
#include <Render/Buffers/Impl/Dx11EnumConversions.hpp>
#include <Render/Buffers/Impl/Dx11ConstantBuffer.hpp>
#include <Render/Framebuffers/Impl/Dx11Framebuffer.hpp>
#include <Render/Shaders/Impl/Dx11ShaderInputLayout.hpp>
#include <Render/Shaders/Impl/Dx11Shader.hpp>
#include <Render/Textures/Impl/Dx11Texture.hpp>
#include <Render/Textures/Impl/Dx11TextureSampler.hpp>

#include <vector>

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
        auto& context = Dx11RenderContext::GetCurrentContext();
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

        auto& context = Dx11RenderContext::GetCurrentContext();
        auto* deviceContext = context.GetDeviceContext();
        deviceContext->RSSetViewports(1, &viewport);
    }

    void Dx11RenderCommand::Draw(std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
    {
        if (count == 0) {
            count = vertexArray->GetVertexCount();
        }

        BindVertexArray(vertexArray);

        auto* deviceContext = Dx11RenderContext::GetCurrentContext().GetDeviceContext();
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        deviceContext->Draw(count, 0);
    }

    void Dx11RenderCommand::DrawIndexed(std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
    {
        if (count == 0) {
            count = vertexArray->GetIndexCount();
        }

        BindVertexArray(vertexArray);

        auto* deviceContext = Dx11RenderContext::GetCurrentContext().GetDeviceContext();
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        deviceContext->DrawIndexed(count, 0, 0);
    }

    void Dx11RenderCommand::DrawInstanced(std::shared_ptr<VertexArray>& vertexArray, uint32_t count, uint32_t instanceCount)
    {
        if (count == 0) {
            count = vertexArray->GetIndexCount();
        }

        if (instanceCount == 0) {
            instanceCount = vertexArray->GetInstanceCount();
        }

        BindVertexArray(vertexArray);
    
        auto* deviceContext = Dx11RenderContext::GetCurrentContext().GetDeviceContext();
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        deviceContext->DrawInstanced(count, instanceCount, 0, 0);
    }

    void Dx11RenderCommand::DrawIndexedInstanced(std::shared_ptr<VertexArray>& vertexArray, uint32_t count, uint32_t instanceCount)
    {
        if (count == 0) {
            count = vertexArray->GetIndexCount();
        }

        if (instanceCount == 0) {
            instanceCount = vertexArray->GetInstanceCount();
        }

        BindVertexArray(vertexArray);

        auto* deviceContext = Dx11RenderContext::GetCurrentContext().GetDeviceContext();
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        deviceContext->DrawIndexedInstanced(count, instanceCount, 0, 0, 0);
    }

    void Dx11RenderCommand::BindVertexBuffers(const std::shared_ptr<VertexBuffer>* vertexBuffers, size_t amount, uint32_t startSlot)
    {
        constexpr size_t maxBuffers = 16;
        static std::vector<ID3D11Buffer*> buffers(maxBuffers);
        static std::vector<uint32_t> strides(maxBuffers);
        static std::vector<uint32_t> offsets(maxBuffers, 0);

        PN_CORE_ASSERT(amount > 0 && amount <= maxBuffers, "Can't bind less than 1 or more than {} vertex buffers", maxBuffers);

        for (int i = 0; i < amount; i++) {
            buffers[i] = (static_cast<Dx11VertexBuffer&>(*vertexBuffers[i])).GetNativeObjectPtr();
            strides[i] = vertexBuffers[i]->GetLayout()->GetVertexSize();
        }

        auto* deviceContext = Dx11RenderContext::GetCurrentContext().GetDeviceContext();
        deviceContext->IASetVertexBuffers(startSlot, static_cast<uint32_t>(amount), &buffers[0], &strides[0], &offsets[0]);
    }

    void Dx11RenderCommand::BindVertexArray(const std::shared_ptr<VertexArray>& vertexArray)
    {
        auto vertexBuffers = vertexArray->GetVertexBuffers();
        BindVertexBuffers(vertexBuffers.data(), vertexBuffers.size(), 0);

        auto* deviceContext = Dx11RenderContext::GetCurrentContext().GetDeviceContext();
        deviceContext->IASetInputLayout(static_cast<Dx11ShaderInputLayout&>(*vertexArray->GetShaderInputLayout()).Get());

        auto indexBuffer = vertexArray->GetIndexBuffer();
        if (indexBuffer) {
            deviceContext->IASetIndexBuffer(
                static_cast<Dx11IndexBuffer&>(*indexBuffer).GetNativeObjectPtr(), 
                IndexBufferFormatToDx11Format(indexBuffer->GetDataFormat()),
                0);
        }
    }

    void Dx11RenderCommand::BindFramebuffer(const std::shared_ptr<Framebuffer>& framebuffer)
    {
        auto* deviceContext = Dx11RenderContext::GetCurrentContext().GetDeviceContext();
        
        if (framebuffer == nullptr) {
            // TODO: Get default framebuffer from current context
            PN_CORE_ASSERT(false, "Binding default framebuffer is not implemented");
            return;
        }

        auto& dx11Framebuffer = static_cast<Dx11Framebuffer&>(*framebuffer);
        auto numRednerTargetViews = dx11Framebuffer.GetNumRenderTargetViews();
        auto renderTargetViews = dx11Framebuffer.GetRenderTargetViewNativePointers();
        auto depthStencilView = dx11Framebuffer.GetDepthStencilViewNativePointer();
        deviceContext->OMSetRenderTargets(numRednerTargetViews, renderTargetViews, depthStencilView);
    }

    void Dx11RenderCommand::BindShader(const std::shared_ptr<Shader>& shader)
    {
        auto& dx11Shader = static_cast<Dx11Shader&>(*shader);
        auto* deviceContext = Dx11RenderContext::GetCurrentContext().GetDeviceContext();
        deviceContext->VSSetShader(dx11Shader.GetNativeVertexShaderObj(), nullptr, 0);
        deviceContext->PSSetShader(dx11Shader.GetNativeFragmentShaderObj(), nullptr, 0);
    }

    void Dx11RenderCommand::BindConstantBuffers(const std::shared_ptr<ConstantBuffer>* constantBuffers, size_t amount, uint32_t startSlot)
    {
        constexpr size_t maxBuffers = 16;
        static std::vector<ID3D11Buffer*> buffers(maxBuffers);

        PN_CORE_ASSERT(amount > 0 && amount <= maxBuffers, "Can't bind less than 1 or more than {} vertex buffers", maxBuffers);

        for (int i = 0; i < amount; i++) {
            buffers[i] = (static_cast<Dx11ConstantBuffer&>(*constantBuffers[i])).GetNativeObjectPtr();
        }
    
        auto* deviceContext = Dx11RenderContext::GetCurrentContext().GetDeviceContext();
        deviceContext->VSSetConstantBuffers(0, static_cast<uint32_t>(amount), &buffers[0]);
        deviceContext->PSSetConstantBuffers(0, static_cast<uint32_t>(amount), &buffers[0]);
    }

    void Dx11RenderCommand::BindTextures(const std::shared_ptr<Texture>* textures, size_t amount, uint32_t startSlot)
    {
        constexpr size_t maxTextures = 16;
        static std::vector<ID3D11ShaderResourceView*> views(maxTextures);
        static std::vector<ID3D11SamplerState*> samplers(maxTextures);

        PN_CORE_ASSERT(amount > 0 && amount <= maxTextures, "Cannot bind less than 1 or more than {} textures", maxTextures);

        for (int i = 0; i < amount; i++) {
            views[i] = dynamic_cast<Dx11Texture&>(*textures[i]).GetNativeViewPtr();
            samplers[i] = dynamic_cast<Dx11TextureSampler&>(*textures[i]->GetSampler()).GetNativeObjectPtr();
        }

        auto* deviceContext = Dx11RenderContext::GetCurrentContext().GetDeviceContext();
        deviceContext->PSSetShaderResources(startSlot, static_cast<uint32_t>(amount), &views[0]);
        deviceContext->PSSetSamplers(startSlot, static_cast<uint32_t>(amount), &samplers[0]);
    }

}
