#ifndef __DX11GLFWRENDERCONTEXT_H__
#define __DX11GLFWRENDERCONTEXT_H__

#include <Peanut/Window/RenderContext/RenderContext.hpp>
#include <Peanut/Core/Log.hpp>

// I don't want to include DX11 headers here
struct ID3D11Device;
struct ID3D11RenderTargetView;
struct ID3D11DeviceContext;
struct IDXGISwapChain;

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
    ID3D11RenderTargetView* GetRenderTargetView() { return m_renderTargetView; }
    ID3D11DeviceContext* GetDeviceContext() { return m_deviceContext; }

    static DX11GLFWRenderContext& GetCurrentContext();

private:
    void SetupRenderTarget();

private:
    static DX11GLFWRenderContext* s_currentContext;

    ID3D11Device* m_device;
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11DeviceContext* m_deviceContext;
    IDXGISwapChain* m_swapChain;

    int m_swapInterval;
};

}

#endif