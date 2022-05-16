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
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    memset(&swapChainDesc, 0, sizeof(swapChainDesc));
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = window.GetWidth();
    swapChainDesc.BufferDesc.Height = window.GetHeight();
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    // vsync
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

    // Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
    auto handle = reinterpret_cast<GLFWwindow*>(window.GetNativeHandle());
	swapChainDesc.OutputWindow = glfwGetWin32Window(handle);

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.Windowed = true;

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

    ID3D11Device* device;
    D3D_FEATURE_LEVEL obtainedFeatureLevel;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, // adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_VIDEO_SUPPORT,
        &featureLevel, 1,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &m_swapChain, // out
        &device,
        &obtainedFeatureLevel,
        &m_deviceContext
    );

    if (hr != S_OK) {
        PN_CORE_ASSERT(false, "Unable to create swap chain!");
    }

    // Get the pointer to the back buffer.
    ID3D11Texture2D* backBufferPtr;
	int result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
	{
		PN_CORE_ASSERT(false, "Unable to get back buffer");
	}

	// Create the render target view with the back buffer pointer.
	result = device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if(FAILED(result))
	{
		PN_CORE_ASSERT(false, "Unable to get render target view");
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

    // Setup the viewport for rendering.
    D3D11_VIEWPORT viewport;
	viewport.Width = window.GetWidth();
	viewport.Height = window.GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &viewport);

    PN_CORE_INFO("==========================================");
    PN_CORE_INFO("\tFeature level is DX 11.1: {}", D3D_FEATURE_LEVEL_11_1 == static_cast<uint32_t>(obtainedFeatureLevel));
    PN_CORE_INFO("==========================================");
}

void DX11GLFWRenderContext::SetCurrentContext(Window& window)
{

}

void DX11GLFWRenderContext::SwapBuffers(Window& window)
{
    float color[4];


	// Setup the color to clear the buffer to.
	color[0] = 0.2f;
	color[1] = 0.3f;
	color[2] = 0.7f;
	color[3] = 1.0f;

	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
    m_swapChain->Present(0, 0);
}

}