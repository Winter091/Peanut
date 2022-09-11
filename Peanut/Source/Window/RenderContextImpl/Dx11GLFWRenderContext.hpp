#pragma once

#include <Peanut/Window/RenderContext.hpp>
#include <Peanut/Core/Log.hpp>
#include "Dx11RenderContext.hpp"
#include <Render/Framebuffers/Impl/Dx11Framebuffer.hpp>

#include <d3d11.h>

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

namespace pn {

    class Dx11GLFWRenderContext final : public Dx11RenderContext
    {
    public:
        Dx11GLFWRenderContext();
        Dx11GLFWRenderContext(const Dx11GLFWRenderContext&) = delete;
        Dx11GLFWRenderContext& operator=(const Dx11GLFWRenderContext&) = delete;
        ~Dx11GLFWRenderContext() override;

        void PreWindowSetup() override;
        void PostWindowSetup(Window& window) override;
        void SetCurrentContext(Window& window) override;
        void SetSwapInterval(int interval) override;
        void SwapBuffers(Window& window) override;
        void OnWindowResize(const WindowSizeSettings& settings) override;

        void BindFramebuffer(Dx11Framebuffer* framebuffer);

    private:
        void CreateDeviceAndSwapChain(const Window& window, bool createDebugContext);

        void InitDepthBuffer(int windowWidth, int windowHeight);
        void RecreateDepthTexture(int windowWidth, int windowHeight);

    private:
        ComPtr<ID3D11Device> m_device = nullptr;
        ComPtr<ID3D11RenderTargetView> m_renderTargetView = nullptr;
        ComPtr<ID3D11DeviceContext> m_deviceContext = nullptr;
        ComPtr<IDXGISwapChain> m_swapChain = nullptr;

        ComPtr<ID3D11Texture2D> m_depthStencilBufferTexture = nullptr;
        ComPtr<ID3D11DepthStencilState> m_depthStencilState = nullptr;
        ComPtr<ID3D11DepthStencilView> m_depthStencilView = nullptr;

        int m_swapInterval;
    };

}
