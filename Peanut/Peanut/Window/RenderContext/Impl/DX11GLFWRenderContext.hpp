#ifndef __DX11GLFWRENDERCONTEXT_H__
#define __DX11GLFWRENDERCONTEXT_H__

#include <Peanut/Window/RenderContext/RenderContext.hpp>
#include <Peanut/Core/Log.hpp>

#include <d3d11.h>

namespace pn {

class DX11GLFWRenderContext final : public RenderContext
{
public:
    DX11GLFWRenderContext();
    DX11GLFWRenderContext(const DX11GLFWRenderContext&) = delete;
    DX11GLFWRenderContext& operator=(const DX11GLFWRenderContext&) = delete;
    ~DX11GLFWRenderContext() override;

    void PreWindowSetup() override;
    void PostWindowSetup(Window& window) override;
    void SetCurrentContext(Window& window) override;
    void SetSwapInterval(int interval) override;
    void SwapBuffers(Window& window) override;
    void OnWindowResize(Window& window) override;

    ID3D11Device* GetDevice() { return m_device; }
    ID3D11DeviceContext* GetDeviceContext() { return m_deviceContext; }

    ID3D11RenderTargetView* GetRenderTargetView() { return m_renderTargetView; }
    ID3D11DepthStencilView* GetDepthStencilView() { return m_depthStencilView; }

    static DX11GLFWRenderContext& GetCurrentContext();

private:
    void CreateDeviceAndSwapChain(HWND window, int windowWidth, int windowHeight, bool createDebugContext);

    void InitDepthBuffer(int windowWidth, int windowHeight);
    void RecreateDepthBuffer(int windowWidth, int windowHeight);

private:
    static DX11GLFWRenderContext* s_currentContext;

    ID3D11Device* m_device = nullptr;
    ID3D11RenderTargetView* m_renderTargetView = nullptr;
    ID3D11DeviceContext* m_deviceContext = nullptr;
    IDXGISwapChain* m_swapChain = nullptr;

    ID3D11Texture2D* m_depthStencilBufferTexture = nullptr;
    ID3D11DepthStencilState* m_depthStencilState = nullptr;
    ID3D11DepthStencilView* m_depthStencilView = nullptr;

    int m_swapInterval;
};

}

#endif