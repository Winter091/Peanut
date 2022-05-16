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

void DX11GLFWRenderContext::PostWindowSetup(Window& window)
{
    D3D_FEATURE_LEVEL dxVersion = D3D_FEATURE_LEVEL_11_1;

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    memset(&swapChainDesc, 0, sizeof(swapChainDesc));

    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = window.GetWidth();
    swapChainDesc.BufferDesc.Height = window.GetHeight();
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    auto handle = reinterpret_cast<GLFWwindow*>(window.GetNativeHandle());
	swapChainDesc.OutputWindow = glfwGetWin32Window(handle);

	// Turn multisampling off
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

    // TODO: handle window size stuff correctly
	swapChainDesc.Windowed = true;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	swapChainDesc.Flags = 0;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, // Use default hardware render adapter
        nullptr,
#ifdef PN_DEBUG
        D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_VIDEO_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
#else
        D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_VIDEO_SUPPORT,
#endif
        &dxVersion, 1,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &m_swapChain,
        &m_device,
        nullptr,
        &m_deviceContext
    );

    if (hr != S_OK) {
        PN_CORE_ASSERT(false, "Unable to create swap chain!");
    }

    ID3D11Texture2D* backBufferPtr;
	HRESULT result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result)) {
		PN_CORE_ASSERT(false, "Unable to get back buffer");
	}

	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if(FAILED(result)) {
		PN_CORE_ASSERT(false, "Unable to get render target view");
	}

	backBufferPtr->Release();
	backBufferPtr = 0;

    SetCurrentContext(window);
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