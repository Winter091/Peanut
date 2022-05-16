#include "DX11GLFWRenderContext.hpp"

#include <Peanut/Window/Window/Impl/GLFWWindow.hpp>
#include <Peanut/Core/Core.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <d3d11.h>

namespace pn
{


DX11GLFWRenderContext* DX11GLFWRenderContext::s_currentContext = nullptr;


DX11GLFWRenderContext::DX11GLFWRenderContext()
{
    PN_CORE_INFO("Using DX11-GLFW Render Context");
}

void DX11GLFWRenderContext::PreWindowSetup()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

static DXGI_SWAP_CHAIN_DESC SetupSwapChainDesc(HWND window, int windowWidth, int windowHeight);

void DX11GLFWRenderContext::PostWindowSetup(Window& window)
{
    uint32_t contextFlags = D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_VIDEO_SUPPORT;
#ifdef PN_DEBUG
    contextFlags |= D3D11_CREATE_DEVICE_DEBUG;
    PN_CORE_INFO("Requesting DirectX 11.1 with debug layer");
#else
    PN_CORE_INFO("Requesting DirectX 11.1");
#endif

    D3D_FEATURE_LEVEL dxVersion = D3D_FEATURE_LEVEL_11_1;

    auto glfwWindow = reinterpret_cast<GLFWwindow*>(window.GetNativeHandle());
    auto winWindow = glfwGetWin32Window(glfwWindow);
    auto swapChain = SetupSwapChainDesc(winWindow, window.GetWidth(), window.GetHeight());

    HRESULT result = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, // Use default hardware render adapter
        nullptr,
        contextFlags,
        &dxVersion, 1,
        D3D11_SDK_VERSION,
        &swapChain,
        &m_swapChain,
        &m_device,
        nullptr,
        &m_deviceContext
    );

    if (result != S_OK) {
        PN_CORE_ASSERT(false, "Unable to create swap chain!");
    }

    SetupRenderTarget();
    SetCurrentContext(window);
}

static DXGI_SWAP_CHAIN_DESC SetupSwapChainDesc(HWND window, int windowWidth, int windowHeight)
{
    DXGI_SWAP_CHAIN_DESC swapChain;
    memset(&swapChain, 0, sizeof(swapChain));

    swapChain.BufferCount = 1;
    swapChain.BufferDesc.Width = windowWidth;
    swapChain.BufferDesc.Height = windowHeight;
    swapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    swapChain.BufferDesc.RefreshRate.Numerator = 0;
	swapChain.BufferDesc.RefreshRate.Denominator = 1;

	swapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChain.OutputWindow = window;

	// Turn multisampling off
	swapChain.SampleDesc.Count = 1;
	swapChain.SampleDesc.Quality = 0;

    // TODO: handle window size stuff correctly
	swapChain.Windowed = true;

	swapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChain.Flags = 0;

    return swapChain;
}

void DX11GLFWRenderContext::SetupRenderTarget()
{
    ID3D11Texture2D* backBufferPtr;
    HRESULT hr = m_swapChain->GetBuffer(0, __uuidof( ID3D11Texture2D), (void**)&backBufferPtr);
    if (FAILED(hr)) {
        PN_CORE_ASSERT(false, "IDXGISwapChain::GetBuffer() failed");
    }

    hr = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
    if (FAILED(hr)) {
        PN_CORE_ASSERT(false, "ID3D11Device::CreateRenderTargetView() failed");
    }

    backBufferPtr->Release();

    m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
}

void DX11GLFWRenderContext::OnWindowResize(Window& /*window*/)
{
    m_deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
    m_renderTargetView->Release();

    HRESULT hr = m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
    if (FAILED(hr)) {
        PN_CORE_ASSERT(false, "IDXGISwapChain::ResizeBuffers() failed");
    }

    SetupRenderTarget();
}

void DX11GLFWRenderContext::SetCurrentContext(Window& /*window*/)
{
    s_currentContext = this;
}

void DX11GLFWRenderContext::SwapBuffers(Window& /*window*/)
{
    m_swapChain->Present(1, 0);
}

DX11GLFWRenderContext& DX11GLFWRenderContext::GetCurrentContext()
{
    PN_CORE_ASSERT(s_currentContext, "Trying to get context, but it's not set");
    return *s_currentContext;
}

}