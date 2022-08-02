#pragma once

#include <Peanut/Window/RenderContext.hpp>
#include <Peanut/Core/Log.hpp>

#include <d3d11.h>

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

namespace pn {



class Dx11GLFWRenderContext final : public RenderContext
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
    void OnWindowResize(Window& window) override;

    ID3D11Device* GetDevice() { return m_device.Get(); }
    ID3D11DeviceContext* GetDeviceContext() { return m_deviceContext.Get(); }

    ID3D11RenderTargetView* GetRenderTargetView() { return m_renderTargetView.Get(); }
    ID3D11DepthStencilView* GetDepthStencilView() { return m_depthStencilView.Get(); }

    static Dx11GLFWRenderContext& GetCurrentContext();

private:
    void CreateDeviceAndSwapChain(HWND window, int windowWidth, int windowHeight, bool createDebugContext);

    void InitDepthBuffer(int windowWidth, int windowHeight);
    void RecreateDepthTexture(int windowWidth, int windowHeight);

private:
    static Dx11GLFWRenderContext* s_currentContext;

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